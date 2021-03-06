/*-------------------------------------------------------------------------
 *
 * xlog.c
 *		PostgreSQL transaction log manager
 *
 *
 * Portions Copyright (c) 1996-2005, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/backend/access/transam/xlog.c,v 1.180 2004/12/31 21:59:30 pgsql Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "access/clog.h"
#include "access/subtrans.h"
#include "access/xact.h"
#include "access/xlog.h"
#include "access/xlog_internal.h"
#include "access/xlogutils.h"
#include "catalog/catversion.h"
#include "catalog/pg_control.h"
#include "miscadmin.h"
#include "postmaster/bgwriter.h"
#include "storage/bufpage.h"
#include "storage/fd.h"
#include "storage/lwlock.h"
#include "storage/pmsignal.h"
#include "storage/proc.h"
#include "storage/sinval.h"
#include "storage/spin.h"
#include "utils/builtins.h"
#include "utils/guc.h"
#include "utils/relcache.h"


/*
 * This chunk of hackery attempts to determine which file sync methods
 * are available on the current platform, and to choose an appropriate
 * default method.	We assume that fsync() is always available, and that
 * configure determined whether fdatasync() is.
 */
#define SYNC_METHOD_FSYNC		0
#define SYNC_METHOD_FDATASYNC	1
#define SYNC_METHOD_OPEN		2		/* used for both O_SYNC and
										 * O_DSYNC */

#if defined(O_SYNC)
#define OPEN_SYNC_FLAG	   O_SYNC
#else
#if defined(O_FSYNC)
#define OPEN_SYNC_FLAG	  O_FSYNC
#endif
#endif

#if defined(OPEN_SYNC_FLAG)
#if defined(O_DSYNC) && (O_DSYNC != OPEN_SYNC_FLAG)
#define OPEN_DATASYNC_FLAG	  O_DSYNC
#endif
#endif

#if defined(OPEN_DATASYNC_FLAG)
#define DEFAULT_SYNC_METHOD_STR    "open_datasync"
#define DEFAULT_SYNC_METHOD		   SYNC_METHOD_OPEN
#define DEFAULT_SYNC_FLAGBIT	   OPEN_DATASYNC_FLAG
#else
#if defined(HAVE_FDATASYNC)
#define DEFAULT_SYNC_METHOD_STR   "fdatasync"
#define DEFAULT_SYNC_METHOD		  SYNC_METHOD_FDATASYNC
#define DEFAULT_SYNC_FLAGBIT	  0
#else
#define DEFAULT_SYNC_METHOD_STR   "fsync"
#define DEFAULT_SYNC_METHOD		  SYNC_METHOD_FSYNC
#define DEFAULT_SYNC_FLAGBIT	  0
#endif
#endif


/* User-settable parameters */
int			CheckPointSegments = 3;
int			XLOGbuffers = 8;
char	   *XLogArchiveCommand = NULL;
char	   *XLOG_sync_method = NULL;
const char	XLOG_sync_method_default[] = DEFAULT_SYNC_METHOD_STR;

#ifdef WAL_DEBUG
bool		XLOG_DEBUG = false;
#endif

/*
 * XLOGfileslop is used in the code as the allowed "fuzz" in the number of
 * preallocated XLOG segments --- we try to have at least XLOGfiles advance
 * segments but no more than XLOGfileslop segments.  This could
 * be made a separate GUC variable, but at present I think it's sufficient
 * to hardwire it as 2*CheckPointSegments+1.  Under normal conditions, a
 * checkpoint will free no more than 2*CheckPointSegments log segments, and
 * we want to recycle all of them; the +1 allows boundary cases to happen
 * without wasting a delete/create-segment cycle.
 */

#define XLOGfileslop	(2*CheckPointSegments + 1)


/* these are derived from XLOG_sync_method by assign_xlog_sync_method */
static int	sync_method = DEFAULT_SYNC_METHOD;
static int	open_sync_bit = DEFAULT_SYNC_FLAGBIT;

#define XLOG_SYNC_BIT  (enableFsync ? open_sync_bit : 0)

#define MinXLOGbuffers	4


/*
 * ThisTimeLineID will be same in all backends --- it identifies current
 * WAL timeline for the database system.
 */
TimeLineID	ThisTimeLineID = 0;

/* Are we doing recovery from XLOG? */
bool		InRecovery = false;

/* Are we recovering using offline XLOG archives? */
static bool InArchiveRecovery = false;

/* Was the last xlog file restored from archive, or local? */
static bool restoredFromArchive = false;

/* options taken from recovery.conf */
static char *recoveryRestoreCommand = NULL;
static bool recoveryTarget = false;
static bool recoveryTargetExact = false;
static bool recoveryTargetInclusive = true;
static TransactionId recoveryTargetXid;
static time_t recoveryTargetTime;

/* if recoveryStopsHere returns true, it saves actual stop xid/time here */
static TransactionId recoveryStopXid;
static time_t recoveryStopTime;
static bool recoveryStopAfter;

/* constraint set by read_backup_label */
static XLogRecPtr recoveryMinXlogOffset = {0, 0};

/*
 * During normal operation, the only timeline we care about is ThisTimeLineID.
 * During recovery, however, things are more complicated.  To simplify life
 * for rmgr code, we keep ThisTimeLineID set to the "current" timeline as we
 * scan through the WAL history (that is, it is the line that was active when
 * the currently-scanned WAL record was generated).  We also need these
 * timeline values:
 *
 * recoveryTargetTLI: the desired timeline that we want to end in.
 *
 * expectedTLIs: an integer list of recoveryTargetTLI and the TLIs of
 * its known parents, newest first (so recoveryTargetTLI is always the
 * first list member).	Only these TLIs are expected to be seen in the WAL
 * segments we read, and indeed only these TLIs will be considered as
 * candidate WAL files to open at all.
 *
 * curFileTLI: the TLI appearing in the name of the current input WAL file.
 * (This is not necessarily the same as ThisTimeLineID, because we could
 * be scanning data that was copied from an ancestor timeline when the current
 * file was created.)  During a sequential scan we do not allow this value
 * to decrease.
 */
static TimeLineID recoveryTargetTLI;
static List *expectedTLIs;
static TimeLineID curFileTLI;

/*
 * MyLastRecPtr points to the start of the last XLOG record inserted by the
 * current transaction.  If MyLastRecPtr.xrecoff == 0, then the current
 * xact hasn't yet inserted any transaction-controlled XLOG records.
 *
 * Note that XLOG records inserted outside transaction control are not
 * reflected into MyLastRecPtr.  They do, however, cause MyXactMadeXLogEntry
 * to be set true.	The latter can be used to test whether the current xact
 * made any loggable changes (including out-of-xact changes, such as
 * sequence updates).
 *
 * When we insert/update/delete a tuple in a temporary relation, we do not
 * make any XLOG record, since we don't care about recovering the state of
 * the temp rel after a crash.	However, we will still need to remember
 * whether our transaction committed or aborted in that case.  So, we must
 * set MyXactMadeTempRelUpdate true to indicate that the XID will be of
 * interest later.
 */
XLogRecPtr	MyLastRecPtr = {0, 0};

bool		MyXactMadeXLogEntry = false;

bool		MyXactMadeTempRelUpdate = false;

/*
 * ProcLastRecPtr points to the start of the last XLOG record inserted by the
 * current backend.  It is updated for all inserts, transaction-controlled
 * or not.	ProcLastRecEnd is similar but points to end+1 of last record.
 */
static XLogRecPtr ProcLastRecPtr = {0, 0};

XLogRecPtr	ProcLastRecEnd = {0, 0};

/*
 * RedoRecPtr is this backend's local copy of the REDO record pointer
 * (which is almost but not quite the same as a pointer to the most recent
 * CHECKPOINT record).	We update this from the shared-memory copy,
 * XLogCtl->Insert.RedoRecPtr, whenever we can safely do so (ie, when we
 * hold the Insert lock).  See XLogInsert for details.	We are also allowed
 * to update from XLogCtl->Insert.RedoRecPtr if we hold the info_lck;
 * see GetRedoRecPtr.  A freshly spawned backend obtains the value during
 * InitXLOGAccess.
 */
static XLogRecPtr RedoRecPtr;

/*----------
 * Shared-memory data structures for XLOG control
 *
 * LogwrtRqst indicates a byte position that we need to write and/or fsync
 * the log up to (all records before that point must be written or fsynced).
 * LogwrtResult indicates the byte positions we have already written/fsynced.
 * These structs are identical but are declared separately to indicate their
 * slightly different functions.
 *
 * We do a lot of pushups to minimize the amount of access to lockable
 * shared memory values.  There are actually three shared-memory copies of
 * LogwrtResult, plus one unshared copy in each backend.  Here's how it works:
 *		XLogCtl->LogwrtResult is protected by info_lck
 *		XLogCtl->Write.LogwrtResult is protected by WALWriteLock
 *		XLogCtl->Insert.LogwrtResult is protected by WALInsertLock
 * One must hold the associated lock to read or write any of these, but
 * of course no lock is needed to read/write the unshared LogwrtResult.
 *
 * XLogCtl->LogwrtResult and XLogCtl->Write.LogwrtResult are both "always
 * right", since both are updated by a write or flush operation before
 * it releases WALWriteLock.  The point of keeping XLogCtl->Write.LogwrtResult
 * is that it can be examined/modified by code that already holds WALWriteLock
 * without needing to grab info_lck as well.
 *
 * XLogCtl->Insert.LogwrtResult may lag behind the reality of the other two,
 * but is updated when convenient.	Again, it exists for the convenience of
 * code that is already holding WALInsertLock but not the other locks.
 *
 * The unshared LogwrtResult may lag behind any or all of these, and again
 * is updated when convenient.
 *
 * The request bookkeeping is simpler: there is a shared XLogCtl->LogwrtRqst
 * (protected by info_lck), but we don't need to cache any copies of it.
 *
 * Note that this all works because the request and result positions can only
 * advance forward, never back up, and so we can easily determine which of two
 * values is "more up to date".
 *
 * info_lck is only held long enough to read/update the protected variables,
 * so it's a plain spinlock.  The other locks are held longer (potentially
 * over I/O operations), so we use LWLocks for them.  These locks are:
 *
 * WALInsertLock: must be held to insert a record into the WAL buffers.
 *
 * WALWriteLock: must be held to write WAL buffers to disk (XLogWrite or
 * XLogFlush).
 *
 * ControlFileLock: must be held to read/update control file or create
 * new log file.
 *
 * CheckpointLock: must be held to do a checkpoint (ensures only one
 * checkpointer at a time; even though the postmaster won't launch
 * parallel checkpoint processes, we need this because manual checkpoints
 * could be launched simultaneously).
 *
 *----------
 */

typedef struct XLogwrtRqst
{
	XLogRecPtr	Write;			/* last byte + 1 to write out */
	XLogRecPtr	Flush;			/* last byte + 1 to flush */
} XLogwrtRqst;

typedef struct XLogwrtResult
{
	XLogRecPtr	Write;			/* last byte + 1 written out */
	XLogRecPtr	Flush;			/* last byte + 1 flushed */
} XLogwrtResult;

/*
 * Shared state data for XLogInsert.
 */
typedef struct XLogCtlInsert
{
	XLogwrtResult LogwrtResult; /* a recent value of LogwrtResult */
	XLogRecPtr	PrevRecord;		/* start of previously-inserted record */
	uint16		curridx;		/* current block index in cache */
	XLogPageHeader currpage;	/* points to header of block in cache */
	char	   *currpos;		/* current insertion point in cache */
	XLogRecPtr	RedoRecPtr;		/* current redo point for insertions */
} XLogCtlInsert;

/*
 * Shared state data for XLogWrite/XLogFlush.
 */
typedef struct XLogCtlWrite
{
	XLogwrtResult LogwrtResult; /* current value of LogwrtResult */
	uint16		curridx;		/* cache index of next block to write */
} XLogCtlWrite;

/*
 * Total shared-memory state for XLOG.
 */
typedef struct XLogCtlData
{
	/* Protected by WALInsertLock: */
	XLogCtlInsert Insert;
	/* Protected by info_lck: */
	XLogwrtRqst LogwrtRqst;
	XLogwrtResult LogwrtResult;
	/* Protected by WALWriteLock: */
	XLogCtlWrite Write;

	/*
	 * These values do not change after startup, although the pointed-to
	 * pages and xlblocks values certainly do.	Permission to read/write
	 * the pages and xlblocks values depends on WALInsertLock and
	 * WALWriteLock.
	 */
	char	   *pages;			/* buffers for unwritten XLOG pages */
	XLogRecPtr *xlblocks;		/* 1st byte ptr-s + BLCKSZ */
	uint32		XLogCacheByte;	/* # bytes in xlog buffers */
	uint32		XLogCacheBlck;	/* highest allocated xlog buffer index */
	TimeLineID	ThisTimeLineID;

	slock_t		info_lck;		/* locks shared LogwrtRqst/LogwrtResult */
} XLogCtlData;

static XLogCtlData *XLogCtl = NULL;

/*
 * We maintain an image of pg_control in shared memory.
 */
static ControlFileData *ControlFile = NULL;

/*
 * Macros for managing XLogInsert state.  In most cases, the calling routine
 * has local copies of XLogCtl->Insert and/or XLogCtl->Insert->curridx,
 * so these are passed as parameters instead of being fetched via XLogCtl.
 */

/* Free space remaining in the current xlog page buffer */
#define INSERT_FREESPACE(Insert)  \
	(BLCKSZ - ((Insert)->currpos - (char *) (Insert)->currpage))

/* Construct XLogRecPtr value for current insertion point */
#define INSERT_RECPTR(recptr,Insert,curridx)  \
	( \
	  (recptr).xlogid = XLogCtl->xlblocks[curridx].xlogid, \
	  (recptr).xrecoff = \
		XLogCtl->xlblocks[curridx].xrecoff - INSERT_FREESPACE(Insert) \
	)

#define PrevBufIdx(idx)		\
		(((idx) == 0) ? XLogCtl->XLogCacheBlck : ((idx) - 1))

#define NextBufIdx(idx)		\
		(((idx) == XLogCtl->XLogCacheBlck) ? 0 : ((idx) + 1))


/* File path names */
char		XLogDir[MAXPGPATH];
static char ControlFilePath[MAXPGPATH];

/*
 * Private, possibly out-of-date copy of shared LogwrtResult.
 * See discussion above.
 */
static XLogwrtResult LogwrtResult = {{0, 0}, {0, 0}};

/*
 * openLogFile is -1 or a kernel FD for an open log file segment.
 * When it's open, openLogOff is the current seek offset in the file.
 * openLogId/openLogSeg identify the segment.  These variables are only
 * used to write the XLOG, and so will normally refer to the active segment.
 */
static int	openLogFile = -1;
static uint32 openLogId = 0;
static uint32 openLogSeg = 0;
static uint32 openLogOff = 0;

/*
 * These variables are used similarly to the ones above, but for reading
 * the XLOG.  Note, however, that readOff generally represents the offset
 * of the page just read, not the seek position of the FD itself, which
 * will be just past that page.
 */
static int	readFile = -1;
static uint32 readId = 0;
static uint32 readSeg = 0;
static uint32 readOff = 0;

/* Buffer for currently read page (BLCKSZ bytes) */
static char *readBuf = NULL;

/* Buffer for current ReadRecord result (expandable) */
static char *readRecordBuf = NULL;
static uint32 readRecordBufSize = 0;

/* State information for XLOG reading */
static XLogRecPtr ReadRecPtr;
static XLogRecPtr EndRecPtr;
static XLogRecord *nextRecord = NULL;
static TimeLineID lastPageTLI = 0;

static bool InRedo = false;


static void XLogArchiveNotify(const char *xlog);
static void XLogArchiveNotifySeg(uint32 log, uint32 seg);
static bool XLogArchiveIsDone(const char *xlog);
static void XLogArchiveCleanup(const char *xlog);
static void readRecoveryCommandFile(void);
static void exitArchiveRecovery(TimeLineID endTLI,
					uint32 endLogId, uint32 endLogSeg);
static bool recoveryStopsHere(XLogRecord *record, bool *includeThis);

static bool AdvanceXLInsertBuffer(void);
static void XLogWrite(XLogwrtRqst WriteRqst);
static int XLogFileInit(uint32 log, uint32 seg,
			 bool *use_existent, bool use_lock);
static bool InstallXLogFileSegment(uint32 log, uint32 seg, char *tmppath,
					   bool find_free, int max_advance,
					   bool use_lock);
static int	XLogFileOpen(uint32 log, uint32 seg);
static int	XLogFileRead(uint32 log, uint32 seg, int emode);
static bool RestoreArchivedFile(char *path, const char *xlogfname,
					const char *recovername, off_t expectedSize);
static int	PreallocXlogFiles(XLogRecPtr endptr);
static void MoveOfflineLogs(uint32 log, uint32 seg, XLogRecPtr endptr,
							int *nsegsremoved, int *nsegsrecycled);
static XLogRecord *ReadRecord(XLogRecPtr *RecPtr, int emode);
static bool ValidXLOGHeader(XLogPageHeader hdr, int emode);
static XLogRecord *ReadCheckpointRecord(XLogRecPtr RecPtr, int whichChkpt);
static List *readTimeLineHistory(TimeLineID targetTLI);
static bool existsTimeLineHistory(TimeLineID probeTLI);
static TimeLineID findNewestTimeLine(TimeLineID startTLI);
static void writeTimeLineHistory(TimeLineID newTLI, TimeLineID parentTLI,
					 TimeLineID endTLI,
					 uint32 endLogId, uint32 endLogSeg);
static void WriteControlFile(void);
static void ReadControlFile(void);
static char *str_time(time_t tnow);
static void issue_xlog_fsync(void);

#ifdef WAL_DEBUG
static void xlog_outrec(char *buf, XLogRecord *record);
#endif
static bool read_backup_label(XLogRecPtr *checkPointLoc);
static void remove_backup_label(void);


/*
 * Insert an XLOG record having the specified RMID and info bytes,
 * with the body of the record being the data chunk(s) described by
 * the rdata list (see xlog.h for notes about rdata).
 *
 * Returns XLOG pointer to end of record (beginning of next record).
 * This can be used as LSN for data pages affected by the logged action.
 * (LSN is the XLOG point up to which the XLOG must be flushed to disk
 * before the data page can be written out.  This implements the basic
 * WAL rule "write the log before the data".)
 *
 * NB: this routine feels free to scribble on the XLogRecData structs,
 * though not on the data they reference.  This is OK since the XLogRecData
 * structs are always just temporaries in the calling code.
 */
XLogRecPtr
XLogInsert(RmgrId rmid, uint8 info, XLogRecData *rdata)
{
	XLogCtlInsert *Insert = &XLogCtl->Insert;
	XLogRecord *record;
	XLogContRecord *contrecord;
	XLogRecPtr	RecPtr;
	XLogRecPtr	WriteRqst;
	uint32		freespace;
	uint16		curridx;
	XLogRecData *rdt;
	Buffer		dtbuf[XLR_MAX_BKP_BLOCKS];
	bool		dtbuf_bkp[XLR_MAX_BKP_BLOCKS];
	BkpBlock	dtbuf_xlg[XLR_MAX_BKP_BLOCKS];
	XLogRecPtr	dtbuf_lsn[XLR_MAX_BKP_BLOCKS];
	XLogRecData dtbuf_rdt[2 * XLR_MAX_BKP_BLOCKS];
	crc64		rdata_crc;
	uint32		len,
				write_len;
	unsigned	i;
	XLogwrtRqst LogwrtRqst;
	bool		updrqst;
	bool		no_tran = (rmid == RM_XLOG_ID) ? true : false;

	if (info & XLR_INFO_MASK)
	{
		if ((info & XLR_INFO_MASK) != XLOG_NO_TRAN)
			elog(PANIC, "invalid xlog info mask %02X", (info & XLR_INFO_MASK));
		no_tran = true;
		info &= ~XLR_INFO_MASK;
	}

	/*
	 * In bootstrap mode, we don't actually log anything but XLOG
	 * resources; return a phony record pointer.
	 */
	if (IsBootstrapProcessingMode() && rmid != RM_XLOG_ID)
	{
		RecPtr.xlogid = 0;
		RecPtr.xrecoff = SizeOfXLogLongPHD;		/* start of 1st chkpt
												 * record */
		return (RecPtr);
	}

	/*
	 * Here we scan the rdata list, determine which buffers must be backed
	 * up, and compute the CRC values for the data.  Note that the record
	 * header isn't added into the CRC yet since we don't know the final
	 * length or info bits quite yet.
	 *
	 * We may have to loop back to here if a race condition is detected
	 * below. We could prevent the race by doing all this work while
	 * holding the insert lock, but it seems better to avoid doing CRC
	 * calculations while holding the lock.  This means we have to be
	 * careful about modifying the rdata list until we know we aren't
	 * going to loop back again.  The only change we allow ourselves to
	 * make earlier is to set rdt->data = NULL in list items we have
	 * decided we will have to back up the whole buffer for.  This is OK
	 * because we will certainly decide the same thing again for those
	 * items if we do it over; doing it here saves an extra pass over the
	 * list later.
	 */
begin:;
	for (i = 0; i < XLR_MAX_BKP_BLOCKS; i++)
	{
		dtbuf[i] = InvalidBuffer;
		dtbuf_bkp[i] = false;
	}

	INIT_CRC64(rdata_crc);
	len = 0;
	for (rdt = rdata;;)
	{
		if (rdt->buffer == InvalidBuffer)
		{
			/* Simple data, just include it */
			len += rdt->len;
			COMP_CRC64(rdata_crc, rdt->data, rdt->len);
		}
		else
		{
			/* Find info for buffer */
			for (i = 0; i < XLR_MAX_BKP_BLOCKS; i++)
			{
				if (rdt->buffer == dtbuf[i])
				{
					/* Buffer already referenced by earlier list item */
					if (dtbuf_bkp[i])
						rdt->data = NULL;
					else if (rdt->data)
					{
						len += rdt->len;
						COMP_CRC64(rdata_crc, rdt->data, rdt->len);
					}
					break;
				}
				if (dtbuf[i] == InvalidBuffer)
				{
					/* OK, put it in this slot */
					dtbuf[i] = rdt->buffer;

					/*
					 * XXX We assume page LSN is first data on page
					 */
					dtbuf_lsn[i] = *((XLogRecPtr *) BufferGetBlock(rdt->buffer));
					if (XLByteLE(dtbuf_lsn[i], RedoRecPtr))
					{
						crc64		dtcrc;

						dtbuf_bkp[i] = true;
						rdt->data = NULL;
						INIT_CRC64(dtcrc);
						COMP_CRC64(dtcrc,
								   BufferGetBlock(dtbuf[i]),
								   BLCKSZ);
						dtbuf_xlg[i].node = BufferGetFileNode(dtbuf[i]);
						dtbuf_xlg[i].block = BufferGetBlockNumber(dtbuf[i]);
						COMP_CRC64(dtcrc,
								(char *) &(dtbuf_xlg[i]) + sizeof(crc64),
								   sizeof(BkpBlock) - sizeof(crc64));
						FIN_CRC64(dtcrc);
						dtbuf_xlg[i].crc = dtcrc;
					}
					else if (rdt->data)
					{
						len += rdt->len;
						COMP_CRC64(rdata_crc, rdt->data, rdt->len);
					}
					break;
				}
			}
			if (i >= XLR_MAX_BKP_BLOCKS)
				elog(PANIC, "can backup at most %d blocks per xlog record",
					 XLR_MAX_BKP_BLOCKS);
		}
		/* Break out of loop when rdt points to last list item */
		if (rdt->next == NULL)
			break;
		rdt = rdt->next;
	}

	/*
	 * NOTE: the test for len == 0 here is somewhat fishy, since in theory
	 * all of the rmgr data might have been suppressed in favor of backup
	 * blocks.	Currently, all callers of XLogInsert provide at least some
	 * not-in-a-buffer data and so len == 0 should never happen, but that
	 * may not be true forever.  If you need to remove the len == 0 check,
	 * also remove the check for xl_len == 0 in ReadRecord, below.
	 */
	if (len == 0)
		elog(PANIC, "invalid xlog record length %u", len);

	START_CRIT_SECTION();

	/* update LogwrtResult before doing cache fill check */
	{
		/* use volatile pointer to prevent code rearrangement */
		volatile XLogCtlData *xlogctl = XLogCtl;

		SpinLockAcquire_NoHoldoff(&xlogctl->info_lck);
		LogwrtRqst = xlogctl->LogwrtRqst;
		LogwrtResult = xlogctl->LogwrtResult;
		SpinLockRelease_NoHoldoff(&xlogctl->info_lck);
	}

	/*
	 * If cache is half filled then try to acquire write lock and do
	 * XLogWrite. Ignore any fractional blocks in performing this check.
	 */
	LogwrtRqst.Write.xrecoff -= LogwrtRqst.Write.xrecoff % BLCKSZ;
	if (LogwrtRqst.Write.xlogid != LogwrtResult.Write.xlogid ||
		(LogwrtRqst.Write.xrecoff >= LogwrtResult.Write.xrecoff +
		 XLogCtl->XLogCacheByte / 2))
	{
		if (LWLockConditionalAcquire(WALWriteLock, LW_EXCLUSIVE))
		{
			LogwrtResult = XLogCtl->Write.LogwrtResult;
			if (XLByteLT(LogwrtResult.Write, LogwrtRqst.Write))
				XLogWrite(LogwrtRqst);
			LWLockRelease(WALWriteLock);
		}
	}

	/* Now wait to get insert lock */
	LWLockAcquire(WALInsertLock, LW_EXCLUSIVE);

	/*
	 * Check to see if my RedoRecPtr is out of date.  If so, may have to
	 * go back and recompute everything.  This can only happen just after
	 * a checkpoint, so it's better to be slow in this case and fast
	 * otherwise.
	 */
	if (!XLByteEQ(RedoRecPtr, Insert->RedoRecPtr))
	{
		Assert(XLByteLT(RedoRecPtr, Insert->RedoRecPtr));
		RedoRecPtr = Insert->RedoRecPtr;

		for (i = 0; i < XLR_MAX_BKP_BLOCKS; i++)
		{
			if (dtbuf[i] == InvalidBuffer)
				continue;
			if (dtbuf_bkp[i] == false &&
				XLByteLE(dtbuf_lsn[i], RedoRecPtr))
			{
				/*
				 * Oops, this buffer now needs to be backed up, but we
				 * didn't think so above.  Start over.
				 */
				LWLockRelease(WALInsertLock);
				END_CRIT_SECTION();
				goto begin;
			}
		}
	}

	/*
	 * Make additional rdata list entries for the backup blocks, so that
	 * we don't need to special-case them in the write loop.  Note that we
	 * have now irrevocably changed the input rdata list.  At the exit of
	 * this loop, write_len includes the backup block data.
	 *
	 * Also set the appropriate info bits to show which buffers were backed
	 * up.	The i'th XLR_SET_BKP_BLOCK bit corresponds to the i'th
	 * distinct buffer value (ignoring InvalidBuffer) appearing in the
	 * rdata list.
	 */
	write_len = len;
	for (i = 0; i < XLR_MAX_BKP_BLOCKS; i++)
	{
		if (dtbuf[i] == InvalidBuffer || !(dtbuf_bkp[i]))
			continue;

		info |= XLR_SET_BKP_BLOCK(i);

		rdt->next = &(dtbuf_rdt[2 * i]);

		dtbuf_rdt[2 * i].data = (char *) &(dtbuf_xlg[i]);
		dtbuf_rdt[2 * i].len = sizeof(BkpBlock);
		write_len += sizeof(BkpBlock);

		rdt = dtbuf_rdt[2 * i].next = &(dtbuf_rdt[2 * i + 1]);

		dtbuf_rdt[2 * i + 1].data = (char *) BufferGetBlock(dtbuf[i]);
		dtbuf_rdt[2 * i + 1].len = BLCKSZ;
		write_len += BLCKSZ;
		dtbuf_rdt[2 * i + 1].next = NULL;
	}

	/*
	 * If there isn't enough space on the current XLOG page for a record
	 * header, advance to the next page (leaving the unused space as
	 * zeroes).
	 */
	updrqst = false;
	freespace = INSERT_FREESPACE(Insert);
	if (freespace < SizeOfXLogRecord)
	{
		updrqst = AdvanceXLInsertBuffer();
		freespace = INSERT_FREESPACE(Insert);
	}

	curridx = Insert->curridx;
	record = (XLogRecord *) Insert->currpos;

	/* Insert record header */

	record->xl_prev = Insert->PrevRecord;
	record->xl_xid = GetCurrentTransactionIdIfAny();
	record->xl_len = len;		/* doesn't include backup blocks */
	record->xl_info = info;
	record->xl_rmid = rmid;

	/* Now we can finish computing the main CRC */
	COMP_CRC64(rdata_crc, (char *) record + sizeof(crc64),
			   SizeOfXLogRecord - sizeof(crc64));
	FIN_CRC64(rdata_crc);
	record->xl_crc = rdata_crc;

	/* Compute record's XLOG location */
	INSERT_RECPTR(RecPtr, Insert, curridx);

	/* If first XLOG record of transaction, save it in PGPROC array */
	if (MyLastRecPtr.xrecoff == 0 && !no_tran)
	{
		/*
		 * We do not acquire SInvalLock here because of possible deadlock.
		 * Anyone who wants to inspect other procs' logRec must acquire
		 * WALInsertLock, instead.	A better solution would be a per-PROC
		 * spinlock, but no time for that before 7.2 --- tgl 12/19/01.
		 */
		MyProc->logRec = RecPtr;
	}

#ifdef WAL_DEBUG
	if (XLOG_DEBUG)
	{
		char		buf[8192];

		sprintf(buf, "INSERT @ %X/%X: ", RecPtr.xlogid, RecPtr.xrecoff);
		xlog_outrec(buf, record);
		if (rdata->data != NULL)
		{
			strcat(buf, " - ");
			RmgrTable[record->xl_rmid].rm_desc(buf, record->xl_info, rdata->data);
		}
		elog(LOG, "%s", buf);
	}
#endif

	/* Record begin of record in appropriate places */
	if (!no_tran)
		MyLastRecPtr = RecPtr;
	ProcLastRecPtr = RecPtr;
	Insert->PrevRecord = RecPtr;
	MyXactMadeXLogEntry = true;

	Insert->currpos += SizeOfXLogRecord;
	freespace -= SizeOfXLogRecord;

	/*
	 * Append the data, including backup blocks if any
	 */
	while (write_len)
	{
		while (rdata->data == NULL)
			rdata = rdata->next;

		if (freespace > 0)
		{
			if (rdata->len > freespace)
			{
				memcpy(Insert->currpos, rdata->data, freespace);
				rdata->data += freespace;
				rdata->len -= freespace;
				write_len -= freespace;
			}
			else
			{
				memcpy(Insert->currpos, rdata->data, rdata->len);
				freespace -= rdata->len;
				write_len -= rdata->len;
				Insert->currpos += rdata->len;
				rdata = rdata->next;
				continue;
			}
		}

		/* Use next buffer */
		updrqst = AdvanceXLInsertBuffer();
		curridx = Insert->curridx;
		/* Insert cont-record header */
		Insert->currpage->xlp_info |= XLP_FIRST_IS_CONTRECORD;
		contrecord = (XLogContRecord *) Insert->currpos;
		contrecord->xl_rem_len = write_len;
		Insert->currpos += SizeOfXLogContRecord;
		freespace = INSERT_FREESPACE(Insert);
	}

	/* Ensure next record will be properly aligned */
	Insert->currpos = (char *) Insert->currpage +
		MAXALIGN(Insert->currpos - (char *) Insert->currpage);
	freespace = INSERT_FREESPACE(Insert);

	/*
	 * The recptr I return is the beginning of the *next* record. This
	 * will be stored as LSN for changed data pages...
	 */
	INSERT_RECPTR(RecPtr, Insert, curridx);

	/* Need to update shared LogwrtRqst if some block was filled up */
	if (freespace < SizeOfXLogRecord)
		updrqst = true;			/* curridx is filled and available for
								 * writing out */
	else
		curridx = PrevBufIdx(curridx);
	WriteRqst = XLogCtl->xlblocks[curridx];

	LWLockRelease(WALInsertLock);

	if (updrqst)
	{
		/* use volatile pointer to prevent code rearrangement */
		volatile XLogCtlData *xlogctl = XLogCtl;

		SpinLockAcquire_NoHoldoff(&xlogctl->info_lck);
		/* advance global request to include new block(s) */
		if (XLByteLT(xlogctl->LogwrtRqst.Write, WriteRqst))
			xlogctl->LogwrtRqst.Write = WriteRqst;
		/* update local result copy while I have the chance */
		LogwrtResult = xlogctl->LogwrtResult;
		SpinLockRelease_NoHoldoff(&xlogctl->info_lck);
	}

	ProcLastRecEnd = RecPtr;

	END_CRIT_SECTION();

	return (RecPtr);
}

/*
 * XLogArchiveNotify
 *
 * Create an archive notification file
 *
 * The name of the notification file is the message that will be picked up
 * by the archiver, e.g. we write 0000000100000001000000C6.ready
 * and the archiver then knows to archive XLogDir/0000000100000001000000C6,
 * then when complete, rename it to 0000000100000001000000C6.done
 */
static void
XLogArchiveNotify(const char *xlog)
{
	char		archiveStatusPath[MAXPGPATH];
	FILE	   *fd;

	/* insert an otherwise empty file called <XLOG>.ready */
	StatusFilePath(archiveStatusPath, xlog, ".ready");
	fd = AllocateFile(archiveStatusPath, "w");
	if (fd == NULL)
	{
		ereport(LOG,
				(errcode_for_file_access(),
				 errmsg("could not create archive status file \"%s\": %m",
						archiveStatusPath)));
		return;
	}
	if (FreeFile(fd))
	{
		ereport(LOG,
				(errcode_for_file_access(),
				 errmsg("could not write archive status file \"%s\": %m",
						archiveStatusPath)));
		return;
	}

	/* Notify archiver that it's got something to do */
	if (IsUnderPostmaster)
		SendPostmasterSignal(PMSIGNAL_WAKEN_ARCHIVER);
}

/*
 * Convenience routine to notify using log/seg representation of filename
 */
static void
XLogArchiveNotifySeg(uint32 log, uint32 seg)
{
	char		xlog[MAXFNAMELEN];

	XLogFileName(xlog, ThisTimeLineID, log, seg);
	XLogArchiveNotify(xlog);
}

/*
 * XLogArchiveIsDone
 *
 * Checks for a ".done" archive notification file.	This is called when we
 * are ready to delete or recycle an old XLOG segment file.  If it is okay
 * to delete it then return true.
 *
 * If <XLOG>.done exists, then return true; else if <XLOG>.ready exists,
 * then return false; else create <XLOG>.ready and return false.  The
 * last case covers the possibility that the original attempt to create
 * <XLOG>.ready failed.
 */
static bool
XLogArchiveIsDone(const char *xlog)
{
	char		archiveStatusPath[MAXPGPATH];
	struct stat stat_buf;

	/* First check for .done --- this is the expected case */
	StatusFilePath(archiveStatusPath, xlog, ".done");
	if (stat(archiveStatusPath, &stat_buf) == 0)
		return true;

	/* check for .ready --- this means archiver is still busy with it */
	StatusFilePath(archiveStatusPath, xlog, ".ready");
	if (stat(archiveStatusPath, &stat_buf) == 0)
		return false;

	/* Race condition --- maybe archiver just finished, so recheck */
	StatusFilePath(archiveStatusPath, xlog, ".done");
	if (stat(archiveStatusPath, &stat_buf) == 0)
		return true;

	/* Retry creation of the .ready file */
	XLogArchiveNotify(xlog);
	return false;
}

/*
 * XLogArchiveCleanup
 *
 * Cleanup archive notification file(s) for a particular xlog segment
 */
static void
XLogArchiveCleanup(const char *xlog)
{
	char		archiveStatusPath[MAXPGPATH];

	/* Remove the .done file */
	StatusFilePath(archiveStatusPath, xlog, ".done");
	unlink(archiveStatusPath);
	/* should we complain about failure? */

	/* Remove the .ready file if present --- normally it shouldn't be */
	StatusFilePath(archiveStatusPath, xlog, ".ready");
	unlink(archiveStatusPath);
	/* should we complain about failure? */
}

/*
 * Advance the Insert state to the next buffer page, writing out the next
 * buffer if it still contains unwritten data.
 *
 * The global LogwrtRqst.Write pointer needs to be advanced to include the
 * just-filled page.  If we can do this for free (without an extra lock),
 * we do so here.  Otherwise the caller must do it.  We return TRUE if the
 * request update still needs to be done, FALSE if we did it internally.
 *
 * Must be called with WALInsertLock held.
 */
static bool
AdvanceXLInsertBuffer(void)
{
	XLogCtlInsert *Insert = &XLogCtl->Insert;
	XLogCtlWrite *Write = &XLogCtl->Write;
	uint16		nextidx = NextBufIdx(Insert->curridx);
	bool		update_needed = true;
	XLogRecPtr	OldPageRqstPtr;
	XLogwrtRqst WriteRqst;
	XLogRecPtr	NewPageEndPtr;
	XLogPageHeader NewPage;

	/* Use Insert->LogwrtResult copy if it's more fresh */
	if (XLByteLT(LogwrtResult.Write, Insert->LogwrtResult.Write))
		LogwrtResult = Insert->LogwrtResult;

	/*
	 * Get ending-offset of the buffer page we need to replace (this may
	 * be zero if the buffer hasn't been used yet).  Fall through if it's
	 * already written out.
	 */
	OldPageRqstPtr = XLogCtl->xlblocks[nextidx];
	if (!XLByteLE(OldPageRqstPtr, LogwrtResult.Write))
	{
		/* nope, got work to do... */
		XLogRecPtr	FinishedPageRqstPtr;

		FinishedPageRqstPtr = XLogCtl->xlblocks[Insert->curridx];

		/* Before waiting, get info_lck and update LogwrtResult */
		{
			/* use volatile pointer to prevent code rearrangement */
			volatile XLogCtlData *xlogctl = XLogCtl;

			SpinLockAcquire_NoHoldoff(&xlogctl->info_lck);
			if (XLByteLT(xlogctl->LogwrtRqst.Write, FinishedPageRqstPtr))
				xlogctl->LogwrtRqst.Write = FinishedPageRqstPtr;
			LogwrtResult = xlogctl->LogwrtResult;
			SpinLockRelease_NoHoldoff(&xlogctl->info_lck);
		}

		update_needed = false;	/* Did the shared-request update */

		if (XLByteLE(OldPageRqstPtr, LogwrtResult.Write))
		{
			/* OK, someone wrote it already */
			Insert->LogwrtResult = LogwrtResult;
		}
		else
		{
			/* Must acquire write lock */
			LWLockAcquire(WALWriteLock, LW_EXCLUSIVE);
			LogwrtResult = Write->LogwrtResult;
			if (XLByteLE(OldPageRqstPtr, LogwrtResult.Write))
			{
				/* OK, someone wrote it already */
				LWLockRelease(WALWriteLock);
				Insert->LogwrtResult = LogwrtResult;
			}
			else
			{
				/*
				 * Have to write buffers while holding insert lock. This
				 * is not good, so only write as much as we absolutely
				 * must.
				 */
				WriteRqst.Write = OldPageRqstPtr;
				WriteRqst.Flush.xlogid = 0;
				WriteRqst.Flush.xrecoff = 0;
				XLogWrite(WriteRqst);
				LWLockRelease(WALWriteLock);
				Insert->LogwrtResult = LogwrtResult;
			}
		}
	}

	/*
	 * Now the next buffer slot is free and we can set it up to be the
	 * next output page.
	 */
	NewPageEndPtr = XLogCtl->xlblocks[Insert->curridx];
	if (NewPageEndPtr.xrecoff >= XLogFileSize)
	{
		/* crossing a logid boundary */
		NewPageEndPtr.xlogid += 1;
		NewPageEndPtr.xrecoff = BLCKSZ;
	}
	else
		NewPageEndPtr.xrecoff += BLCKSZ;
	XLogCtl->xlblocks[nextidx] = NewPageEndPtr;
	NewPage = (XLogPageHeader) (XLogCtl->pages + nextidx * BLCKSZ);
	Insert->curridx = nextidx;
	Insert->currpage = NewPage;
	Insert->currpos = ((char *) NewPage) + SizeOfXLogShortPHD;

	/*
	 * Be sure to re-zero the buffer so that bytes beyond what we've
	 * written will look like zeroes and not valid XLOG records...
	 */
	MemSet((char *) NewPage, 0, BLCKSZ);

	/*
	 * Fill the new page's header
	 */
	NewPage->xlp_magic = XLOG_PAGE_MAGIC;
	/* NewPage->xlp_info = 0; */	/* done by memset */
	NewPage->xlp_tli = ThisTimeLineID;
	NewPage->xlp_pageaddr.xlogid = NewPageEndPtr.xlogid;
	NewPage->xlp_pageaddr.xrecoff = NewPageEndPtr.xrecoff - BLCKSZ;

	/*
	 * If first page of an XLOG segment file, make it a long header.
	 */
	if ((NewPage->xlp_pageaddr.xrecoff % XLogSegSize) == 0)
	{
		XLogLongPageHeader NewLongPage = (XLogLongPageHeader) NewPage;

		NewLongPage->xlp_sysid = ControlFile->system_identifier;
		NewLongPage->xlp_seg_size = XLogSegSize;
		NewPage->xlp_info |= XLP_LONG_HEADER;
		Insert->currpos = ((char *) NewPage) + SizeOfXLogLongPHD;
	}

	return update_needed;
}

/*
 * Write and/or fsync the log at least as far as WriteRqst indicates.
 *
 * Must be called with WALWriteLock held.
 */
static void
XLogWrite(XLogwrtRqst WriteRqst)
{
	XLogCtlWrite *Write = &XLogCtl->Write;
	char	   *from;
	bool		ispartialpage;
	bool		use_existent;

	/*
	 * Update local LogwrtResult (caller probably did this already,
	 * but...)
	 */
	LogwrtResult = Write->LogwrtResult;

	while (XLByteLT(LogwrtResult.Write, WriteRqst.Write))
	{
		/*
		 * Make sure we're not ahead of the insert process.  This could
		 * happen if we're passed a bogus WriteRqst.Write that is past the
		 * end of the last page that's been initialized by
		 * AdvanceXLInsertBuffer.
		 */
		if (!XLByteLT(LogwrtResult.Write, XLogCtl->xlblocks[Write->curridx]))
			elog(PANIC, "xlog write request %X/%X is past end of log %X/%X",
				 LogwrtResult.Write.xlogid, LogwrtResult.Write.xrecoff,
				 XLogCtl->xlblocks[Write->curridx].xlogid,
				 XLogCtl->xlblocks[Write->curridx].xrecoff);

		/* Advance LogwrtResult.Write to end of current buffer page */
		LogwrtResult.Write = XLogCtl->xlblocks[Write->curridx];
		ispartialpage = XLByteLT(WriteRqst.Write, LogwrtResult.Write);

		if (!XLByteInPrevSeg(LogwrtResult.Write, openLogId, openLogSeg))
		{
			/*
			 * Switch to new logfile segment.
			 */
			if (openLogFile >= 0)
			{
				if (close(openLogFile))
					ereport(PANIC,
							(errcode_for_file_access(),
					errmsg("could not close log file %u, segment %u: %m",
						   openLogId, openLogSeg)));
				openLogFile = -1;
			}
			XLByteToPrevSeg(LogwrtResult.Write, openLogId, openLogSeg);

			/* create/use new log file */
			use_existent = true;
			openLogFile = XLogFileInit(openLogId, openLogSeg,
									   &use_existent, true);
			openLogOff = 0;

			/* update pg_control, unless someone else already did */
			LWLockAcquire(ControlFileLock, LW_EXCLUSIVE);
			if (ControlFile->logId < openLogId ||
				(ControlFile->logId == openLogId &&
				 ControlFile->logSeg < openLogSeg + 1))
			{
				ControlFile->logId = openLogId;
				ControlFile->logSeg = openLogSeg + 1;
				ControlFile->time = time(NULL);
				UpdateControlFile();

				/*
				 * Signal bgwriter to start a checkpoint if it's been
				 * too long since the last one.  (We look at local copy of
				 * RedoRecPtr which might be a little out of date, but
				 * should be close enough for this purpose.)
				 *
				 * A straight computation of segment number could overflow
				 * 32 bits.  Rather than assuming we have working 64-bit
				 * arithmetic, we compare the highest-order bits separately,
				 * and force a checkpoint immediately when they change.
				 */
				if (IsUnderPostmaster)
				{
					uint32		old_segno,
								new_segno;
					uint32		old_highbits,
								new_highbits;

					old_segno = (RedoRecPtr.xlogid % XLogSegSize) * XLogSegsPerFile +
						(RedoRecPtr.xrecoff / XLogSegSize);
					old_highbits = RedoRecPtr.xlogid / XLogSegSize;
					new_segno = (openLogId % XLogSegSize) * XLogSegsPerFile +
						openLogSeg;
					new_highbits = openLogId / XLogSegSize;
					if (new_highbits != old_highbits ||
						new_segno >= old_segno + (uint32) CheckPointSegments)
					{
#ifdef WAL_DEBUG
						if (XLOG_DEBUG)
							elog(LOG, "time for a checkpoint, signaling bgwriter");
#endif
						RequestCheckpoint(false);
					}
				}
			}
			LWLockRelease(ControlFileLock);
		}

		if (openLogFile < 0)
		{
			XLByteToPrevSeg(LogwrtResult.Write, openLogId, openLogSeg);
			openLogFile = XLogFileOpen(openLogId, openLogSeg);
			openLogOff = 0;
		}

		/* Need to seek in the file? */
		if (openLogOff != (LogwrtResult.Write.xrecoff - BLCKSZ) % XLogSegSize)
		{
			openLogOff = (LogwrtResult.Write.xrecoff - BLCKSZ) % XLogSegSize;
			if (lseek(openLogFile, (off_t) openLogOff, SEEK_SET) < 0)
				ereport(PANIC,
						(errcode_for_file_access(),
						 errmsg("could not seek in log file %u, segment %u to offset %u: %m",
								openLogId, openLogSeg, openLogOff)));
		}

		/* OK to write the page */
		from = XLogCtl->pages + Write->curridx * BLCKSZ;
		errno = 0;
		if (write(openLogFile, from, BLCKSZ) != BLCKSZ)
		{
			/* if write didn't set errno, assume problem is no disk space */
			if (errno == 0)
				errno = ENOSPC;
			ereport(PANIC,
					(errcode_for_file_access(),
					 errmsg("could not write to log file %u, segment %u at offset %u: %m",
							openLogId, openLogSeg, openLogOff)));
		}
		openLogOff += BLCKSZ;

		/*
		 * If we just wrote the whole last page of a logfile segment,
		 * fsync the segment immediately.  This avoids having to go back
		 * and re-open prior segments when an fsync request comes along
		 * later. Doing it here ensures that one and only one backend will
		 * perform this fsync.
		 *
		 * This is also the right place to notify the Archiver that the
		 * segment is ready to copy to archival storage.
		 */
		if (openLogOff >= XLogSegSize && !ispartialpage)
		{
			issue_xlog_fsync();
			LogwrtResult.Flush = LogwrtResult.Write;	/* end of current page */

			if (XLogArchivingActive())
				XLogArchiveNotifySeg(openLogId, openLogSeg);
		}

		if (ispartialpage)
		{
			/* Only asked to write a partial page */
			LogwrtResult.Write = WriteRqst.Write;
			break;
		}
		Write->curridx = NextBufIdx(Write->curridx);
	}

	/*
	 * If asked to flush, do so
	 */
	if (XLByteLT(LogwrtResult.Flush, WriteRqst.Flush) &&
		XLByteLT(LogwrtResult.Flush, LogwrtResult.Write))
	{
		/*
		 * Could get here without iterating above loop, in which case we
		 * might have no open file or the wrong one.  However, we do not
		 * need to fsync more than one file.
		 */
		if (sync_method != SYNC_METHOD_OPEN)
		{
			if (openLogFile >= 0 &&
			 !XLByteInPrevSeg(LogwrtResult.Write, openLogId, openLogSeg))
			{
				if (close(openLogFile))
					ereport(PANIC,
							(errcode_for_file_access(),
					errmsg("could not close log file %u, segment %u: %m",
						   openLogId, openLogSeg)));
				openLogFile = -1;
			}
			if (openLogFile < 0)
			{
				XLByteToPrevSeg(LogwrtResult.Write, openLogId, openLogSeg);
				openLogFile = XLogFileOpen(openLogId, openLogSeg);
				openLogOff = 0;
			}
			issue_xlog_fsync();
		}
		LogwrtResult.Flush = LogwrtResult.Write;
	}

	/*
	 * Update shared-memory status
	 *
	 * We make sure that the shared 'request' values do not fall behind the
	 * 'result' values.  This is not absolutely essential, but it saves
	 * some code in a couple of places.
	 */
	{
		/* use volatile pointer to prevent code rearrangement */
		volatile XLogCtlData *xlogctl = XLogCtl;

		SpinLockAcquire_NoHoldoff(&xlogctl->info_lck);
		xlogctl->LogwrtResult = LogwrtResult;
		if (XLByteLT(xlogctl->LogwrtRqst.Write, LogwrtResult.Write))
			xlogctl->LogwrtRqst.Write = LogwrtResult.Write;
		if (XLByteLT(xlogctl->LogwrtRqst.Flush, LogwrtResult.Flush))
			xlogctl->LogwrtRqst.Flush = LogwrtResult.Flush;
		SpinLockRelease_NoHoldoff(&xlogctl->info_lck);
	}

	Write->LogwrtResult = LogwrtResult;
}

/*
 * Ensure that all XLOG data through the given position is flushed to disk.
 *
 * NOTE: this differs from XLogWrite mainly in that the WALWriteLock is not
 * already held, and we try to avoid acquiring it if possible.
 */
void
XLogFlush(XLogRecPtr record)
{
	XLogRecPtr	WriteRqstPtr;
	XLogwrtRqst WriteRqst;

	/* Disabled during REDO */
	if (InRedo)
		return;

	/* Quick exit if already known flushed */
	if (XLByteLE(record, LogwrtResult.Flush))
		return;

#ifdef WAL_DEBUG
	if (XLOG_DEBUG)
		elog(LOG, "xlog flush request %X/%X; write %X/%X; flush %X/%X",
			 record.xlogid, record.xrecoff,
			 LogwrtResult.Write.xlogid, LogwrtResult.Write.xrecoff,
			 LogwrtResult.Flush.xlogid, LogwrtResult.Flush.xrecoff);
#endif

	START_CRIT_SECTION();

	/*
	 * Since fsync is usually a horribly expensive operation, we try to
	 * piggyback as much data as we can on each fsync: if we see any more
	 * data entered into the xlog buffer, we'll write and fsync that too,
	 * so that the final value of LogwrtResult.Flush is as large as
	 * possible. This gives us some chance of avoiding another fsync
	 * immediately after.
	 */

	/* initialize to given target; may increase below */
	WriteRqstPtr = record;

	/* read LogwrtResult and update local state */
	{
		/* use volatile pointer to prevent code rearrangement */
		volatile XLogCtlData *xlogctl = XLogCtl;

		SpinLockAcquire_NoHoldoff(&xlogctl->info_lck);
		if (XLByteLT(WriteRqstPtr, xlogctl->LogwrtRqst.Write))
			WriteRqstPtr = xlogctl->LogwrtRqst.Write;
		LogwrtResult = xlogctl->LogwrtResult;
		SpinLockRelease_NoHoldoff(&xlogctl->info_lck);
	}

	/* done already? */
	if (!XLByteLE(record, LogwrtResult.Flush))
	{
		/* now wait for the write lock */
		LWLockAcquire(WALWriteLock, LW_EXCLUSIVE);
		LogwrtResult = XLogCtl->Write.LogwrtResult;
		if (!XLByteLE(record, LogwrtResult.Flush))
		{
			/* try to write/flush later additions to XLOG as well */
			if (LWLockConditionalAcquire(WALInsertLock, LW_EXCLUSIVE))
			{
				XLogCtlInsert *Insert = &XLogCtl->Insert;
				uint32		freespace = INSERT_FREESPACE(Insert);

				if (freespace < SizeOfXLogRecord)		/* buffer is full */
					WriteRqstPtr = XLogCtl->xlblocks[Insert->curridx];
				else
				{
					WriteRqstPtr = XLogCtl->xlblocks[Insert->curridx];
					WriteRqstPtr.xrecoff -= freespace;
				}
				LWLockRelease(WALInsertLock);
				WriteRqst.Write = WriteRqstPtr;
				WriteRqst.Flush = WriteRqstPtr;
			}
			else
			{
				WriteRqst.Write = WriteRqstPtr;
				WriteRqst.Flush = record;
			}
			XLogWrite(WriteRqst);
		}
		LWLockRelease(WALWriteLock);
	}

	END_CRIT_SECTION();

	/*
	 * If we still haven't flushed to the request point then we have a
	 * problem; most likely, the requested flush point is past end of
	 * XLOG. This has been seen to occur when a disk page has a corrupted
	 * LSN.
	 *
	 * Formerly we treated this as a PANIC condition, but that hurts the
	 * system's robustness rather than helping it: we do not want to take
	 * down the whole system due to corruption on one data page.  In
	 * particular, if the bad page is encountered again during recovery
	 * then we would be unable to restart the database at all!	(This
	 * scenario has actually happened in the field several times with 7.1
	 * releases. Note that we cannot get here while InRedo is true, but if
	 * the bad page is brought in and marked dirty during recovery then
	 * CreateCheckPoint will try to flush it at the end of recovery.)
	 *
	 * The current approach is to ERROR under normal conditions, but only
	 * WARNING during recovery, so that the system can be brought up even
	 * if there's a corrupt LSN.  Note that for calls from xact.c, the
	 * ERROR will be promoted to PANIC since xact.c calls this routine
	 * inside a critical section.  However, calls from bufmgr.c are not
	 * within critical sections and so we will not force a restart for a
	 * bad LSN on a data page.
	 */
	if (XLByteLT(LogwrtResult.Flush, record))
		elog(InRecovery ? WARNING : ERROR,
			 "xlog flush request %X/%X is not satisfied --- flushed only to %X/%X",
			 record.xlogid, record.xrecoff,
			 LogwrtResult.Flush.xlogid, LogwrtResult.Flush.xrecoff);
}

/*
 * Create a new XLOG file segment, or open a pre-existing one.
 *
 * log, seg: identify segment to be created/opened.
 *
 * *use_existent: if TRUE, OK to use a pre-existing file (else, any
 * pre-existing file will be deleted).	On return, TRUE if a pre-existing
 * file was used.
 *
 * use_lock: if TRUE, acquire ControlFileLock while moving file into
 * place.  This should be TRUE except during bootstrap log creation.  The
 * caller must *not* hold the lock at call.
 *
 * Returns FD of opened file.
 */
static int
XLogFileInit(uint32 log, uint32 seg,
			 bool *use_existent, bool use_lock)
{
	char		path[MAXPGPATH];
	char		tmppath[MAXPGPATH];
	char		zbuffer[BLCKSZ];
	int			fd;
	int			nbytes;

	XLogFilePath(path, ThisTimeLineID, log, seg);

	/*
	 * Try to use existent file (checkpoint maker may have created it
	 * already)
	 */
	if (*use_existent)
	{
		fd = BasicOpenFile(path, O_RDWR | PG_BINARY | XLOG_SYNC_BIT,
						   S_IRUSR | S_IWUSR);
		if (fd < 0)
		{
			if (errno != ENOENT)
				ereport(PANIC,
						(errcode_for_file_access(),
						 errmsg("could not open file \"%s\" (log file %u, segment %u): %m",
								path, log, seg)));
		}
		else
			return (fd);
	}

	/*
	 * Initialize an empty (all zeroes) segment.  NOTE: it is possible
	 * that another process is doing the same thing.  If so, we will end
	 * up pre-creating an extra log segment.  That seems OK, and better
	 * than holding the lock throughout this lengthy process.
	 */
	snprintf(tmppath, MAXPGPATH, "%s/xlogtemp.%d", XLogDir, (int)getpid());

	unlink(tmppath);

	/* do not use XLOG_SYNC_BIT here --- want to fsync only at end of fill */
	fd = BasicOpenFile(tmppath, O_RDWR | O_CREAT | O_EXCL | PG_BINARY,
					   S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not create file \"%s\": %m", tmppath)));

	/*
	 * Zero-fill the file.	We have to do this the hard way to ensure that
	 * all the file space has really been allocated --- on platforms that
	 * allow "holes" in files, just seeking to the end doesn't allocate
	 * intermediate space.	This way, we know that we have all the space
	 * and (after the fsync below) that all the indirect blocks are down
	 * on disk.  Therefore, fdatasync(2) or O_DSYNC will be sufficient to
	 * sync future writes to the log file.
	 */
	MemSet(zbuffer, 0, sizeof(zbuffer));
	for (nbytes = 0; nbytes < XLogSegSize; nbytes += sizeof(zbuffer))
	{
		errno = 0;
		if ((int) write(fd, zbuffer, sizeof(zbuffer)) != (int) sizeof(zbuffer))
		{
			int			save_errno = errno;

			/*
			 * If we fail to make the file, delete it to release disk
			 * space
			 */
			unlink(tmppath);
			/* if write didn't set errno, assume problem is no disk space */
			errno = save_errno ? save_errno : ENOSPC;

			ereport(PANIC,
					(errcode_for_file_access(),
				 errmsg("could not write to file \"%s\": %m", tmppath)));
		}
	}

	if (pg_fsync(fd) != 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not fsync file \"%s\": %m", tmppath)));

	if (close(fd))
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not close file \"%s\": %m", tmppath)));

	/*
	 * Now move the segment into place with its final name.
	 *
	 * If caller didn't want to use a pre-existing file, get rid of any
	 * pre-existing file.  Otherwise, cope with possibility that someone
	 * else has created the file while we were filling ours: if so, use
	 * ours to pre-create a future log segment.
	 */
	if (!InstallXLogFileSegment(log, seg, tmppath,
								*use_existent, XLOGfileslop,
								use_lock))
	{
		/* No need for any more future segments... */
		unlink(tmppath);
	}

	/* Set flag to tell caller there was no existent file */
	*use_existent = false;

	/* Now open original target segment (might not be file I just made) */
	fd = BasicOpenFile(path, O_RDWR | PG_BINARY | XLOG_SYNC_BIT,
					   S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
		errmsg("could not open file \"%s\" (log file %u, segment %u): %m",
			   path, log, seg)));

	return (fd);
}

/*
 * Create a new XLOG file segment by copying a pre-existing one.
 *
 * log, seg: identify segment to be created.
 *
 * srcTLI, srclog, srcseg: identify segment to be copied (could be from
 *		a different timeline)
 *
 * Currently this is only used during recovery, and so there are no locking
 * considerations.	But we should be just as tense as XLogFileInit to avoid
 * emplacing a bogus file.
 */
static void
XLogFileCopy(uint32 log, uint32 seg,
			 TimeLineID srcTLI, uint32 srclog, uint32 srcseg)
{
	char		path[MAXPGPATH];
	char		tmppath[MAXPGPATH];
	char		buffer[BLCKSZ];
	int			srcfd;
	int			fd;
	int			nbytes;

	/*
	 * Open the source file
	 */
	XLogFilePath(path, srcTLI, srclog, srcseg);
	srcfd = BasicOpenFile(path, O_RDONLY | PG_BINARY, 0);
	if (srcfd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not open file \"%s\": %m", path)));

	/*
	 * Copy into a temp file name.
	 */
	snprintf(tmppath, MAXPGPATH, "%s/xlogtemp.%d", XLogDir, (int)getpid());

	unlink(tmppath);

	/* do not use XLOG_SYNC_BIT here --- want to fsync only at end of fill */
	fd = BasicOpenFile(tmppath, O_RDWR | O_CREAT | O_EXCL | PG_BINARY,
					   S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not create file \"%s\": %m", tmppath)));

	/*
	 * Do the data copying.
	 */
	for (nbytes = 0; nbytes < XLogSegSize; nbytes += sizeof(buffer))
	{
		errno = 0;
		if ((int) read(srcfd, buffer, sizeof(buffer)) != (int) sizeof(buffer))
		{
			if (errno != 0)
				ereport(PANIC,
						(errcode_for_file_access(),
						 errmsg("could not read file \"%s\": %m", path)));
			else
				ereport(PANIC,
					 (errmsg("not enough data in file \"%s\"", path)));
		}
		errno = 0;
		if ((int) write(fd, buffer, sizeof(buffer)) != (int) sizeof(buffer))
		{
			int			save_errno = errno;

			/*
			 * If we fail to make the file, delete it to release disk
			 * space
			 */
			unlink(tmppath);
			/* if write didn't set errno, assume problem is no disk space */
			errno = save_errno ? save_errno : ENOSPC;

			ereport(PANIC,
					(errcode_for_file_access(),
				 errmsg("could not write to file \"%s\": %m", tmppath)));
		}
	}

	if (pg_fsync(fd) != 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not fsync file \"%s\": %m", tmppath)));

	if (close(fd))
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not close file \"%s\": %m", tmppath)));

	close(srcfd);

	/*
	 * Now move the segment into place with its final name.
	 */
	if (!InstallXLogFileSegment(log, seg, tmppath, false, 0, false))
		elog(PANIC, "InstallXLogFileSegment should not have failed");
}

/*
 * Install a new XLOG segment file as a current or future log segment.
 *
 * This is used both to install a newly-created segment (which has a temp
 * filename while it's being created) and to recycle an old segment.
 *
 * log, seg: identify segment to install as (or first possible target).
 *
 * tmppath: initial name of file to install.  It will be renamed into place.
 *
 * find_free: if TRUE, install the new segment at the first empty log/seg
 * number at or after the passed numbers.  If FALSE, install the new segment
 * exactly where specified, deleting any existing segment file there.
 *
 * max_advance: maximum number of log/seg slots to advance past the starting
 * point.  Fail if no free slot is found in this range.  (Irrelevant if
 * find_free is FALSE.)
 *
 * use_lock: if TRUE, acquire ControlFileLock while moving file into
 * place.  This should be TRUE except during bootstrap log creation.  The
 * caller must *not* hold the lock at call.
 *
 * Returns TRUE if file installed, FALSE if not installed because of
 * exceeding max_advance limit.  (Any other kind of failure causes ereport().)
 */
static bool
InstallXLogFileSegment(uint32 log, uint32 seg, char *tmppath,
					   bool find_free, int max_advance,
					   bool use_lock)
{
	char		path[MAXPGPATH];
	struct stat stat_buf;

	XLogFilePath(path, ThisTimeLineID, log, seg);

	/*
	 * We want to be sure that only one process does this at a time.
	 */
	if (use_lock)
		LWLockAcquire(ControlFileLock, LW_EXCLUSIVE);

	if (!find_free)
	{
		/* Force installation: get rid of any pre-existing segment file */
		unlink(path);
	}
	else
	{
		/* Find a free slot to put it in */
		while (stat(path, &stat_buf) == 0)
		{
			if (--max_advance < 0)
			{
				/* Failed to find a free slot within specified range */
				if (use_lock)
					LWLockRelease(ControlFileLock);
				return false;
			}
			NextLogSeg(log, seg);
			XLogFilePath(path, ThisTimeLineID, log, seg);
		}
	}

	/*
	 * Prefer link() to rename() here just to be really sure that we don't
	 * overwrite an existing logfile.  However, there shouldn't be one, so
	 * rename() is an acceptable substitute except for the truly paranoid.
	 */
#if HAVE_WORKING_LINK
	if (link(tmppath, path) < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not link file \"%s\" to \"%s\" (initialization of log file %u, segment %u): %m",
						tmppath, path, log, seg)));
	unlink(tmppath);
#else
	if (rename(tmppath, path) < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not rename file \"%s\" to \"%s\" (initialization of log file %u, segment %u): %m",
						tmppath, path, log, seg)));
#endif

	if (use_lock)
		LWLockRelease(ControlFileLock);

	return true;
}

/*
 * Open a pre-existing logfile segment for writing.
 */
static int
XLogFileOpen(uint32 log, uint32 seg)
{
	char		path[MAXPGPATH];
	int			fd;

	XLogFilePath(path, ThisTimeLineID, log, seg);

	fd = BasicOpenFile(path, O_RDWR | PG_BINARY | XLOG_SYNC_BIT,
					   S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
		errmsg("could not open file \"%s\" (log file %u, segment %u): %m",
			   path, log, seg)));

	return fd;
}

/*
 * Open a logfile segment for reading (during recovery).
 */
static int
XLogFileRead(uint32 log, uint32 seg, int emode)
{
	char		path[MAXPGPATH];
	char		xlogfname[MAXFNAMELEN];
	ListCell   *cell;
	int			fd;

	/*
	 * Loop looking for a suitable timeline ID: we might need to read any
	 * of the timelines listed in expectedTLIs.
	 *
	 * We expect curFileTLI on entry to be the TLI of the preceding file in
	 * sequence, or 0 if there was no predecessor.	We do not allow
	 * curFileTLI to go backwards; this prevents us from picking up the
	 * wrong file when a parent timeline extends to higher segment numbers
	 * than the child we want to read.
	 */
	foreach(cell, expectedTLIs)
	{
		TimeLineID	tli = (TimeLineID) lfirst_int(cell);

		if (tli < curFileTLI)
			break;				/* don't bother looking at too-old TLIs */

		if (InArchiveRecovery)
		{
			XLogFileName(xlogfname, tli, log, seg);
			restoredFromArchive = RestoreArchivedFile(path, xlogfname,
													  "RECOVERYXLOG",
													  XLogSegSize);
		}
		else
			XLogFilePath(path, tli, log, seg);

		fd = BasicOpenFile(path, O_RDONLY | PG_BINARY, 0);
		if (fd >= 0)
		{
			/* Success! */
			curFileTLI = tli;
			return fd;
		}
		if (errno != ENOENT)	/* unexpected failure? */
			ereport(PANIC,
					(errcode_for_file_access(),
					 errmsg("could not open file \"%s\" (log file %u, segment %u): %m",
							path, log, seg)));
	}

	/* Couldn't find it.  For simplicity, complain about front timeline */
	XLogFilePath(path, recoveryTargetTLI, log, seg);
	errno = ENOENT;
	ereport(emode,
			(errcode_for_file_access(),
	   errmsg("could not open file \"%s\" (log file %u, segment %u): %m",
			  path, log, seg)));
	return -1;
}

/*
 * Attempt to retrieve the specified file from off-line archival storage.
 * If successful, fill "path" with its complete path (note that this will be
 * a temp file name that doesn't follow the normal naming convention), and
 * return TRUE.
 *
 * If not successful, fill "path" with the name of the normal on-line file
 * (which may or may not actually exist, but we'll try to use it), and return
 * FALSE.
 *
 * For fixed-size files, the caller may pass the expected size as an
 * additional crosscheck on successful recovery.  If the file size is not
 * known, set expectedSize = 0.
 */
static bool
RestoreArchivedFile(char *path, const char *xlogfname,
					const char *recovername, off_t expectedSize)
{
	char		xlogpath[MAXPGPATH];
	char		xlogRestoreCmd[MAXPGPATH];
	char	   *dp;
	char	   *endp;
	const char *sp;
	int			rc;
	struct stat stat_buf;

	/*
	 * When doing archive recovery, we always prefer an archived log file
	 * even if a file of the same name exists in XLogDir.  The reason is
	 * that the file in XLogDir could be an old, un-filled or
	 * partly-filled version that was copied and restored as part of
	 * backing up $PGDATA.
	 *
	 * We could try to optimize this slightly by checking the local copy
	 * lastchange timestamp against the archived copy, but we have no API
	 * to do this, nor can we guarantee that the lastchange timestamp was
	 * preserved correctly when we copied to archive. Our aim is
	 * robustness, so we elect not to do this.
	 *
	 * If we cannot obtain the log file from the archive, however, we will
	 * try to use the XLogDir file if it exists.  This is so that we can
	 * make use of log segments that weren't yet transferred to the
	 * archive.
	 *
	 * Notice that we don't actually overwrite any files when we copy back
	 * from archive because the recoveryRestoreCommand may inadvertently
	 * restore inappropriate xlogs, or they may be corrupt, so we may wish
	 * to fallback to the segments remaining in current XLogDir later. The
	 * copy-from-archive filename is always the same, ensuring that we
	 * don't run out of disk space on long recoveries.
	 */
	snprintf(xlogpath, MAXPGPATH, "%s/%s", XLogDir, recovername);

	/*
	 * Make sure there is no existing file named recovername.
	 */
	if (stat(xlogpath, &stat_buf) != 0)
	{
		if (errno != ENOENT)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not stat file \"%s\": %m",
							xlogpath)));
	}
	else
	{
		if (unlink(xlogpath) != 0)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not remove file \"%s\": %m",
							xlogpath)));
	}

	/*
	 * construct the command to be executed
	 */
	dp = xlogRestoreCmd;
	endp = xlogRestoreCmd + MAXPGPATH - 1;
	*endp = '\0';

	for (sp = recoveryRestoreCommand; *sp; sp++)
	{
		if (*sp == '%')
		{
			switch (sp[1])
			{
				case 'p':
					/* %p: full path of target file */
					sp++;
					StrNCpy(dp, xlogpath, endp - dp);
					make_native_path(dp);
					dp += strlen(dp);
					break;
				case 'f':
					/* %f: filename of desired file */
					sp++;
					StrNCpy(dp, xlogfname, endp - dp);
					dp += strlen(dp);
					break;
				case '%':
					/* convert %% to a single % */
					sp++;
					if (dp < endp)
						*dp++ = *sp;
					break;
				default:
					/* otherwise treat the % as not special */
					if (dp < endp)
						*dp++ = *sp;
					break;
			}
		}
		else
		{
			if (dp < endp)
				*dp++ = *sp;
		}
	}
	*dp = '\0';

	ereport(DEBUG3,
			(errmsg_internal("executing restore command \"%s\"",
							 xlogRestoreCmd)));

	/*
	 * Copy xlog from archival storage to XLogDir
	 */
	rc = system(xlogRestoreCmd);
	if (rc == 0)
	{
		/*
		 * command apparently succeeded, but let's make sure the file is
		 * really there now and has the correct size.
		 *
		 * XXX I made wrong-size a fatal error to ensure the DBA would notice
		 * it, but is that too strong?	We could try to plow ahead with a
		 * local copy of the file ... but the problem is that there
		 * probably isn't one, and we'd incorrectly conclude we've reached
		 * the end of WAL and we're done recovering ...
		 */
		if (stat(xlogpath, &stat_buf) == 0)
		{
			if (expectedSize > 0 && stat_buf.st_size != expectedSize)
				ereport(FATAL,
						(errmsg("archive file \"%s\" has wrong size: %lu instead of %lu",
								xlogfname,
								(unsigned long) stat_buf.st_size,
								(unsigned long) expectedSize)));
			else
			{
				ereport(LOG,
						(errmsg("restored log file \"%s\" from archive",
								xlogfname)));
				strcpy(path, xlogpath);
				return true;
			}
		}
		else
		{
			/* stat failed */
			if (errno != ENOENT)
				ereport(FATAL,
						(errcode_for_file_access(),
						 errmsg("could not stat file \"%s\": %m",
								xlogpath)));
		}
	}

	/*
	 * remember, we rollforward UNTIL the restore fails so failure here is
	 * just part of the process... that makes it difficult to determine
	 * whether the restore failed because there isn't an archive to
	 * restore, or because the administrator has specified the restore
	 * program incorrectly.  We have to assume the former.
	 */
	ereport(DEBUG1,
		 (errmsg("could not restore file \"%s\" from archive: return code %d",
				 xlogfname, rc)));

	/*
	 * if an archived file is not available, there might still be a
	 * version of this file in XLogDir, so return that as the filename to
	 * open.
	 *
	 * In many recovery scenarios we expect this to fail also, but if so that
	 * just means we've reached the end of WAL.
	 */
	snprintf(path, MAXPGPATH, "%s/%s", XLogDir, xlogfname);
	return false;
}

/*
 * Preallocate log files beyond the specified log endpoint, according to
 * the XLOGfile user parameter.
 */
static int
PreallocXlogFiles(XLogRecPtr endptr)
{
	int			nsegsadded = 0;
	uint32		_logId;
	uint32		_logSeg;
	int			lf;
	bool		use_existent;

	XLByteToPrevSeg(endptr, _logId, _logSeg);
	if ((endptr.xrecoff - 1) % XLogSegSize >=
		(uint32) (0.75 * XLogSegSize))
	{
		NextLogSeg(_logId, _logSeg);
		use_existent = true;
		lf = XLogFileInit(_logId, _logSeg, &use_existent, true);
		close(lf);
		if (!use_existent)
			nsegsadded++;
	}
	return nsegsadded;
}

/*
 * Remove or move offline all log files older or equal to passed log/seg#
 *
 * endptr is current (or recent) end of xlog; this is used to determine
 * whether we want to recycle rather than delete no-longer-wanted log files.
 */
static void
MoveOfflineLogs(uint32 log, uint32 seg, XLogRecPtr endptr,
				int *nsegsremoved, int *nsegsrecycled)
{
	uint32		endlogId;
	uint32		endlogSeg;
	DIR		   *xldir;
	struct dirent *xlde;
	char		lastoff[MAXFNAMELEN];
	char		path[MAXPGPATH];

	*nsegsremoved = 0;
	*nsegsrecycled = 0;

	XLByteToPrevSeg(endptr, endlogId, endlogSeg);

	xldir = AllocateDir(XLogDir);
	if (xldir == NULL)
		ereport(ERROR,
				(errcode_for_file_access(),
			errmsg("could not open transaction log directory \"%s\": %m",
				   XLogDir)));

	XLogFileName(lastoff, ThisTimeLineID, log, seg);

	errno = 0;
	while ((xlde = readdir(xldir)) != NULL)
	{
		/*
		 * We ignore the timeline part of the XLOG segment identifiers in
		 * deciding whether a segment is still needed.	This ensures that
		 * we won't prematurely remove a segment from a parent timeline.
		 * We could probably be a little more proactive about removing
		 * segments of non-parent timelines, but that would be a whole lot
		 * more complicated.
		 *
		 * We use the alphanumeric sorting property of the filenames to
		 * decide which ones are earlier than the lastoff segment.
		 */
		if (strlen(xlde->d_name) == 24 &&
			strspn(xlde->d_name, "0123456789ABCDEF") == 24 &&
			strcmp(xlde->d_name + 8, lastoff + 8) <= 0)
		{
			bool		recycle;

			if (XLogArchivingActive())
				recycle = XLogArchiveIsDone(xlde->d_name);
			else
				recycle = true;

			if (recycle)
			{
				snprintf(path, MAXPGPATH, "%s/%s", XLogDir, xlde->d_name);

				/*
				 * Before deleting the file, see if it can be recycled as
				 * a future log segment.  We allow recycling segments up
				 * to XLOGfileslop segments beyond the current XLOG
				 * location.
				 */
				if (InstallXLogFileSegment(endlogId, endlogSeg, path,
										   true, XLOGfileslop,
										   true))
				{
					ereport(DEBUG2,
						  (errmsg("recycled transaction log file \"%s\"",
								  xlde->d_name)));
					(*nsegsrecycled)++;
				}
				else
				{
					/* No need for any more future segments... */
					ereport(DEBUG2,
						  (errmsg("removing transaction log file \"%s\"",
								  xlde->d_name)));
					unlink(path);
					(*nsegsremoved)++;
				}

				XLogArchiveCleanup(xlde->d_name);
			}
		}
		errno = 0;
	}
#ifdef WIN32

	/*
	 * This fix is in mingw cvs (runtime/mingwex/dirent.c rev 1.4), but
	 * not in released version
	 */
	if (GetLastError() == ERROR_NO_MORE_FILES)
		errno = 0;
#endif
	if (errno)
		ereport(ERROR,
				(errcode_for_file_access(),
			errmsg("could not read transaction log directory \"%s\": %m",
				   XLogDir)));
	FreeDir(xldir);
}

/*
 * Restore the backup blocks present in an XLOG record, if any.
 *
 * We assume all of the record has been read into memory at *record.
 *
 * Note: when a backup block is available in XLOG, we restore it
 * unconditionally, even if the page in the database appears newer.
 * This is to protect ourselves against database pages that were partially
 * or incorrectly written during a crash.  We assume that the XLOG data
 * must be good because it has passed a CRC check, while the database
 * page might not be.  This will force us to replay all subsequent
 * modifications of the page that appear in XLOG, rather than possibly
 * ignoring them as already applied, but that's not a huge drawback.
 */
static void
RestoreBkpBlocks(XLogRecord *record, XLogRecPtr lsn)
{
	Relation	reln;
	Buffer		buffer;
	Page		page;
	BkpBlock	bkpb;
	char	   *blk;
	int			i;

	blk = (char *) XLogRecGetData(record) + record->xl_len;
	for (i = 0; i < XLR_MAX_BKP_BLOCKS; i++)
	{
		if (!(record->xl_info & XLR_SET_BKP_BLOCK(i)))
			continue;

		memcpy((char *) &bkpb, blk, sizeof(BkpBlock));
		blk += sizeof(BkpBlock);

		reln = XLogOpenRelation(true, record->xl_rmid, bkpb.node);

		if (reln)
		{
			buffer = XLogReadBuffer(true, reln, bkpb.block);
			if (BufferIsValid(buffer))
			{
				page = (Page) BufferGetPage(buffer);
				memcpy((char *) page, blk, BLCKSZ);
				PageSetLSN(page, lsn);
				PageSetTLI(page, ThisTimeLineID);
				LockBuffer(buffer, BUFFER_LOCK_UNLOCK);
				WriteBuffer(buffer);
			}
		}

		blk += BLCKSZ;
	}
}

/*
 * CRC-check an XLOG record.  We do not believe the contents of an XLOG
 * record (other than to the minimal extent of computing the amount of
 * data to read in) until we've checked the CRCs.
 *
 * We assume all of the record has been read into memory at *record.
 */
static bool
RecordIsValid(XLogRecord *record, XLogRecPtr recptr, int emode)
{
	crc64		crc;
	crc64		cbuf;
	int			i;
	uint32		len = record->xl_len;
	char	   *blk;

	/* Check CRC of rmgr data and record header */
	INIT_CRC64(crc);
	COMP_CRC64(crc, XLogRecGetData(record), len);
	COMP_CRC64(crc, (char *) record + sizeof(crc64),
			   SizeOfXLogRecord - sizeof(crc64));
	FIN_CRC64(crc);

	if (!EQ_CRC64(record->xl_crc, crc))
	{
		ereport(emode,
				(errmsg("incorrect resource manager data checksum in record at %X/%X",
						recptr.xlogid, recptr.xrecoff)));
		return (false);
	}

	/* Check CRCs of backup blocks, if any */
	blk = (char *) XLogRecGetData(record) + len;
	for (i = 0; i < XLR_MAX_BKP_BLOCKS; i++)
	{
		if (!(record->xl_info & XLR_SET_BKP_BLOCK(i)))
			continue;

		INIT_CRC64(crc);
		COMP_CRC64(crc, blk + sizeof(BkpBlock), BLCKSZ);
		COMP_CRC64(crc, blk + sizeof(crc64),
				   sizeof(BkpBlock) - sizeof(crc64));
		FIN_CRC64(crc);
		memcpy((char *) &cbuf, blk, sizeof(crc64));		/* don't assume
														 * alignment */

		if (!EQ_CRC64(cbuf, crc))
		{
			ereport(emode,
					(errmsg("incorrect checksum of backup block %d in record at %X/%X",
							i + 1, recptr.xlogid, recptr.xrecoff)));
			return (false);
		}
		blk += sizeof(BkpBlock) + BLCKSZ;
	}

	return (true);
}

/*
 * Attempt to read an XLOG record.
 *
 * If RecPtr is not NULL, try to read a record at that position.  Otherwise
 * try to read a record just after the last one previously read.
 *
 * If no valid record is available, returns NULL, or fails if emode is PANIC.
 * (emode must be either PANIC or LOG.)
 *
 * The record is copied into readRecordBuf, so that on successful return,
 * the returned record pointer always points there.
 */
static XLogRecord *
ReadRecord(XLogRecPtr *RecPtr, int emode)
{
	XLogRecord *record;
	char	   *buffer;
	XLogRecPtr	tmpRecPtr = EndRecPtr;
	bool		randAccess = false;
	uint32		len,
				total_len;
	uint32		targetPageOff;
	uint32		targetRecOff;
	uint32		pageHeaderSize;
	unsigned	i;

	if (readBuf == NULL)
	{
		/*
		 * First time through, permanently allocate readBuf.  We do it
		 * this way, rather than just making a static array, for two
		 * reasons: (1) no need to waste the storage in most
		 * instantiations of the backend; (2) a static char array isn't
		 * guaranteed to have any particular alignment, whereas malloc()
		 * will provide MAXALIGN'd storage.
		 */
		readBuf = (char *) malloc(BLCKSZ);
		Assert(readBuf != NULL);
	}

	if (RecPtr == NULL)
	{
		RecPtr = &tmpRecPtr;
		/* fast case if next record is on same page */
		if (nextRecord != NULL)
		{
			record = nextRecord;
			goto got_record;
		}
		/* align old recptr to next page */
		if (tmpRecPtr.xrecoff % BLCKSZ != 0)
			tmpRecPtr.xrecoff += (BLCKSZ - tmpRecPtr.xrecoff % BLCKSZ);
		if (tmpRecPtr.xrecoff >= XLogFileSize)
		{
			(tmpRecPtr.xlogid)++;
			tmpRecPtr.xrecoff = 0;
		}
		/* We will account for page header size below */
	}
	else
	{
		if (!XRecOffIsValid(RecPtr->xrecoff))
			ereport(PANIC,
					(errmsg("invalid record offset at %X/%X",
							RecPtr->xlogid, RecPtr->xrecoff)));

		/*
		 * Since we are going to a random position in WAL, forget any
		 * prior state about what timeline we were in, and allow it to be
		 * any timeline in expectedTLIs.  We also set a flag to allow
		 * curFileTLI to go backwards (but we can't reset that variable
		 * right here, since we might not change files at all).
		 */
		lastPageTLI = 0;		/* see comment in ValidXLOGHeader */
		randAccess = true;		/* allow curFileTLI to go backwards too */
	}

	if (readFile >= 0 && !XLByteInSeg(*RecPtr, readId, readSeg))
	{
		close(readFile);
		readFile = -1;
	}
	XLByteToSeg(*RecPtr, readId, readSeg);
	if (readFile < 0)
	{
		/* Now it's okay to reset curFileTLI if random fetch */
		if (randAccess)
			curFileTLI = 0;

		readFile = XLogFileRead(readId, readSeg, emode);
		if (readFile < 0)
			goto next_record_is_invalid;
		readOff = (uint32) (-1);	/* force read to occur below */
	}

	targetPageOff = ((RecPtr->xrecoff % XLogSegSize) / BLCKSZ) * BLCKSZ;
	if (readOff != targetPageOff)
	{
		readOff = targetPageOff;
		if (lseek(readFile, (off_t) readOff, SEEK_SET) < 0)
		{
			ereport(emode,
					(errcode_for_file_access(),
					 errmsg("could not seek in log file %u, segment %u to offset %u: %m",
							readId, readSeg, readOff)));
			goto next_record_is_invalid;
		}
		if (read(readFile, readBuf, BLCKSZ) != BLCKSZ)
		{
			ereport(emode,
					(errcode_for_file_access(),
					 errmsg("could not read from log file %u, segment %u at offset %u: %m",
							readId, readSeg, readOff)));
			goto next_record_is_invalid;
		}
		if (!ValidXLOGHeader((XLogPageHeader) readBuf, emode))
			goto next_record_is_invalid;
	}
	pageHeaderSize = XLogPageHeaderSize((XLogPageHeader) readBuf);
	targetRecOff = RecPtr->xrecoff % BLCKSZ;
	if (targetRecOff == 0)
	{
		/*
		 * Can only get here in the continuing-from-prev-page case,
		 * because XRecOffIsValid eliminated the zero-page-offset case
		 * otherwise. Need to skip over the new page's header.
		 */
		tmpRecPtr.xrecoff += pageHeaderSize;
		targetRecOff = pageHeaderSize;
	}
	else if (targetRecOff < pageHeaderSize)
	{
		ereport(emode,
				(errmsg("invalid record offset at %X/%X",
						RecPtr->xlogid, RecPtr->xrecoff)));
		goto next_record_is_invalid;
	}
	if ((((XLogPageHeader) readBuf)->xlp_info & XLP_FIRST_IS_CONTRECORD) &&
		targetRecOff == pageHeaderSize)
	{
		ereport(emode,
				(errmsg("contrecord is requested by %X/%X",
						RecPtr->xlogid, RecPtr->xrecoff)));
		goto next_record_is_invalid;
	}
	record = (XLogRecord *) ((char *) readBuf + RecPtr->xrecoff % BLCKSZ);

got_record:;

	/*
	 * Currently, xl_len == 0 must be bad data, but that might not be true
	 * forever.  See note in XLogInsert.
	 */
	if (record->xl_len == 0)
	{
		ereport(emode,
				(errmsg("record with zero length at %X/%X",
						RecPtr->xlogid, RecPtr->xrecoff)));
		goto next_record_is_invalid;
	}
	if (record->xl_rmid > RM_MAX_ID)
	{
		ereport(emode,
				(errmsg("invalid resource manager ID %u at %X/%X",
					 record->xl_rmid, RecPtr->xlogid, RecPtr->xrecoff)));
		goto next_record_is_invalid;
	}

	/*
	 * Compute total length of record including any appended backup
	 * blocks.
	 */
	total_len = SizeOfXLogRecord + record->xl_len;
	for (i = 0; i < XLR_MAX_BKP_BLOCKS; i++)
	{
		if (!(record->xl_info & XLR_SET_BKP_BLOCK(i)))
			continue;
		total_len += sizeof(BkpBlock) + BLCKSZ;
	}

	/*
	 * Allocate or enlarge readRecordBuf as needed.  To avoid useless
	 * small increases, round its size to a multiple of BLCKSZ, and make
	 * sure it's at least 4*BLCKSZ to start with.  (That is enough for all
	 * "normal" records, but very large commit or abort records might need
	 * more space.)
	 */
	if (total_len > readRecordBufSize)
	{
		uint32		newSize = total_len;

		newSize += BLCKSZ - (newSize % BLCKSZ);
		newSize = Max(newSize, 4 * BLCKSZ);
		if (readRecordBuf)
			free(readRecordBuf);
		readRecordBuf = (char *) malloc(newSize);
		if (!readRecordBuf)
		{
			readRecordBufSize = 0;
			/* We treat this as a "bogus data" condition */
			ereport(emode,
					(errmsg("record length %u at %X/%X too long",
							total_len, RecPtr->xlogid, RecPtr->xrecoff)));
			goto next_record_is_invalid;
		}
		readRecordBufSize = newSize;
	}

	buffer = readRecordBuf;
	nextRecord = NULL;
	len = BLCKSZ - RecPtr->xrecoff % BLCKSZ;
	if (total_len > len)
	{
		/* Need to reassemble record */
		XLogContRecord *contrecord;
		uint32		gotlen = len;

		memcpy(buffer, record, len);
		record = (XLogRecord *) buffer;
		buffer += len;
		for (;;)
		{
			readOff += BLCKSZ;
			if (readOff >= XLogSegSize)
			{
				close(readFile);
				readFile = -1;
				NextLogSeg(readId, readSeg);
				readFile = XLogFileRead(readId, readSeg, emode);
				if (readFile < 0)
					goto next_record_is_invalid;
				readOff = 0;
			}
			if (read(readFile, readBuf, BLCKSZ) != BLCKSZ)
			{
				ereport(emode,
						(errcode_for_file_access(),
						 errmsg("could not read from log file %u, segment %u, offset %u: %m",
								readId, readSeg, readOff)));
				goto next_record_is_invalid;
			}
			if (!ValidXLOGHeader((XLogPageHeader) readBuf, emode))
				goto next_record_is_invalid;
			if (!(((XLogPageHeader) readBuf)->xlp_info & XLP_FIRST_IS_CONTRECORD))
			{
				ereport(emode,
						(errmsg("there is no contrecord flag in log file %u, segment %u, offset %u",
								readId, readSeg, readOff)));
				goto next_record_is_invalid;
			}
			pageHeaderSize = XLogPageHeaderSize((XLogPageHeader) readBuf);
			contrecord = (XLogContRecord *) ((char *) readBuf + pageHeaderSize);
			if (contrecord->xl_rem_len == 0 ||
				total_len != (contrecord->xl_rem_len + gotlen))
			{
				ereport(emode,
						(errmsg("invalid contrecord length %u in log file %u, segment %u, offset %u",
								contrecord->xl_rem_len,
								readId, readSeg, readOff)));
				goto next_record_is_invalid;
			}
			len = BLCKSZ - pageHeaderSize - SizeOfXLogContRecord;
			if (contrecord->xl_rem_len > len)
			{
				memcpy(buffer, (char *) contrecord + SizeOfXLogContRecord, len);
				gotlen += len;
				buffer += len;
				continue;
			}
			memcpy(buffer, (char *) contrecord + SizeOfXLogContRecord,
				   contrecord->xl_rem_len);
			break;
		}
		if (!RecordIsValid(record, *RecPtr, emode))
			goto next_record_is_invalid;
		pageHeaderSize = XLogPageHeaderSize((XLogPageHeader) readBuf);
		if (BLCKSZ - SizeOfXLogRecord >= pageHeaderSize +
			SizeOfXLogContRecord + MAXALIGN(contrecord->xl_rem_len))
		{
			nextRecord = (XLogRecord *) ((char *) contrecord +
				SizeOfXLogContRecord + MAXALIGN(contrecord->xl_rem_len));
		}
		EndRecPtr.xlogid = readId;
		EndRecPtr.xrecoff = readSeg * XLogSegSize + readOff +
			pageHeaderSize + SizeOfXLogContRecord +
			MAXALIGN(contrecord->xl_rem_len);
		ReadRecPtr = *RecPtr;
		return record;
	}

	/* Record does not cross a page boundary */
	if (!RecordIsValid(record, *RecPtr, emode))
		goto next_record_is_invalid;
	if (BLCKSZ - SizeOfXLogRecord >= RecPtr->xrecoff % BLCKSZ +
		MAXALIGN(total_len))
		nextRecord = (XLogRecord *) ((char *) record + MAXALIGN(total_len));
	EndRecPtr.xlogid = RecPtr->xlogid;
	EndRecPtr.xrecoff = RecPtr->xrecoff + MAXALIGN(total_len);
	ReadRecPtr = *RecPtr;
	memcpy(buffer, record, total_len);
	return (XLogRecord *) buffer;

next_record_is_invalid:;
	close(readFile);
	readFile = -1;
	nextRecord = NULL;
	return NULL;
}

/*
 * Check whether the xlog header of a page just read in looks valid.
 *
 * This is just a convenience subroutine to avoid duplicated code in
 * ReadRecord.	It's not intended for use from anywhere else.
 */
static bool
ValidXLOGHeader(XLogPageHeader hdr, int emode)
{
	XLogRecPtr	recaddr;

	if (hdr->xlp_magic != XLOG_PAGE_MAGIC)
	{
		ereport(emode,
				(errmsg("invalid magic number %04X in log file %u, segment %u, offset %u",
						hdr->xlp_magic, readId, readSeg, readOff)));
		return false;
	}
	if ((hdr->xlp_info & ~XLP_ALL_FLAGS) != 0)
	{
		ereport(emode,
				(errmsg("invalid info bits %04X in log file %u, segment %u, offset %u",
						hdr->xlp_info, readId, readSeg, readOff)));
		return false;
	}
	if (hdr->xlp_info & XLP_LONG_HEADER)
	{
		XLogLongPageHeader longhdr = (XLogLongPageHeader) hdr;

		if (longhdr->xlp_sysid != ControlFile->system_identifier)
		{
			char		fhdrident_str[32];
			char		sysident_str[32];

			/*
			 * Format sysids separately to keep platform-dependent format
			 * code out of the translatable message string.
			 */
			snprintf(fhdrident_str, sizeof(fhdrident_str), UINT64_FORMAT,
					 longhdr->xlp_sysid);
			snprintf(sysident_str, sizeof(sysident_str), UINT64_FORMAT,
					 ControlFile->system_identifier);
			ereport(emode,
					(errmsg("WAL file is from different system"),
				errdetail("WAL file SYSID is %s, pg_control SYSID is %s",
						  fhdrident_str, sysident_str)));
			return false;
		}
		if (longhdr->xlp_seg_size != XLogSegSize)
		{
			ereport(emode,
					(errmsg("WAL file is from different system"),
				  errdetail("Incorrect XLOG_SEG_SIZE in page header.")));
			return false;
		}
	}
	recaddr.xlogid = readId;
	recaddr.xrecoff = readSeg * XLogSegSize + readOff;
	if (!XLByteEQ(hdr->xlp_pageaddr, recaddr))
	{
		ereport(emode,
				(errmsg("unexpected pageaddr %X/%X in log file %u, segment %u, offset %u",
					 hdr->xlp_pageaddr.xlogid, hdr->xlp_pageaddr.xrecoff,
						readId, readSeg, readOff)));
		return false;
	}

	/*
	 * Check page TLI is one of the expected values.
	 */
	if (!list_member_int(expectedTLIs, (int) hdr->xlp_tli))
	{
		ereport(emode,
				(errmsg("unexpected timeline ID %u in log file %u, segment %u, offset %u",
						hdr->xlp_tli,
						readId, readSeg, readOff)));
		return false;
	}

	/*
	 * Since child timelines are always assigned a TLI greater than their
	 * immediate parent's TLI, we should never see TLI go backwards across
	 * successive pages of a consistent WAL sequence.
	 *
	 * Of course this check should only be applied when advancing
	 * sequentially across pages; therefore ReadRecord resets lastPageTLI
	 * to zero when going to a random page.
	 */
	if (hdr->xlp_tli < lastPageTLI)
	{
		ereport(emode,
				(errmsg("out-of-sequence timeline ID %u (after %u) in log file %u, segment %u, offset %u",
						hdr->xlp_tli, lastPageTLI,
						readId, readSeg, readOff)));
		return false;
	}
	lastPageTLI = hdr->xlp_tli;
	return true;
}

/*
 * Try to read a timeline's history file.
 *
 * If successful, return the list of component TLIs (the given TLI followed by
 * its ancestor TLIs).	If we can't find the history file, assume that the
 * timeline has no parents, and return a list of just the specified timeline
 * ID.
 */
static List *
readTimeLineHistory(TimeLineID targetTLI)
{
	List	   *result;
	char		path[MAXPGPATH];
	char		histfname[MAXFNAMELEN];
	char		fline[MAXPGPATH];
	FILE	   *fd;

	if (InArchiveRecovery)
	{
		TLHistoryFileName(histfname, targetTLI);
		RestoreArchivedFile(path, histfname, "RECOVERYHISTORY", 0);
	}
	else
		TLHistoryFilePath(path, targetTLI);

	fd = AllocateFile(path, "r");
	if (fd == NULL)
	{
		if (errno != ENOENT)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not open file \"%s\": %m", path)));
		/* Not there, so assume no parents */
		return list_make1_int((int) targetTLI);
	}

	result = NIL;

	/*
	 * Parse the file...
	 */
	while (fgets(fline, MAXPGPATH, fd) != NULL)
	{
		/* skip leading whitespace and check for # comment */
		char	   *ptr;
		char	   *endptr;
		TimeLineID	tli;

		for (ptr = fline; *ptr; ptr++)
		{
			if (!isspace((unsigned char) *ptr))
				break;
		}
		if (*ptr == '\0' || *ptr == '#')
			continue;

		/* expect a numeric timeline ID as first field of line */
		tli = (TimeLineID) strtoul(ptr, &endptr, 0);
		if (endptr == ptr)
			ereport(FATAL,
					(errmsg("syntax error in history file: %s", fline),
					 errhint("Expected a numeric timeline ID.")));

		if (result &&
			tli <= (TimeLineID) linitial_int(result))
			ereport(FATAL,
					(errmsg("invalid data in history file: %s", fline),
			   errhint("Timeline IDs must be in increasing sequence.")));

		/* Build list with newest item first */
		result = lcons_int((int) tli, result);

		/* we ignore the remainder of each line */
	}

	FreeFile(fd);

	if (result &&
		targetTLI <= (TimeLineID) linitial_int(result))
		ereport(FATAL,
				(errmsg("invalid data in history file \"%s\"", path),
		errhint("Timeline IDs must be less than child timeline's ID.")));

	result = lcons_int((int) targetTLI, result);

	ereport(DEBUG3,
			(errmsg_internal("history of timeline %u is %s",
							 targetTLI, nodeToString(result))));

	return result;
}

/*
 * Probe whether a timeline history file exists for the given timeline ID
 */
static bool
existsTimeLineHistory(TimeLineID probeTLI)
{
	char		path[MAXPGPATH];
	char		histfname[MAXFNAMELEN];
	FILE	   *fd;

	if (InArchiveRecovery)
	{
		TLHistoryFileName(histfname, probeTLI);
		RestoreArchivedFile(path, histfname, "RECOVERYHISTORY", 0);
	}
	else
		TLHistoryFilePath(path, probeTLI);

	fd = AllocateFile(path, "r");
	if (fd != NULL)
	{
		FreeFile(fd);
		return true;
	}
	else
	{
		if (errno != ENOENT)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not open file \"%s\": %m", path)));
		return false;
	}
}

/*
 * Find the newest existing timeline, assuming that startTLI exists.
 *
 * Note: while this is somewhat heuristic, it does positively guarantee
 * that (result + 1) is not a known timeline, and therefore it should
 * be safe to assign that ID to a new timeline.
 */
static TimeLineID
findNewestTimeLine(TimeLineID startTLI)
{
	TimeLineID	newestTLI;
	TimeLineID	probeTLI;

	/*
	 * The algorithm is just to probe for the existence of timeline
	 * history files.  XXX is it useful to allow gaps in the sequence?
	 */
	newestTLI = startTLI;

	for (probeTLI = startTLI + 1;; probeTLI++)
	{
		if (existsTimeLineHistory(probeTLI))
		{
			newestTLI = probeTLI;		/* probeTLI exists */
		}
		else
		{
			/* doesn't exist, assume we're done */
			break;
		}
	}

	return newestTLI;
}

/*
 * Create a new timeline history file.
 *
 *	newTLI: ID of the new timeline
 *	parentTLI: ID of its immediate parent
 *	endTLI et al: ID of the last used WAL file, for annotation purposes
 *
 * Currently this is only used during recovery, and so there are no locking
 * considerations.	But we should be just as tense as XLogFileInit to avoid
 * emplacing a bogus file.
 */
static void
writeTimeLineHistory(TimeLineID newTLI, TimeLineID parentTLI,
					 TimeLineID endTLI, uint32 endLogId, uint32 endLogSeg)
{
	char		path[MAXPGPATH];
	char		tmppath[MAXPGPATH];
	char		histfname[MAXFNAMELEN];
	char		xlogfname[MAXFNAMELEN];
	char		buffer[BLCKSZ];
	int			srcfd;
	int			fd;
	int			nbytes;

	Assert(newTLI > parentTLI); /* else bad selection of newTLI */

	/*
	 * Write into a temp file name.
	 */
	snprintf(tmppath, MAXPGPATH, "%s/xlogtemp.%d", XLogDir, (int)getpid());

	unlink(tmppath);

	/* do not use XLOG_SYNC_BIT here --- want to fsync only at end of fill */
	fd = BasicOpenFile(tmppath, O_RDWR | O_CREAT | O_EXCL,
					   S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not create file \"%s\": %m", tmppath)));

	/*
	 * If a history file exists for the parent, copy it verbatim
	 */
	if (InArchiveRecovery)
	{
		TLHistoryFileName(histfname, parentTLI);
		RestoreArchivedFile(path, histfname, "RECOVERYHISTORY", 0);
	}
	else
		TLHistoryFilePath(path, parentTLI);

	srcfd = BasicOpenFile(path, O_RDONLY, 0);
	if (srcfd < 0)
	{
		if (errno != ENOENT)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not open file \"%s\": %m", path)));
		/* Not there, so assume parent has no parents */
	}
	else
	{
		for (;;)
		{
			errno = 0;
			nbytes = (int) read(srcfd, buffer, sizeof(buffer));
			if (nbytes < 0 || errno != 0)
				ereport(PANIC,
						(errcode_for_file_access(),
						 errmsg("could not read file \"%s\": %m", path)));
			if (nbytes == 0)
				break;
			errno = 0;
			if ((int) write(fd, buffer, nbytes) != nbytes)
			{
				int			save_errno = errno;

				/*
				 * If we fail to make the file, delete it to release disk
				 * space
				 */
				unlink(tmppath);

				/*
				 * if write didn't set errno, assume problem is no disk
				 * space
				 */
				errno = save_errno ? save_errno : ENOSPC;

				ereport(PANIC,
						(errcode_for_file_access(),
				 errmsg("could not write to file \"%s\": %m", tmppath)));
			}
		}
		close(srcfd);
	}

	/*
	 * Append one line with the details of this timeline split.
	 *
	 * If we did have a parent file, insert an extra newline just in case the
	 * parent file failed to end with one.
	 */
	XLogFileName(xlogfname, endTLI, endLogId, endLogSeg);

	snprintf(buffer, sizeof(buffer),
			 "%s%u\t%s\t%s transaction %u at %s\n",
			 (srcfd < 0) ? "" : "\n",
			 parentTLI,
			 xlogfname,
			 recoveryStopAfter ? "after" : "before",
			 recoveryStopXid,
			 str_time(recoveryStopTime));

	nbytes = strlen(buffer);
	errno = 0;
	if ((int) write(fd, buffer, nbytes) != nbytes)
	{
		int			save_errno = errno;

		/*
		 * If we fail to make the file, delete it to release disk space
		 */
		unlink(tmppath);
		/* if write didn't set errno, assume problem is no disk space */
		errno = save_errno ? save_errno : ENOSPC;

		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not write to file \"%s\": %m", tmppath)));
	}

	if (pg_fsync(fd) != 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not fsync file \"%s\": %m", tmppath)));

	if (close(fd))
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not close file \"%s\": %m", tmppath)));


	/*
	 * Now move the completed history file into place with its final name.
	 */
	TLHistoryFilePath(path, newTLI);

	/*
	 * Prefer link() to rename() here just to be really sure that we don't
	 * overwrite an existing logfile.  However, there shouldn't be one, so
	 * rename() is an acceptable substitute except for the truly paranoid.
	 */
#if HAVE_WORKING_LINK
	if (link(tmppath, path) < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not link file \"%s\" to \"%s\": %m",
						tmppath, path)));
	unlink(tmppath);
#else
	if (rename(tmppath, path) < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not rename file \"%s\" to \"%s\": %m",
						tmppath, path)));
#endif

	/* The history file can be archived immediately. */
	TLHistoryFileName(histfname, newTLI);
	XLogArchiveNotify(histfname);
}

/*
 * I/O routines for pg_control
 *
 * *ControlFile is a buffer in shared memory that holds an image of the
 * contents of pg_control.	WriteControlFile() initializes pg_control
 * given a preloaded buffer, ReadControlFile() loads the buffer from
 * the pg_control file (during postmaster or standalone-backend startup),
 * and UpdateControlFile() rewrites pg_control after we modify xlog state.
 *
 * For simplicity, WriteControlFile() initializes the fields of pg_control
 * that are related to checking backend/database compatibility, and
 * ReadControlFile() verifies they are correct.  We could split out the
 * I/O and compatibility-check functions, but there seems no need currently.
 */

void
XLOGPathInit(void)
{
	/* Init XLOG file paths */
	snprintf(XLogDir, MAXPGPATH, "%s/pg_xlog", DataDir);
	snprintf(ControlFilePath, MAXPGPATH, "%s/global/pg_control", DataDir);
}

static void
WriteControlFile(void)
{
	int			fd;
	char		buffer[BLCKSZ]; /* need not be aligned */
	char	   *localeptr;

	/*
	 * Initialize version and compatibility-check fields
	 */
	ControlFile->pg_control_version = PG_CONTROL_VERSION;
	ControlFile->catalog_version_no = CATALOG_VERSION_NO;
	ControlFile->blcksz = BLCKSZ;
	ControlFile->relseg_size = RELSEG_SIZE;
	ControlFile->xlog_seg_size = XLOG_SEG_SIZE;

	ControlFile->nameDataLen = NAMEDATALEN;
	ControlFile->funcMaxArgs = FUNC_MAX_ARGS;

#ifdef HAVE_INT64_TIMESTAMP
	ControlFile->enableIntTimes = TRUE;
#else
	ControlFile->enableIntTimes = FALSE;
#endif

	ControlFile->localeBuflen = LOCALE_NAME_BUFLEN;
	localeptr = setlocale(LC_COLLATE, NULL);
	if (!localeptr)
		ereport(PANIC,
				(errmsg("invalid LC_COLLATE setting")));
	StrNCpy(ControlFile->lc_collate, localeptr, LOCALE_NAME_BUFLEN);
	localeptr = setlocale(LC_CTYPE, NULL);
	if (!localeptr)
		ereport(PANIC,
				(errmsg("invalid LC_CTYPE setting")));
	StrNCpy(ControlFile->lc_ctype, localeptr, LOCALE_NAME_BUFLEN);

	/* Contents are protected with a CRC */
	INIT_CRC64(ControlFile->crc);
	COMP_CRC64(ControlFile->crc,
			   (char *) ControlFile + sizeof(crc64),
			   sizeof(ControlFileData) - sizeof(crc64));
	FIN_CRC64(ControlFile->crc);

	/*
	 * We write out BLCKSZ bytes into pg_control, zero-padding the excess
	 * over sizeof(ControlFileData).  This reduces the odds of
	 * premature-EOF errors when reading pg_control.  We'll still fail
	 * when we check the contents of the file, but hopefully with a more
	 * specific error than "couldn't read pg_control".
	 */
	if (sizeof(ControlFileData) > BLCKSZ)
		ereport(PANIC,
				(errmsg("sizeof(ControlFileData) is larger than BLCKSZ; fix either one")));

	memset(buffer, 0, BLCKSZ);
	memcpy(buffer, ControlFile, sizeof(ControlFileData));

	fd = BasicOpenFile(ControlFilePath, O_RDWR | O_CREAT | O_EXCL | PG_BINARY,
					   S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not create control file \"%s\": %m",
						ControlFilePath)));

	errno = 0;
	if (write(fd, buffer, BLCKSZ) != BLCKSZ)
	{
		/* if write didn't set errno, assume problem is no disk space */
		if (errno == 0)
			errno = ENOSPC;
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not write to control file: %m")));
	}

	if (pg_fsync(fd) != 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not fsync control file: %m")));

	if (close(fd))
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not close control file: %m")));
}

static void
ReadControlFile(void)
{
	crc64		crc;
	int			fd;

	/*
	 * Read data...
	 */
	fd = BasicOpenFile(ControlFilePath, O_RDWR | PG_BINARY, S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not open control file \"%s\": %m",
						ControlFilePath)));

	if (read(fd, ControlFile, sizeof(ControlFileData)) != sizeof(ControlFileData))
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not read from control file: %m")));

	close(fd);

	/*
	 * Check for expected pg_control format version.  If this is wrong,
	 * the CRC check will likely fail because we'll be checking the wrong
	 * number of bytes.  Complaining about wrong version will probably be
	 * more enlightening than complaining about wrong CRC.
	 */
	if (ControlFile->pg_control_version != PG_CONTROL_VERSION)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with PG_CONTROL_VERSION %d,"
			  " but the server was compiled with PG_CONTROL_VERSION %d.",
					ControlFile->pg_control_version, PG_CONTROL_VERSION),
				 errhint("It looks like you need to initdb.")));
	/* Now check the CRC. */
	INIT_CRC64(crc);
	COMP_CRC64(crc,
			   (char *) ControlFile + sizeof(crc64),
			   sizeof(ControlFileData) - sizeof(crc64));
	FIN_CRC64(crc);

	if (!EQ_CRC64(crc, ControlFile->crc))
		ereport(FATAL,
				(errmsg("incorrect checksum in control file")));

	/*
	 * Do compatibility checking immediately.  We do this here for 2
	 * reasons:
	 *
	 * (1) if the database isn't compatible with the backend executable, we
	 * want to abort before we can possibly do any damage;
	 *
	 * (2) this code is executed in the postmaster, so the setlocale() will
	 * propagate to forked backends, which aren't going to read this file
	 * for themselves.	(These locale settings are considered critical
	 * compatibility items because they can affect sort order of indexes.)
	 */
	if (ControlFile->catalog_version_no != CATALOG_VERSION_NO)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with CATALOG_VERSION_NO %d,"
			  " but the server was compiled with CATALOG_VERSION_NO %d.",
					ControlFile->catalog_version_no, CATALOG_VERSION_NO),
				 errhint("It looks like you need to initdb.")));
	if (ControlFile->blcksz != BLCKSZ)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
		 errdetail("The database cluster was initialized with BLCKSZ %d,"
				   " but the server was compiled with BLCKSZ %d.",
				   ControlFile->blcksz, BLCKSZ),
			 errhint("It looks like you need to recompile or initdb.")));
	if (ControlFile->relseg_size != RELSEG_SIZE)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with RELSEG_SIZE %d,"
					 " but the server was compiled with RELSEG_SIZE %d.",
						   ControlFile->relseg_size, RELSEG_SIZE),
			 errhint("It looks like you need to recompile or initdb.")));
	if (ControlFile->xlog_seg_size != XLOG_SEG_SIZE)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with XLOG_SEG_SIZE %d,"
				   " but the server was compiled with XLOG_SEG_SIZE %d.",
						   ControlFile->xlog_seg_size, XLOG_SEG_SIZE),
			 errhint("It looks like you need to recompile or initdb.")));
	if (ControlFile->nameDataLen != NAMEDATALEN)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with NAMEDATALEN %d,"
					 " but the server was compiled with NAMEDATALEN %d.",
						   ControlFile->nameDataLen, NAMEDATALEN),
			 errhint("It looks like you need to recompile or initdb.")));
	if (ControlFile->funcMaxArgs != FUNC_MAX_ARGS)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with FUNC_MAX_ARGS %d,"
				   " but the server was compiled with FUNC_MAX_ARGS %d.",
						   ControlFile->funcMaxArgs, FUNC_MAX_ARGS),
			 errhint("It looks like you need to recompile or initdb.")));

#ifdef HAVE_INT64_TIMESTAMP
	if (ControlFile->enableIntTimes != TRUE)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized without HAVE_INT64_TIMESTAMP"
			  " but the server was compiled with HAVE_INT64_TIMESTAMP."),
			 errhint("It looks like you need to recompile or initdb.")));
#else
	if (ControlFile->enableIntTimes != FALSE)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with HAVE_INT64_TIMESTAMP"
		   " but the server was compiled without HAVE_INT64_TIMESTAMP."),
			 errhint("It looks like you need to recompile or initdb.")));
#endif

	if (ControlFile->localeBuflen != LOCALE_NAME_BUFLEN)
		ereport(FATAL,
				(errmsg("database files are incompatible with server"),
				 errdetail("The database cluster was initialized with LOCALE_NAME_BUFLEN %d,"
			  " but the server was compiled with LOCALE_NAME_BUFLEN %d.",
						   ControlFile->localeBuflen, LOCALE_NAME_BUFLEN),
			 errhint("It looks like you need to recompile or initdb.")));
	if (setlocale(LC_COLLATE, ControlFile->lc_collate) == NULL)
		ereport(FATAL,
		(errmsg("database files are incompatible with operating system"),
		 errdetail("The database cluster was initialized with LC_COLLATE \"%s\","
				   " which is not recognized by setlocale().",
				   ControlFile->lc_collate),
		 errhint("It looks like you need to initdb or install locale support.")));
	if (setlocale(LC_CTYPE, ControlFile->lc_ctype) == NULL)
		ereport(FATAL,
		(errmsg("database files are incompatible with operating system"),
		 errdetail("The database cluster was initialized with LC_CTYPE \"%s\","
				   " which is not recognized by setlocale().",
				   ControlFile->lc_ctype),
		 errhint("It looks like you need to initdb or install locale support.")));

	/* Make the fixed locale settings visible as GUC variables, too */
	SetConfigOption("lc_collate", ControlFile->lc_collate,
					PGC_INTERNAL, PGC_S_OVERRIDE);
	SetConfigOption("lc_ctype", ControlFile->lc_ctype,
					PGC_INTERNAL, PGC_S_OVERRIDE);
}

void
UpdateControlFile(void)
{
	int			fd;

	INIT_CRC64(ControlFile->crc);
	COMP_CRC64(ControlFile->crc,
			   (char *) ControlFile + sizeof(crc64),
			   sizeof(ControlFileData) - sizeof(crc64));
	FIN_CRC64(ControlFile->crc);

	fd = BasicOpenFile(ControlFilePath, O_RDWR | PG_BINARY, S_IRUSR | S_IWUSR);
	if (fd < 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not open control file \"%s\": %m",
						ControlFilePath)));

	errno = 0;
	if (write(fd, ControlFile, sizeof(ControlFileData)) != sizeof(ControlFileData))
	{
		/* if write didn't set errno, assume problem is no disk space */
		if (errno == 0)
			errno = ENOSPC;
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not write to control file: %m")));
	}

	if (pg_fsync(fd) != 0)
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not fsync control file: %m")));

	if (close(fd))
		ereport(PANIC,
				(errcode_for_file_access(),
				 errmsg("could not close control file: %m")));
}

/*
 * Initialization of shared memory for XLOG
 */

int
XLOGShmemSize(void)
{
	if (XLOGbuffers < MinXLOGbuffers)
		XLOGbuffers = MinXLOGbuffers;

	return MAXALIGN(sizeof(XLogCtlData) + sizeof(XLogRecPtr) * XLOGbuffers)
		+ BLCKSZ * XLOGbuffers +
		MAXALIGN(sizeof(ControlFileData));
}

void
XLOGShmemInit(void)
{
	bool		foundXLog,
				foundCFile;

	/* this must agree with space requested by XLOGShmemSize() */
	if (XLOGbuffers < MinXLOGbuffers)
		XLOGbuffers = MinXLOGbuffers;

	XLogCtl = (XLogCtlData *)
		ShmemInitStruct("XLOG Ctl",
						MAXALIGN(sizeof(XLogCtlData) +
								 sizeof(XLogRecPtr) * XLOGbuffers)
						+ BLCKSZ * XLOGbuffers,
						&foundXLog);
	ControlFile = (ControlFileData *)
		ShmemInitStruct("Control File", sizeof(ControlFileData), &foundCFile);

	if (foundXLog || foundCFile)
	{
		/* both should be present or neither */
		Assert(foundXLog && foundCFile);
		return;
	}

	memset(XLogCtl, 0, sizeof(XLogCtlData));

	/*
	 * Since XLogCtlData contains XLogRecPtr fields, its sizeof should be
	 * a multiple of the alignment for same, so no extra alignment padding
	 * is needed here.
	 */
	XLogCtl->xlblocks = (XLogRecPtr *)
		(((char *) XLogCtl) + sizeof(XLogCtlData));
	memset(XLogCtl->xlblocks, 0, sizeof(XLogRecPtr) * XLOGbuffers);

	/*
	 * Here, on the other hand, we must MAXALIGN to ensure the page
	 * buffers have worst-case alignment.
	 */
	XLogCtl->pages =
		((char *) XLogCtl) + MAXALIGN(sizeof(XLogCtlData) +
									  sizeof(XLogRecPtr) * XLOGbuffers);
	memset(XLogCtl->pages, 0, BLCKSZ * XLOGbuffers);

	/*
	 * Do basic initialization of XLogCtl shared data. (StartupXLOG will
	 * fill in additional info.)
	 */
	XLogCtl->XLogCacheByte = BLCKSZ * XLOGbuffers;
	XLogCtl->XLogCacheBlck = XLOGbuffers - 1;
	XLogCtl->Insert.currpage = (XLogPageHeader) (XLogCtl->pages);
	SpinLockInit(&XLogCtl->info_lck);

	/*
	 * If we are not in bootstrap mode, pg_control should already exist.
	 * Read and validate it immediately (see comments in ReadControlFile()
	 * for the reasons why).
	 */
	if (!IsBootstrapProcessingMode())
		ReadControlFile();
}

/*
 * This func must be called ONCE on system install.  It creates pg_control
 * and the initial XLOG segment.
 */
void
BootStrapXLOG(void)
{
	CheckPoint	checkPoint;
	char	   *buffer;
	XLogPageHeader page;
	XLogLongPageHeader longpage;
	XLogRecord *record;
	bool		use_existent;
	uint64		sysidentifier;
	struct timeval tv;
	crc64		crc;

	/*
	 * Select a hopefully-unique system identifier code for this
	 * installation. We use the result of gettimeofday(), including the
	 * fractional seconds field, as being about as unique as we can easily
	 * get.  (Think not to use random(), since it hasn't been seeded and
	 * there's no portable way to seed it other than the system clock
	 * value...)  The upper half of the uint64 value is just the tv_sec
	 * part, while the lower half is the XOR of tv_sec and tv_usec.  This
	 * is to ensure that we don't lose uniqueness unnecessarily if
	 * "uint64" is really only 32 bits wide.  A person knowing this
	 * encoding can determine the initialization time of the installation,
	 * which could perhaps be useful sometimes.
	 */
	gettimeofday(&tv, NULL);
	sysidentifier = ((uint64) tv.tv_sec) << 32;
	sysidentifier |= (uint32) (tv.tv_sec | tv.tv_usec);

	/* First timeline ID is always 1 */
	ThisTimeLineID = 1;

	/* Use malloc() to ensure buffer is MAXALIGNED */
	buffer = (char *) malloc(BLCKSZ);
	page = (XLogPageHeader) buffer;
	memset(buffer, 0, BLCKSZ);

	/* Set up information for the initial checkpoint record */
	checkPoint.redo.xlogid = 0;
	checkPoint.redo.xrecoff = SizeOfXLogLongPHD;
	checkPoint.undo = checkPoint.redo;
	checkPoint.ThisTimeLineID = ThisTimeLineID;
	checkPoint.nextXid = FirstNormalTransactionId;
	checkPoint.nextOid = BootstrapObjectIdData;
	checkPoint.time = time(NULL);

	ShmemVariableCache->nextXid = checkPoint.nextXid;
	ShmemVariableCache->nextOid = checkPoint.nextOid;
	ShmemVariableCache->oidCount = 0;

	/* Set up the XLOG page header */
	page->xlp_magic = XLOG_PAGE_MAGIC;
	page->xlp_info = XLP_LONG_HEADER;
	page->xlp_tli = ThisTimeLineID;
	page->xlp_pageaddr.xlogid = 0;
	page->xlp_pageaddr.xrecoff = 0;
	longpage = (XLogLongPageHeader) page;
	longpage->xlp_sysid = sysidentifier;
	longpage->xlp_seg_size = XLogSegSize;

	/* Insert the initial checkpoint record */
	record = (XLogRecord *) ((char *) page + SizeOfXLogLongPHD);
	record->xl_prev.xlogid = 0;
	record->xl_prev.xrecoff = 0;
	record->xl_xid = InvalidTransactionId;
	record->xl_len = sizeof(checkPoint);
	record->xl_info = XLOG_CHECKPOINT_SHUTDOWN;
	record->xl_rmid = RM_XLOG_ID;
	memcpy(XLogRecGetData(record), &checkPoint, sizeof(checkPoint));

	INIT_CRC64(crc);
	COMP_CRC64(crc, &checkPoint, sizeof(checkPoint));
	COMP_CRC64(crc, (char *) record + sizeof(crc64),
			   SizeOfXLogRecord - sizeof(crc64));
	FIN_CRC64(crc);
	record->xl_crc = crc;

	/* Create first XLOG segment file */
	use_existent = false;
	openLogFile = XLogFileInit(0, 0, &use_existent, false);

	/* Write the first page with the initial record */
	errno = 0;
	if (write(openLogFile, buffer, BLCKSZ) != BLCKSZ)
	{
		/* if write didn't set errno, assume problem is no disk space */
		if (errno == 0)
			errno = ENOSPC;
		ereport(PANIC,
				(errcode_for_file_access(),
		  errmsg("could not write bootstrap transaction log file: %m")));
	}

	if (pg_fsync(openLogFile) != 0)
		ereport(PANIC,
				(errcode_for_file_access(),
		  errmsg("could not fsync bootstrap transaction log file: %m")));

	if (close(openLogFile))
		ereport(PANIC,
				(errcode_for_file_access(),
		  errmsg("could not close bootstrap transaction log file: %m")));

	openLogFile = -1;

	/* Now create pg_control */

	memset(ControlFile, 0, sizeof(ControlFileData));
	/* Initialize pg_control status fields */
	ControlFile->system_identifier = sysidentifier;
	ControlFile->state = DB_SHUTDOWNED;
	ControlFile->time = checkPoint.time;
	ControlFile->logId = 0;
	ControlFile->logSeg = 1;
	ControlFile->checkPoint = checkPoint.redo;
	ControlFile->checkPointCopy = checkPoint;
	/* some additional ControlFile fields are set in WriteControlFile() */

	WriteControlFile();

	/* Bootstrap the commit log, too */
	BootStrapCLOG();
	BootStrapSUBTRANS();
}

static char *
str_time(time_t tnow)
{
	static char buf[128];

	strftime(buf, sizeof(buf),
			 "%Y-%m-%d %H:%M:%S %Z",
			 localtime(&tnow));

	return buf;
}

/*
 * See if there is a recovery command file (recovery.conf), and if so
 * read in parameters for archive recovery.
 *
 * XXX longer term intention is to expand this to
 * cater for additional parameters and controls
 * possibly use a flex lexer similar to the GUC one
 */
static void
readRecoveryCommandFile(void)
{
	char		recoveryCommandFile[MAXPGPATH];
	FILE	   *fd;
	char		cmdline[MAXPGPATH];
	TimeLineID	rtli = 0;
	bool		rtliGiven = false;
	bool		syntaxError = false;

	snprintf(recoveryCommandFile, MAXPGPATH, "%s/recovery.conf", DataDir);
	fd = AllocateFile(recoveryCommandFile, "r");
	if (fd == NULL)
	{
		if (errno == ENOENT)
			return;				/* not there, so no archive recovery */
		ereport(FATAL,
				(errcode_for_file_access(),
				 errmsg("could not open recovery command file \"%s\": %m",
						recoveryCommandFile)));
	}

	ereport(LOG,
			(errmsg("starting archive recovery")));

	/*
	 * Parse the file...
	 */
	while (fgets(cmdline, MAXPGPATH, fd) != NULL)
	{
		/* skip leading whitespace and check for # comment */
		char	   *ptr;
		char	   *tok1;
		char	   *tok2;

		for (ptr = cmdline; *ptr; ptr++)
		{
			if (!isspace((unsigned char) *ptr))
				break;
		}
		if (*ptr == '\0' || *ptr == '#')
			continue;

		/* identify the quoted parameter value */
		tok1 = strtok(ptr, "'");
		if (!tok1)
		{
			syntaxError = true;
			break;
		}
		tok2 = strtok(NULL, "'");
		if (!tok2)
		{
			syntaxError = true;
			break;
		}
		/* reparse to get just the parameter name */
		tok1 = strtok(ptr, " \t=");
		if (!tok1)
		{
			syntaxError = true;
			break;
		}

		if (strcmp(tok1, "restore_command") == 0)
		{
			recoveryRestoreCommand = pstrdup(tok2);
			ereport(LOG,
					(errmsg("restore_command = \"%s\"",
							recoveryRestoreCommand)));
		}
		else if (strcmp(tok1, "recovery_target_timeline") == 0)
		{
			rtliGiven = true;
			if (strcmp(tok2, "latest") == 0)
				rtli = 0;
			else
			{
				errno = 0;
				rtli = (TimeLineID) strtoul(tok2, NULL, 0);
				if (errno == EINVAL || errno == ERANGE)
					ereport(FATAL,
							(errmsg("recovery_target_timeline is not a valid number: \"%s\"",
									tok2)));
			}
			if (rtli)
				ereport(LOG,
						(errmsg("recovery_target_timeline = %u", rtli)));
			else
				ereport(LOG,
						(errmsg("recovery_target_timeline = latest")));
		}
		else if (strcmp(tok1, "recovery_target_xid") == 0)
		{
			errno = 0;
			recoveryTargetXid = (TransactionId) strtoul(tok2, NULL, 0);
			if (errno == EINVAL || errno == ERANGE)
				ereport(FATAL,
						(errmsg("recovery_target_xid is not a valid number: \"%s\"",
								tok2)));
			ereport(LOG,
					(errmsg("recovery_target_xid = %u",
							recoveryTargetXid)));
			recoveryTarget = true;
			recoveryTargetExact = true;
		}
		else if (strcmp(tok1, "recovery_target_time") == 0)
		{
			/*
			 * if recovery_target_xid specified, then this overrides
			 * recovery_target_time
			 */
			if (recoveryTargetExact)
				continue;
			recoveryTarget = true;
			recoveryTargetExact = false;

			/*
			 * Convert the time string given by the user to the time_t
			 * format. We use type abstime's input converter because we
			 * know abstime has the same representation as time_t.
			 */
			recoveryTargetTime = (time_t)
				DatumGetAbsoluteTime(DirectFunctionCall1(abstimein,
												 CStringGetDatum(tok2)));
			ereport(LOG,
					(errmsg("recovery_target_time = %s",
						  DatumGetCString(DirectFunctionCall1(abstimeout,
			AbsoluteTimeGetDatum((AbsoluteTime) recoveryTargetTime))))));
		}
		else if (strcmp(tok1, "recovery_target_inclusive") == 0)
		{
			/*
			 * does nothing if a recovery_target is not also set
			 */
			if (strcmp(tok2, "true") == 0)
				recoveryTargetInclusive = true;
			else
			{
				recoveryTargetInclusive = false;
				tok2 = "false";
			}
			ereport(LOG,
					(errmsg("recovery_target_inclusive = %s", tok2)));
		}
		else
			ereport(FATAL,
					(errmsg("unrecognized recovery parameter \"%s\"",
							tok1)));
	}

	FreeFile(fd);

	if (syntaxError)
		ereport(FATAL,
				(errmsg("syntax error in recovery command file: %s",
						cmdline),
		  errhint("Lines should have the format parameter = 'value'.")));

	/* Check that required parameters were supplied */
	if (recoveryRestoreCommand == NULL)
		ereport(FATAL,
				(errmsg("recovery command file \"%s\" did not specify restore_command",
						recoveryCommandFile)));

	/* Enable fetching from archive recovery area */
	InArchiveRecovery = true;

	/*
	 * If user specified recovery_target_timeline, validate it or compute
	 * the "latest" value.	We can't do this until after we've gotten the
	 * restore command and set InArchiveRecovery, because we need to fetch
	 * timeline history files from the archive.
	 */
	if (rtliGiven)
	{
		if (rtli)
		{
			/* Timeline 1 does not have a history file, all else should */
			if (rtli != 1 && !existsTimeLineHistory(rtli))
				ereport(FATAL,
					(errmsg("recovery_target_timeline %u does not exist",
							rtli)));
			recoveryTargetTLI = rtli;
		}
		else
		{
			/* We start the "latest" search from pg_control's timeline */
			recoveryTargetTLI = findNewestTimeLine(recoveryTargetTLI);
		}
	}
}

/*
 * Exit archive-recovery state
 */
static void
exitArchiveRecovery(TimeLineID endTLI, uint32 endLogId, uint32 endLogSeg)
{
	char		recoveryPath[MAXPGPATH];
	char		xlogpath[MAXPGPATH];
	char		recoveryCommandFile[MAXPGPATH];
	char		recoveryCommandDone[MAXPGPATH];

	/*
	 * We are no longer in archive recovery state.
	 */
	InArchiveRecovery = false;

	/*
	 * We should have the ending log segment currently open.  Verify, and
	 * then close it (to avoid problems on Windows with trying to rename
	 * or delete an open file).
	 */
	Assert(readFile >= 0);
	Assert(readId == endLogId);
	Assert(readSeg == endLogSeg);

	close(readFile);
	readFile = -1;

	/*
	 * If the segment was fetched from archival storage, we want to
	 * replace the existing xlog segment (if any) with the archival
	 * version.  This is because whatever is in XLogDir is very possibly
	 * older than what we have from the archives, since it could have come
	 * from restoring a PGDATA backup.	In any case, the archival version
	 * certainly is more descriptive of what our current database state
	 * is, because that is what we replayed from.
	 *
	 * Note that if we are establishing a new timeline, ThisTimeLineID is
	 * already set to the new value, and so we will create a new file
	 * instead of overwriting any existing file.
	 */
	snprintf(recoveryPath, MAXPGPATH, "%s/RECOVERYXLOG", XLogDir);
	XLogFilePath(xlogpath, ThisTimeLineID, endLogId, endLogSeg);

	if (restoredFromArchive)
	{
		ereport(DEBUG3,
				(errmsg_internal("moving last restored xlog to \"%s\"",
								 xlogpath)));
		unlink(xlogpath);		/* might or might not exist */
		if (rename(recoveryPath, xlogpath) != 0)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not rename file \"%s\" to \"%s\": %m",
							recoveryPath, xlogpath)));
		/* XXX might we need to fix permissions on the file? */
	}
	else
	{
		/*
		 * If the latest segment is not archival, but there's still a
		 * RECOVERYXLOG laying about, get rid of it.
		 */
		unlink(recoveryPath);	/* ignore any error */

		/*
		 * If we are establishing a new timeline, we have to copy data
		 * from the last WAL segment of the old timeline to create a
		 * starting WAL segment for the new timeline.
		 */
		if (endTLI != ThisTimeLineID)
			XLogFileCopy(endLogId, endLogSeg,
						 endTLI, endLogId, endLogSeg);
	}

	/*
	 * Let's just make real sure there are not .ready or .done flags
	 * posted for the new segment.
	 */
	XLogFileName(xlogpath, ThisTimeLineID, endLogId, endLogSeg);
	XLogArchiveCleanup(xlogpath);

	/* Get rid of any remaining recovered timeline-history file, too */
	snprintf(recoveryPath, MAXPGPATH, "%s/RECOVERYHISTORY", XLogDir);
	unlink(recoveryPath);		/* ignore any error */

	/*
	 * Rename the config file out of the way, so that we don't
	 * accidentally re-enter archive recovery mode in a subsequent crash.
	 */
	snprintf(recoveryCommandFile, MAXPGPATH, "%s/recovery.conf", DataDir);
	snprintf(recoveryCommandDone, MAXPGPATH, "%s/recovery.done", DataDir);
	unlink(recoveryCommandDone);
	if (rename(recoveryCommandFile, recoveryCommandDone) != 0)
		ereport(FATAL,
				(errcode_for_file_access(),
				 errmsg("could not rename file \"%s\" to \"%s\": %m",
						recoveryCommandFile, recoveryCommandDone)));

	ereport(LOG,
			(errmsg("archive recovery complete")));
}

/*
 * For point-in-time recovery, this function decides whether we want to
 * stop applying the XLOG at or after the current record.
 *
 * Returns TRUE if we are stopping, FALSE otherwise.  On TRUE return,
 * *includeThis is set TRUE if we should apply this record before stopping.
 * Also, some information is saved in recoveryStopXid et al for use in
 * annotating the new timeline's history file.
 */
static bool
recoveryStopsHere(XLogRecord *record, bool *includeThis)
{
	bool		stopsHere;
	uint8		record_info;
	time_t		recordXtime;

	/* Do we have a PITR target at all? */
	if (!recoveryTarget)
		return false;

	/* We only consider stopping at COMMIT or ABORT records */
	if (record->xl_rmid != RM_XACT_ID)
		return false;
	record_info = record->xl_info & ~XLR_INFO_MASK;
	if (record_info == XLOG_XACT_COMMIT)
	{
		xl_xact_commit *recordXactCommitData;

		recordXactCommitData = (xl_xact_commit *) XLogRecGetData(record);
		recordXtime = recordXactCommitData->xtime;
	}
	else if (record_info == XLOG_XACT_ABORT)
	{
		xl_xact_abort *recordXactAbortData;

		recordXactAbortData = (xl_xact_abort *) XLogRecGetData(record);
		recordXtime = recordXactAbortData->xtime;
	}
	else
		return false;

	if (recoveryTargetExact)
	{
		/*
		 * there can be only one transaction end record with this exact
		 * transactionid
		 *
		 * when testing for an xid, we MUST test for equality only, since
		 * transactions are numbered in the order they start, not the
		 * order they complete. A higher numbered xid will complete before
		 * you about 50% of the time...
		 */
		stopsHere = (record->xl_xid == recoveryTargetXid);
		if (stopsHere)
			*includeThis = recoveryTargetInclusive;
	}
	else
	{
		/*
		 * there can be many transactions that share the same commit time,
		 * so we stop after the last one, if we are inclusive, or stop at
		 * the first one if we are exclusive
		 */
		if (recoveryTargetInclusive)
			stopsHere = (recordXtime > recoveryTargetTime);
		else
			stopsHere = (recordXtime >= recoveryTargetTime);
		if (stopsHere)
			*includeThis = false;
	}

	if (stopsHere)
	{
		recoveryStopXid = record->xl_xid;
		recoveryStopTime = recordXtime;
		recoveryStopAfter = *includeThis;

		if (record_info == XLOG_XACT_COMMIT)
		{
			if (recoveryStopAfter)
				ereport(LOG,
						(errmsg("recovery stopping after commit of transaction %u, time %s",
						  recoveryStopXid, str_time(recoveryStopTime))));
			else
				ereport(LOG,
						(errmsg("recovery stopping before commit of transaction %u, time %s",
						  recoveryStopXid, str_time(recoveryStopTime))));
		}
		else
		{
			if (recoveryStopAfter)
				ereport(LOG,
						(errmsg("recovery stopping after abort of transaction %u, time %s",
						  recoveryStopXid, str_time(recoveryStopTime))));
			else
				ereport(LOG,
						(errmsg("recovery stopping before abort of transaction %u, time %s",
						  recoveryStopXid, str_time(recoveryStopTime))));
		}
	}

	return stopsHere;
}

/*
 * This must be called ONCE during postmaster or standalone-backend startup
 */
void
StartupXLOG(void)
{
	XLogCtlInsert *Insert;
	CheckPoint	checkPoint;
	bool		wasShutdown;
	bool		needNewTimeLine = false;
	XLogRecPtr	RecPtr,
				LastRec,
				checkPointLoc,
				EndOfLog;
	uint32		endLogId;
	uint32		endLogSeg;
	XLogRecord *record;
	uint32		freespace;

	CritSectionCount++;

	/*
	 * Read control file and check XLOG status looks valid.
	 *
	 * Note: in most control paths, *ControlFile is already valid and we need
	 * not do ReadControlFile() here, but might as well do it to be sure.
	 */
	ReadControlFile();

	if (ControlFile->logSeg == 0 ||
		ControlFile->state < DB_SHUTDOWNED ||
		ControlFile->state > DB_IN_PRODUCTION ||
		!XRecOffIsValid(ControlFile->checkPoint.xrecoff))
		ereport(FATAL,
				(errmsg("control file contains invalid data")));

	if (ControlFile->state == DB_SHUTDOWNED)
		ereport(LOG,
				(errmsg("database system was shut down at %s",
						str_time(ControlFile->time))));
	else if (ControlFile->state == DB_SHUTDOWNING)
		ereport(LOG,
				(errmsg("database system shutdown was interrupted at %s",
						str_time(ControlFile->time))));
	else if (ControlFile->state == DB_IN_RECOVERY)
		ereport(LOG,
		(errmsg("database system was interrupted while in recovery at %s",
				str_time(ControlFile->time)),
		 errhint("This probably means that some data is corrupted and"
				 " you will have to use the last backup for recovery.")));
	else if (ControlFile->state == DB_IN_PRODUCTION)
		ereport(LOG,
				(errmsg("database system was interrupted at %s",
						str_time(ControlFile->time))));

	/* This is just to allow attaching to startup process with a debugger */
#ifdef XLOG_REPLAY_DELAY
	if (ControlFile->state != DB_SHUTDOWNED)
		pg_usleep(60000000L);
#endif

	/*
	 * Initialize on the assumption we want to recover to the same
	 * timeline that's active according to pg_control.
	 */
	recoveryTargetTLI = ControlFile->checkPointCopy.ThisTimeLineID;

	/*
	 * Check for recovery control file, and if so set up state for offline
	 * recovery
	 */
	readRecoveryCommandFile();

	/* Now we can determine the list of expected TLIs */
	expectedTLIs = readTimeLineHistory(recoveryTargetTLI);

	/*
	 * If pg_control's timeline is not in expectedTLIs, then we cannot
	 * proceed: the backup is not part of the history of the requested
	 * timeline.
	 */
	if (!list_member_int(expectedTLIs,
					   (int) ControlFile->checkPointCopy.ThisTimeLineID))
		ereport(FATAL,
				(errmsg("requested timeline %u is not a child of database system timeline %u",
						recoveryTargetTLI,
						ControlFile->checkPointCopy.ThisTimeLineID)));

	if (read_backup_label(&checkPointLoc))
	{
		/*
		 * When a backup_label file is present, we want to roll forward
		 * from the checkpoint it identifies, rather than using
		 * pg_control.
		 */
		record = ReadCheckpointRecord(checkPointLoc, 0);
		if (record != NULL)
		{
			ereport(LOG,
					(errmsg("checkpoint record is at %X/%X",
						  checkPointLoc.xlogid, checkPointLoc.xrecoff)));
			InRecovery = true;	/* force recovery even if SHUTDOWNED */
		}
		else
		{
			ereport(PANIC,
				  (errmsg("could not locate required checkpoint record"),
				   errhint("If you are not restoring from a backup, try removing the file \"%s/backup_label\".", DataDir)));
		}
	}
	else
	{
		/*
		 * Get the last valid checkpoint record.  If the latest one
		 * according to pg_control is broken, try the next-to-last one.
		 */
		checkPointLoc = ControlFile->checkPoint;
		record = ReadCheckpointRecord(checkPointLoc, 1);
		if (record != NULL)
		{
			ereport(LOG,
					(errmsg("checkpoint record is at %X/%X",
						  checkPointLoc.xlogid, checkPointLoc.xrecoff)));
		}
		else
		{
			checkPointLoc = ControlFile->prevCheckPoint;
			record = ReadCheckpointRecord(checkPointLoc, 2);
			if (record != NULL)
			{
				ereport(LOG,
					 (errmsg("using previous checkpoint record at %X/%X",
						  checkPointLoc.xlogid, checkPointLoc.xrecoff)));
				InRecovery = true;		/* force recovery even if
										 * SHUTDOWNED */
			}
			else
				ereport(PANIC,
				 (errmsg("could not locate a valid checkpoint record")));
		}
	}

	LastRec = RecPtr = checkPointLoc;
	memcpy(&checkPoint, XLogRecGetData(record), sizeof(CheckPoint));
	wasShutdown = (record->xl_info == XLOG_CHECKPOINT_SHUTDOWN);

	ereport(LOG,
			(errmsg("redo record is at %X/%X; undo record is at %X/%X; shutdown %s",
					checkPoint.redo.xlogid, checkPoint.redo.xrecoff,
					checkPoint.undo.xlogid, checkPoint.undo.xrecoff,
					wasShutdown ? "TRUE" : "FALSE")));
	ereport(LOG,
			(errmsg("next transaction ID: %u; next OID: %u",
					checkPoint.nextXid, checkPoint.nextOid)));
	if (!TransactionIdIsNormal(checkPoint.nextXid))
		ereport(PANIC,
				(errmsg("invalid next transaction ID")));

	ShmemVariableCache->nextXid = checkPoint.nextXid;
	ShmemVariableCache->nextOid = checkPoint.nextOid;
	ShmemVariableCache->oidCount = 0;

	/*
	 * We must replay WAL entries using the same TimeLineID they were
	 * created under, so temporarily adopt the TLI indicated by the
	 * checkpoint (see also xlog_redo()).
	 */
	ThisTimeLineID = checkPoint.ThisTimeLineID;

	RedoRecPtr = XLogCtl->Insert.RedoRecPtr = checkPoint.redo;

	if (XLByteLT(RecPtr, checkPoint.redo))
		ereport(PANIC,
				(errmsg("invalid redo in checkpoint record")));
	if (checkPoint.undo.xrecoff == 0)
		checkPoint.undo = RecPtr;

	/*
	 * Check whether we need to force recovery from WAL.  If it appears to
	 * have been a clean shutdown and we did not have a recovery.conf
	 * file, then assume no recovery needed.
	 */
	if (XLByteLT(checkPoint.undo, RecPtr) ||
		XLByteLT(checkPoint.redo, RecPtr))
	{
		if (wasShutdown)
			ereport(PANIC,
			(errmsg("invalid redo/undo record in shutdown checkpoint")));
		InRecovery = true;
	}
	else if (ControlFile->state != DB_SHUTDOWNED)
		InRecovery = true;
	else if (InArchiveRecovery)
	{
		/* force recovery due to presence of recovery.conf */
		InRecovery = true;
	}

	/* REDO */
	if (InRecovery)
	{
		int			rmid;

		if (InArchiveRecovery)
			ereport(LOG,
					(errmsg("automatic recovery in progress")));
		else
			ereport(LOG,
					(errmsg("database system was not properly shut down; "
							"automatic recovery in progress")));
		ControlFile->state = DB_IN_RECOVERY;
		ControlFile->time = time(NULL);
		UpdateControlFile();

		/* Start up the recovery environment */
		XLogInitRelationCache();

		for (rmid = 0; rmid <= RM_MAX_ID; rmid++)
		{
			if (RmgrTable[rmid].rm_startup != NULL)
				RmgrTable[rmid].rm_startup();
		}

		/*
		 * Find the first record that logically follows the checkpoint ---
		 * it might physically precede it, though.
		 */
		if (XLByteLT(checkPoint.redo, RecPtr))
		{
			/* back up to find the record */
			record = ReadRecord(&(checkPoint.redo), PANIC);
		}
		else
		{
			/* just have to read next record after CheckPoint */
			record = ReadRecord(NULL, LOG);
		}

		if (record != NULL)
		{
			bool		recoveryContinue = true;
			bool		recoveryApply = true;

			InRedo = true;
			ereport(LOG,
					(errmsg("redo starts at %X/%X",
							ReadRecPtr.xlogid, ReadRecPtr.xrecoff)));

			/*
			 * main redo apply loop
			 */
			do
			{
#ifdef WAL_DEBUG
				if (XLOG_DEBUG)
				{
					char		buf[8192];

					sprintf(buf, "REDO @ %X/%X; LSN %X/%X: ",
							ReadRecPtr.xlogid, ReadRecPtr.xrecoff,
							EndRecPtr.xlogid, EndRecPtr.xrecoff);
					xlog_outrec(buf, record);
					strcat(buf, " - ");
					RmgrTable[record->xl_rmid].rm_desc(buf,
								record->xl_info, XLogRecGetData(record));
					elog(LOG, "%s", buf);
				}
#endif

				/*
				 * Have we reached our recovery target?
				 */
				if (recoveryStopsHere(record, &recoveryApply))
				{
					needNewTimeLine = true;		/* see below */
					recoveryContinue = false;
					if (!recoveryApply)
						break;
				}

				/* nextXid must be beyond record's xid */
				if (TransactionIdFollowsOrEquals(record->xl_xid,
											ShmemVariableCache->nextXid))
				{
					ShmemVariableCache->nextXid = record->xl_xid;
					TransactionIdAdvance(ShmemVariableCache->nextXid);
				}

				if (record->xl_info & XLR_BKP_BLOCK_MASK)
					RestoreBkpBlocks(record, EndRecPtr);

				RmgrTable[record->xl_rmid].rm_redo(EndRecPtr, record);

				LastRec = ReadRecPtr;

				record = ReadRecord(NULL, LOG);
			} while (record != NULL && recoveryContinue);

			/*
			 * end of main redo apply loop
			 */

			ereport(LOG,
					(errmsg("redo done at %X/%X",
							ReadRecPtr.xlogid, ReadRecPtr.xrecoff)));
			InRedo = false;
		}
		else
		{
			/* there are no WAL records following the checkpoint */
			ereport(LOG,
					(errmsg("redo is not required")));
		}
	}

	/*
	 * Re-fetch the last valid or last applied record, so we can identify
	 * the exact endpoint of what we consider the valid portion of WAL.
	 */
	record = ReadRecord(&LastRec, PANIC);
	EndOfLog = EndRecPtr;
	XLByteToPrevSeg(EndOfLog, endLogId, endLogSeg);

	/*
	 * Complain if we did not roll forward far enough to render the backup
	 * dump consistent.
	 */
	if (XLByteLT(EndOfLog, recoveryMinXlogOffset))
	{
		if (needNewTimeLine)	/* stopped because of stop request */
			ereport(FATAL,
					(errmsg("requested recovery stop point is before end time of backup dump")));
		else
			/* ran off end of WAL */
			ereport(FATAL,
					(errmsg("WAL ends before end time of backup dump")));
	}

	/*
	 * Consider whether we need to assign a new timeline ID.
	 *
	 * If we stopped short of the end of WAL during recovery, then we are
	 * generating a new timeline and must assign it a unique new ID.
	 * Otherwise, we can just extend the timeline we were in when we ran
	 * out of WAL.
	 */
	if (needNewTimeLine)
	{
		ThisTimeLineID = findNewestTimeLine(recoveryTargetTLI) + 1;
		ereport(LOG,
				(errmsg("selected new timeline ID: %u", ThisTimeLineID)));
		writeTimeLineHistory(ThisTimeLineID, recoveryTargetTLI,
							 curFileTLI, endLogId, endLogSeg);
	}

	/* Save the selected TimeLineID in shared memory, too */
	XLogCtl->ThisTimeLineID = ThisTimeLineID;

	/*
	 * We are now done reading the old WAL.  Turn off archive fetching if
	 * it was active, and make a writable copy of the last WAL segment.
	 * (Note that we also have a copy of the last block of the old WAL in
	 * readBuf; we will use that below.)
	 */
	if (InArchiveRecovery)
		exitArchiveRecovery(curFileTLI, endLogId, endLogSeg);

	/*
	 * Prepare to write WAL starting at EndOfLog position, and init xlog
	 * buffer cache using the block containing the last record from the
	 * previous incarnation.
	 */
	openLogId = endLogId;
	openLogSeg = endLogSeg;
	openLogFile = XLogFileOpen(openLogId, openLogSeg);
	openLogOff = 0;
	ControlFile->logId = openLogId;
	ControlFile->logSeg = openLogSeg + 1;
	Insert = &XLogCtl->Insert;
	Insert->PrevRecord = LastRec;
	XLogCtl->xlblocks[0].xlogid = openLogId;
	XLogCtl->xlblocks[0].xrecoff =
		((EndOfLog.xrecoff - 1) / BLCKSZ + 1) * BLCKSZ;

	/*
	 * Tricky point here: readBuf contains the *last* block that the
	 * LastRec record spans, not the one it starts in.	The last block is
	 * indeed the one we want to use.
	 */
	Assert(readOff == (XLogCtl->xlblocks[0].xrecoff - BLCKSZ) % XLogSegSize);
	memcpy((char *) Insert->currpage, readBuf, BLCKSZ);
	Insert->currpos = (char *) Insert->currpage +
		(EndOfLog.xrecoff + BLCKSZ - XLogCtl->xlblocks[0].xrecoff);

	LogwrtResult.Write = LogwrtResult.Flush = EndOfLog;

	XLogCtl->Write.LogwrtResult = LogwrtResult;
	Insert->LogwrtResult = LogwrtResult;
	XLogCtl->LogwrtResult = LogwrtResult;

	XLogCtl->LogwrtRqst.Write = EndOfLog;
	XLogCtl->LogwrtRqst.Flush = EndOfLog;

	freespace = INSERT_FREESPACE(Insert);
	if (freespace > 0)
	{
		/* Make sure rest of page is zero */
		MemSet(Insert->currpos, 0, freespace);
		XLogCtl->Write.curridx = 0;
	}
	else
	{
		/*
		 * Whenever Write.LogwrtResult points to exactly the end of a
		 * page, Write.curridx must point to the *next* page (see
		 * XLogWrite()).
		 *
		 * Note: it might seem we should do AdvanceXLInsertBuffer() here, but
		 * this is sufficient.	The first actual attempt to insert a log
		 * record will advance the insert state.
		 */
		XLogCtl->Write.curridx = NextBufIdx(0);
	}

#ifdef NOT_USED
	/* UNDO */
	if (InRecovery)
	{
		RecPtr = ReadRecPtr;
		if (XLByteLT(checkPoint.undo, RecPtr))
		{
			ereport(LOG,
					(errmsg("undo starts at %X/%X",
							RecPtr.xlogid, RecPtr.xrecoff)));
			do
			{
				record = ReadRecord(&RecPtr, PANIC);
				if (TransactionIdIsValid(record->xl_xid) &&
					!TransactionIdDidCommit(record->xl_xid))
					RmgrTable[record->xl_rmid].rm_undo(EndRecPtr, record);
				RecPtr = record->xl_prev;
			} while (XLByteLE(checkPoint.undo, RecPtr));
			ereport(LOG,
					(errmsg("undo done at %X/%X",
							ReadRecPtr.xlogid, ReadRecPtr.xrecoff)));
		}
		else
			ereport(LOG,
					(errmsg("undo is not required")));
	}
#endif

	if (InRecovery)
	{
		int			rmid;

		/*
		 * Allow resource managers to do any required cleanup.
		 */
		for (rmid = 0; rmid <= RM_MAX_ID; rmid++)
		{
			if (RmgrTable[rmid].rm_cleanup != NULL)
				RmgrTable[rmid].rm_cleanup();
		}

		/*
		 * Perform a new checkpoint to update our recovery activity to
		 * disk.
		 *
		 * Note that we write a shutdown checkpoint rather than an on-line
		 * one.  This is not particularly critical, but since we may be
		 * assigning a new TLI, using a shutdown checkpoint allows us to
		 * have the rule that TLI only changes in shutdown checkpoints,
		 * which allows some extra error checking in xlog_redo.
		 *
		 * In case we had to use the secondary checkpoint, make sure that it
		 * will still be shown as the secondary checkpoint after this
		 * CreateCheckPoint operation; we don't want the broken primary
		 * checkpoint to become prevCheckPoint...
		 */
		if (XLByteEQ(checkPointLoc, ControlFile->prevCheckPoint))
			ControlFile->checkPoint = checkPointLoc;

		CreateCheckPoint(true, true);

		/*
		 * Close down recovery environment
		 */
		XLogCloseRelationCache();

		/*
		 * Now that we've checkpointed the recovery, it's safe to flush
		 * old backup_label, if present.
		 */
		remove_backup_label();
	}

	/*
	 * Preallocate additional log files, if wanted.
	 */
	(void) PreallocXlogFiles(EndOfLog);

	/*
	 * Okay, we're officially UP.
	 */
	InRecovery = false;

	ControlFile->state = DB_IN_PRODUCTION;
	ControlFile->time = time(NULL);
	UpdateControlFile();

	/* Start up the commit log, too */
	StartupCLOG();
	StartupSUBTRANS();

	ereport(LOG,
			(errmsg("database system is ready")));
	CritSectionCount--;

	/* Shut down readFile facility, free space */
	if (readFile >= 0)
	{
		close(readFile);
		readFile = -1;
	}
	if (readBuf)
	{
		free(readBuf);
		readBuf = NULL;
	}
	if (readRecordBuf)
	{
		free(readRecordBuf);
		readRecordBuf = NULL;
		readRecordBufSize = 0;
	}
}

/*
 * Subroutine to try to fetch and validate a prior checkpoint record.
 *
 * whichChkpt identifies the checkpoint (merely for reporting purposes).
 * 1 for "primary", 2 for "secondary", 0 for "other" (backup_label)
 */
static XLogRecord *
ReadCheckpointRecord(XLogRecPtr RecPtr, int whichChkpt)
{
	XLogRecord *record;

	if (!XRecOffIsValid(RecPtr.xrecoff))
	{
		switch (whichChkpt)
		{
			case 1:
				ereport(LOG,
						(errmsg("invalid primary checkpoint link in control file")));
				break;
			case 2:
				ereport(LOG,
						(errmsg("invalid secondary checkpoint link in control file")));
				break;
			default:
				ereport(LOG,
				(errmsg("invalid checkpoint link in backup_label file")));
				break;
		}
		return NULL;
	}

	record = ReadRecord(&RecPtr, LOG);

	if (record == NULL)
	{
		switch (whichChkpt)
		{
			case 1:
				ereport(LOG,
						(errmsg("invalid primary checkpoint record")));
				break;
			case 2:
				ereport(LOG,
						(errmsg("invalid secondary checkpoint record")));
				break;
			default:
				ereport(LOG,
						(errmsg("invalid checkpoint record")));
				break;
		}
		return NULL;
	}
	if (record->xl_rmid != RM_XLOG_ID)
	{
		switch (whichChkpt)
		{
			case 1:
				ereport(LOG,
						(errmsg("invalid resource manager ID in primary checkpoint record")));
				break;
			case 2:
				ereport(LOG,
						(errmsg("invalid resource manager ID in secondary checkpoint record")));
				break;
			default:
				ereport(LOG,
						(errmsg("invalid resource manager ID in checkpoint record")));
				break;
		}
		return NULL;
	}
	if (record->xl_info != XLOG_CHECKPOINT_SHUTDOWN &&
		record->xl_info != XLOG_CHECKPOINT_ONLINE)
	{
		switch (whichChkpt)
		{
			case 1:
				ereport(LOG,
				(errmsg("invalid xl_info in primary checkpoint record")));
				break;
			case 2:
				ereport(LOG,
						(errmsg("invalid xl_info in secondary checkpoint record")));
				break;
			default:
				ereport(LOG,
						(errmsg("invalid xl_info in checkpoint record")));
				break;
		}
		return NULL;
	}
	if (record->xl_len != sizeof(CheckPoint))
	{
		switch (whichChkpt)
		{
			case 1:
				ereport(LOG,
				(errmsg("invalid length of primary checkpoint record")));
				break;
			case 2:
				ereport(LOG,
						(errmsg("invalid length of secondary checkpoint record")));
				break;
			default:
				ereport(LOG,
						(errmsg("invalid length of checkpoint record")));
				break;
		}
		return NULL;
	}
	return record;
}

/*
 * This must be called during startup of a backend process, except that
 * it need not be called in a standalone backend (which does StartupXLOG
 * instead).  We need to initialize the local copies of ThisTimeLineID and
 * RedoRecPtr.
 *
 * Note: before Postgres 8.0, we went to some effort to keep the postmaster
 * process's copies of ThisTimeLineID and RedoRecPtr valid too.  This was
 * unnecessary however, since the postmaster itself never touches XLOG anyway.
 */
void
InitXLOGAccess(void)
{
	/* ThisTimeLineID doesn't change so we need no lock to copy it */
	ThisTimeLineID = XLogCtl->ThisTimeLineID;
	/* Use GetRedoRecPtr to copy the RedoRecPtr safely */
	(void) GetRedoRecPtr();
}

/*
 * Once spawned, a backend may update its local RedoRecPtr from
 * XLogCtl->Insert.RedoRecPtr; it must hold the insert lock or info_lck
 * to do so.  This is done in XLogInsert() or GetRedoRecPtr().
 */
XLogRecPtr
GetRedoRecPtr(void)
{
	/* use volatile pointer to prevent code rearrangement */
	volatile XLogCtlData *xlogctl = XLogCtl;

	SpinLockAcquire_NoHoldoff(&xlogctl->info_lck);
	Assert(XLByteLE(RedoRecPtr, xlogctl->Insert.RedoRecPtr));
	RedoRecPtr = xlogctl->Insert.RedoRecPtr;
	SpinLockRelease_NoHoldoff(&xlogctl->info_lck);

	return RedoRecPtr;
}

/*
 * This must be called ONCE during postmaster or standalone-backend shutdown
 */
void
ShutdownXLOG(int code, Datum arg)
{
	ereport(LOG,
			(errmsg("shutting down")));

	CritSectionCount++;
	CreateCheckPoint(true, true);
	ShutdownCLOG();
	ShutdownSUBTRANS();
	CritSectionCount--;

	ereport(LOG,
			(errmsg("database system is shut down")));
}

/*
 * Perform a checkpoint --- either during shutdown, or on-the-fly
 *
 * If force is true, we force a checkpoint regardless of whether any XLOG
 * activity has occurred since the last one.
 */
void
CreateCheckPoint(bool shutdown, bool force)
{
	CheckPoint	checkPoint;
	XLogRecPtr	recptr;
	XLogCtlInsert *Insert = &XLogCtl->Insert;
	XLogRecData rdata;
	uint32		freespace;
	uint32		_logId;
	uint32		_logSeg;
	int			nsegsadded = 0;
	int			nsegsremoved = 0;
	int			nsegsrecycled = 0;

	/*
	 * Acquire CheckpointLock to ensure only one checkpoint happens at a
	 * time.  (This is just pro forma, since in the present system
	 * structure there is only one process that is allowed to issue
	 * checkpoints at any given time.)
	 */
	LWLockAcquire(CheckpointLock, LW_EXCLUSIVE);

	/*
	 * Use a critical section to force system panic if we have trouble.
	 */
	START_CRIT_SECTION();

	if (shutdown)
	{
		ControlFile->state = DB_SHUTDOWNING;
		ControlFile->time = time(NULL);
		UpdateControlFile();
	}

	MemSet(&checkPoint, 0, sizeof(checkPoint));
	checkPoint.ThisTimeLineID = ThisTimeLineID;
	checkPoint.time = time(NULL);

	/*
	 * We must hold CheckpointStartLock while determining the checkpoint
	 * REDO pointer.  This ensures that any concurrent transaction commits
	 * will be either not yet logged, or logged and recorded in pg_clog.
	 * See notes in RecordTransactionCommit().
	 */
	LWLockAcquire(CheckpointStartLock, LW_EXCLUSIVE);

	/* And we need WALInsertLock too */
	LWLockAcquire(WALInsertLock, LW_EXCLUSIVE);

	/*
	 * If this isn't a shutdown or forced checkpoint, and we have not
	 * inserted any XLOG records since the start of the last checkpoint,
	 * skip the checkpoint.  The idea here is to avoid inserting duplicate
	 * checkpoints when the system is idle. That wastes log space, and
	 * more importantly it exposes us to possible loss of both current and
	 * previous checkpoint records if the machine crashes just as we're
	 * writing the update. (Perhaps it'd make even more sense to
	 * checkpoint only when the previous checkpoint record is in a
	 * different xlog page?)
	 *
	 * We have to make two tests to determine that nothing has happened since
	 * the start of the last checkpoint: current insertion point must
	 * match the end of the last checkpoint record, and its redo pointer
	 * must point to itself.
	 */
	if (!shutdown && !force)
	{
		XLogRecPtr	curInsert;

		INSERT_RECPTR(curInsert, Insert, Insert->curridx);
		if (curInsert.xlogid == ControlFile->checkPoint.xlogid &&
			curInsert.xrecoff == ControlFile->checkPoint.xrecoff +
			MAXALIGN(SizeOfXLogRecord + sizeof(CheckPoint)) &&
			ControlFile->checkPoint.xlogid ==
			ControlFile->checkPointCopy.redo.xlogid &&
			ControlFile->checkPoint.xrecoff ==
			ControlFile->checkPointCopy.redo.xrecoff)
		{
			LWLockRelease(WALInsertLock);
			LWLockRelease(CheckpointStartLock);
			LWLockRelease(CheckpointLock);
			END_CRIT_SECTION();
			return;
		}
	}

	/*
	 * Compute new REDO record ptr = location of next XLOG record.
	 *
	 * NB: this is NOT necessarily where the checkpoint record itself will
	 * be, since other backends may insert more XLOG records while we're
	 * off doing the buffer flush work.  Those XLOG records are logically
	 * after the checkpoint, even though physically before it.	Got that?
	 */
	freespace = INSERT_FREESPACE(Insert);
	if (freespace < SizeOfXLogRecord)
	{
		(void) AdvanceXLInsertBuffer();
		/* OK to ignore update return flag, since we will do flush anyway */
		freespace = INSERT_FREESPACE(Insert);
	}
	INSERT_RECPTR(checkPoint.redo, Insert, Insert->curridx);

	/*
	 * Here we update the shared RedoRecPtr for future XLogInsert calls;
	 * this must be done while holding the insert lock AND the info_lck.
	 *
	 * Note: if we fail to complete the checkpoint, RedoRecPtr will be left
	 * pointing past where it really needs to point.  This is okay; the
	 * only consequence is that XLogInsert might back up whole buffers
	 * that it didn't really need to.  We can't postpone advancing
	 * RedoRecPtr because XLogInserts that happen while we are dumping
	 * buffers must assume that their buffer changes are not included in
	 * the checkpoint.
	 */
	{
		/* use volatile pointer to prevent code rearrangement */
		volatile XLogCtlData *xlogctl = XLogCtl;

		SpinLockAcquire_NoHoldoff(&xlogctl->info_lck);
		RedoRecPtr = xlogctl->Insert.RedoRecPtr = checkPoint.redo;
		SpinLockRelease_NoHoldoff(&xlogctl->info_lck);
	}

	/*
	 * Get UNDO record ptr - this is oldest of PGPROC->logRec values. We
	 * do this while holding insert lock to ensure that we won't miss any
	 * about-to-commit transactions (UNDO must include all xacts that have
	 * commits after REDO point).
	 *
	 * XXX temporarily ifdef'd out to avoid three-way deadlock condition:
	 * GetUndoRecPtr needs to grab SInvalLock to ensure that it is looking
	 * at a stable set of proc records, but grabbing SInvalLock while
	 * holding WALInsertLock is no good.  GetNewTransactionId may cause a
	 * WAL record to be written while holding XidGenLock, and
	 * GetSnapshotData needs to get XidGenLock while holding SInvalLock,
	 * so there's a risk of deadlock. Need to find a better solution.  See
	 * pgsql-hackers discussion of 17-Dec-01.
	 *
	 * XXX actually, the whole UNDO code is dead code and unlikely to ever be
	 * revived, so the lack of a good solution here is not troubling.
	 */
#ifdef NOT_USED
	checkPoint.undo = GetUndoRecPtr();

	if (shutdown && checkPoint.undo.xrecoff != 0)
		elog(PANIC, "active transaction while database system is shutting down");
#endif

	/*
	 * Now we can release insert lock and checkpoint start lock, allowing
	 * other xacts to proceed even while we are flushing disk buffers.
	 */
	LWLockRelease(WALInsertLock);

	LWLockRelease(CheckpointStartLock);

	/*
	 * Get the other info we need for the checkpoint record.
	 */
	LWLockAcquire(XidGenLock, LW_SHARED);
	checkPoint.nextXid = ShmemVariableCache->nextXid;
	LWLockRelease(XidGenLock);

	LWLockAcquire(OidGenLock, LW_SHARED);
	checkPoint.nextOid = ShmemVariableCache->nextOid;
	if (!shutdown)
		checkPoint.nextOid += ShmemVariableCache->oidCount;
	LWLockRelease(OidGenLock);

	/*
	 * Having constructed the checkpoint record, ensure all shmem disk
	 * buffers and commit-log buffers are flushed to disk.
	 *
	 * This I/O could fail for various reasons.  If so, we will fail to
	 * complete the checkpoint, but there is no reason to force a system
	 * panic.  Accordingly, exit critical section while doing it.
	 */
	END_CRIT_SECTION();

	if (!shutdown)
		ereport(DEBUG1,
				(errmsg("checkpoint starting")));

	CheckPointCLOG();
	CheckPointSUBTRANS();
	FlushBufferPool();

	START_CRIT_SECTION();

	/*
	 * Now insert the checkpoint record into XLOG.
	 */
	rdata.buffer = InvalidBuffer;
	rdata.data = (char *) (&checkPoint);
	rdata.len = sizeof(checkPoint);
	rdata.next = NULL;

	recptr = XLogInsert(RM_XLOG_ID,
						shutdown ? XLOG_CHECKPOINT_SHUTDOWN :
						XLOG_CHECKPOINT_ONLINE,
						&rdata);

	XLogFlush(recptr);

	/*
	 * We now have ProcLastRecPtr = start of actual checkpoint record,
	 * recptr = end of actual checkpoint record.
	 */
	if (shutdown && !XLByteEQ(checkPoint.redo, ProcLastRecPtr))
		ereport(PANIC,
				(errmsg("concurrent transaction log activity while database system is shutting down")));

	/*
	 * Select point at which we can truncate the log, which we base on the
	 * prior checkpoint's earliest info.
	 *
	 * With UNDO support: oldest item is redo or undo, whichever is older;
	 * but watch out for case that undo = 0.
	 *
	 * Without UNDO support: just use the redo pointer.  This allows xlog
	 * space to be freed much faster when there are long-running
	 * transactions.
	 */
#ifdef NOT_USED
	if (ControlFile->checkPointCopy.undo.xrecoff != 0 &&
		XLByteLT(ControlFile->checkPointCopy.undo,
				 ControlFile->checkPointCopy.redo))
		XLByteToSeg(ControlFile->checkPointCopy.undo, _logId, _logSeg);
	else
#endif
		XLByteToSeg(ControlFile->checkPointCopy.redo, _logId, _logSeg);

	/*
	 * Update the control file.
	 */
	LWLockAcquire(ControlFileLock, LW_EXCLUSIVE);
	if (shutdown)
		ControlFile->state = DB_SHUTDOWNED;
	ControlFile->prevCheckPoint = ControlFile->checkPoint;
	ControlFile->checkPoint = ProcLastRecPtr;
	ControlFile->checkPointCopy = checkPoint;
	ControlFile->time = time(NULL);
	UpdateControlFile();
	LWLockRelease(ControlFileLock);

	/*
	 * We are now done with critical updates; no need for system panic if
	 * we have trouble while fooling with offline log segments.
	 */
	END_CRIT_SECTION();

	/*
	 * Delete offline log files (those no longer needed even for previous
	 * checkpoint).
	 */
	if (_logId || _logSeg)
	{
		PrevLogSeg(_logId, _logSeg);
		MoveOfflineLogs(_logId, _logSeg, recptr,
						&nsegsremoved, &nsegsrecycled);
	}

	/*
	 * Make more log segments if needed.  (Do this after deleting offline
	 * log segments, to avoid having peak disk space usage higher than
	 * necessary.)
	 */
	if (!shutdown)
		nsegsadded = PreallocXlogFiles(recptr);

	/*
	 * Truncate pg_subtrans if possible.  We can throw away all data
	 * before the oldest XMIN of any running transaction.  No future
	 * transaction will attempt to reference any pg_subtrans entry older
	 * than that (see Asserts in subtrans.c).  During recovery, though, we
	 * mustn't do this because StartupSUBTRANS hasn't been called yet.
	 */
	if (!InRecovery)
		TruncateSUBTRANS(GetOldestXmin(true));

	if (!shutdown)
		ereport(DEBUG1,
				(errmsg("checkpoint complete; %d transaction log file(s) added, %d removed, %d recycled",
						nsegsadded, nsegsremoved, nsegsrecycled)));

	LWLockRelease(CheckpointLock);
}

/*
 * Write a NEXTOID log record
 */
void
XLogPutNextOid(Oid nextOid)
{
	XLogRecData rdata;

	rdata.buffer = InvalidBuffer;
	rdata.data = (char *) (&nextOid);
	rdata.len = sizeof(Oid);
	rdata.next = NULL;
	(void) XLogInsert(RM_XLOG_ID, XLOG_NEXTOID, &rdata);
}

/*
 * XLOG resource manager's routines
 */
void
xlog_redo(XLogRecPtr lsn, XLogRecord *record)
{
	uint8		info = record->xl_info & ~XLR_INFO_MASK;

	if (info == XLOG_NEXTOID)
	{
		Oid			nextOid;

		memcpy(&nextOid, XLogRecGetData(record), sizeof(Oid));
		if (ShmemVariableCache->nextOid < nextOid)
		{
			ShmemVariableCache->nextOid = nextOid;
			ShmemVariableCache->oidCount = 0;
		}
	}
	else if (info == XLOG_CHECKPOINT_SHUTDOWN)
	{
		CheckPoint	checkPoint;

		memcpy(&checkPoint, XLogRecGetData(record), sizeof(CheckPoint));
		/* In a SHUTDOWN checkpoint, believe the counters exactly */
		ShmemVariableCache->nextXid = checkPoint.nextXid;
		ShmemVariableCache->nextOid = checkPoint.nextOid;
		ShmemVariableCache->oidCount = 0;

		/*
		 * TLI may change in a shutdown checkpoint, but it shouldn't
		 * decrease
		 */
		if (checkPoint.ThisTimeLineID != ThisTimeLineID)
		{
			if (checkPoint.ThisTimeLineID < ThisTimeLineID ||
				!list_member_int(expectedTLIs,
								 (int) checkPoint.ThisTimeLineID))
				ereport(PANIC,
						(errmsg("unexpected timeline ID %u (after %u) in checkpoint record",
							checkPoint.ThisTimeLineID, ThisTimeLineID)));
			/* Following WAL records should be run with new TLI */
			ThisTimeLineID = checkPoint.ThisTimeLineID;
		}
	}
	else if (info == XLOG_CHECKPOINT_ONLINE)
	{
		CheckPoint	checkPoint;

		memcpy(&checkPoint, XLogRecGetData(record), sizeof(CheckPoint));
		/* In an ONLINE checkpoint, treat the counters like NEXTOID */
		if (TransactionIdPrecedes(ShmemVariableCache->nextXid,
								  checkPoint.nextXid))
			ShmemVariableCache->nextXid = checkPoint.nextXid;
		if (ShmemVariableCache->nextOid < checkPoint.nextOid)
		{
			ShmemVariableCache->nextOid = checkPoint.nextOid;
			ShmemVariableCache->oidCount = 0;
		}
		/* TLI should not change in an on-line checkpoint */
		if (checkPoint.ThisTimeLineID != ThisTimeLineID)
			ereport(PANIC,
					(errmsg("unexpected timeline ID %u (should be %u) in checkpoint record",
							checkPoint.ThisTimeLineID, ThisTimeLineID)));
	}
}

void
xlog_undo(XLogRecPtr lsn, XLogRecord *record)
{
}

void
xlog_desc(char *buf, uint8 xl_info, char *rec)
{
	uint8		info = xl_info & ~XLR_INFO_MASK;

	if (info == XLOG_CHECKPOINT_SHUTDOWN ||
		info == XLOG_CHECKPOINT_ONLINE)
	{
		CheckPoint *checkpoint = (CheckPoint *) rec;

		sprintf(buf + strlen(buf), "checkpoint: redo %X/%X; undo %X/%X; "
				"tli %u; xid %u; oid %u; %s",
				checkpoint->redo.xlogid, checkpoint->redo.xrecoff,
				checkpoint->undo.xlogid, checkpoint->undo.xrecoff,
				checkpoint->ThisTimeLineID, checkpoint->nextXid,
				checkpoint->nextOid,
			 (info == XLOG_CHECKPOINT_SHUTDOWN) ? "shutdown" : "online");
	}
	else if (info == XLOG_NEXTOID)
	{
		Oid			nextOid;

		memcpy(&nextOid, rec, sizeof(Oid));
		sprintf(buf + strlen(buf), "nextOid: %u", nextOid);
	}
	else
		strcat(buf, "UNKNOWN");
}

#ifdef WAL_DEBUG

static void
xlog_outrec(char *buf, XLogRecord *record)
{
	int			bkpb;
	int			i;

	sprintf(buf + strlen(buf), "prev %X/%X; xid %u",
			record->xl_prev.xlogid, record->xl_prev.xrecoff,
			record->xl_xid);

	for (i = 0, bkpb = 0; i < XLR_MAX_BKP_BLOCKS; i++)
	{
		if (!(record->xl_info & (XLR_SET_BKP_BLOCK(i))))
			continue;
		bkpb++;
	}

	if (bkpb)
		sprintf(buf + strlen(buf), "; bkpb %d", bkpb);

	sprintf(buf + strlen(buf), ": %s",
			RmgrTable[record->xl_rmid].rm_name);
}
#endif   /* WAL_DEBUG */


/*
 * GUC support
 */
const char *
assign_xlog_sync_method(const char *method, bool doit, GucSource source)
{
	int			new_sync_method;
	int			new_sync_bit;

	if (pg_strcasecmp(method, "fsync") == 0)
	{
		new_sync_method = SYNC_METHOD_FSYNC;
		new_sync_bit = 0;
	}
#ifdef HAVE_FDATASYNC
	else if (pg_strcasecmp(method, "fdatasync") == 0)
	{
		new_sync_method = SYNC_METHOD_FDATASYNC;
		new_sync_bit = 0;
	}
#endif
#ifdef OPEN_SYNC_FLAG
	else if (pg_strcasecmp(method, "open_sync") == 0)
	{
		new_sync_method = SYNC_METHOD_OPEN;
		new_sync_bit = OPEN_SYNC_FLAG;
	}
#endif
#ifdef OPEN_DATASYNC_FLAG
	else if (pg_strcasecmp(method, "open_datasync") == 0)
	{
		new_sync_method = SYNC_METHOD_OPEN;
		new_sync_bit = OPEN_DATASYNC_FLAG;
	}
#endif
	else
		return NULL;

	if (!doit)
		return method;

	if (sync_method != new_sync_method || open_sync_bit != new_sync_bit)
	{
		/*
		 * To ensure that no blocks escape unsynced, force an fsync on the
		 * currently open log segment (if any).  Also, if the open flag is
		 * changing, close the log file so it will be reopened (with new
		 * flag bit) at next use.
		 */
		if (openLogFile >= 0)
		{
			if (pg_fsync(openLogFile) != 0)
				ereport(PANIC,
						(errcode_for_file_access(),
					errmsg("could not fsync log file %u, segment %u: %m",
						   openLogId, openLogSeg)));
			if (open_sync_bit != new_sync_bit)
			{
				if (close(openLogFile))
					ereport(PANIC,
							(errcode_for_file_access(),
					errmsg("could not close log file %u, segment %u: %m",
						   openLogId, openLogSeg)));
				openLogFile = -1;
			}
		}
		sync_method = new_sync_method;
		open_sync_bit = new_sync_bit;
	}

	return method;
}


/*
 * Issue appropriate kind of fsync (if any) on the current XLOG output file
 */
static void
issue_xlog_fsync(void)
{
	switch (sync_method)
	{
		case SYNC_METHOD_FSYNC:
			if (pg_fsync(openLogFile) != 0)
				ereport(PANIC,
						(errcode_for_file_access(),
					errmsg("could not fsync log file %u, segment %u: %m",
						   openLogId, openLogSeg)));
			break;
#ifdef HAVE_FDATASYNC
		case SYNC_METHOD_FDATASYNC:
			if (pg_fdatasync(openLogFile) != 0)
				ereport(PANIC,
						(errcode_for_file_access(),
				errmsg("could not fdatasync log file %u, segment %u: %m",
					   openLogId, openLogSeg)));
			break;
#endif
		case SYNC_METHOD_OPEN:
			/* write synced it already */
			break;
		default:
			elog(PANIC, "unrecognized wal_sync_method: %d", sync_method);
			break;
	}
}


/*
 * pg_start_backup: set up for taking an on-line backup dump
 *
 * Essentially what this does is to create a backup label file in $PGDATA,
 * where it will be archived as part of the backup dump.  The label file
 * contains the user-supplied label string (typically this would be used
 * to tell where the backup dump will be stored) and the starting time and
 * starting WAL offset for the dump.
 */
Datum
pg_start_backup(PG_FUNCTION_ARGS)
{
	text	   *backupid = PG_GETARG_TEXT_P(0);
	text	   *result;
	char	   *backupidstr;
	XLogRecPtr	checkpointloc;
	XLogRecPtr	startpoint;
	time_t		stamp_time;
	char		strfbuf[128];
	char		labelfilepath[MAXPGPATH];
	char		xlogfilename[MAXFNAMELEN];
	uint32		_logId;
	uint32		_logSeg;
	struct stat stat_buf;
	FILE	   *fp;

	if (!superuser())
		ereport(ERROR,
				(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
				 (errmsg("must be superuser to run a backup"))));

	if (!XLogArchivingActive())
		ereport(ERROR,
				(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				 (errmsg("WAL archiving is not active"),
				 (errhint("archive_command must be defined before "
						  "online backups can be made safely.")))));

	backupidstr = DatumGetCString(DirectFunctionCall1(textout,
											 PointerGetDatum(backupid)));

	/*
	 * Force a CHECKPOINT.	This is not strictly necessary, but it seems
	 * like a good idea to minimize the amount of past WAL needed to use
	 * the backup.	Also, this guarantees that two successive backup runs
	 * will have different checkpoint positions and hence different
	 * history file names, even if nothing happened in between.
	 */
	RequestCheckpoint(true);

	/*
	 * Now we need to fetch the checkpoint record location, and also its
	 * REDO pointer.  The oldest point in WAL that would be needed to
	 * restore starting from the checkpoint is precisely the REDO pointer.
	 */
	LWLockAcquire(ControlFileLock, LW_EXCLUSIVE);
	checkpointloc = ControlFile->checkPoint;
	startpoint = ControlFile->checkPointCopy.redo;
	LWLockRelease(ControlFileLock);

	XLByteToSeg(startpoint, _logId, _logSeg);
	XLogFileName(xlogfilename, ThisTimeLineID, _logId, _logSeg);

	/*
	 * We deliberately use strftime/localtime not the src/timezone
	 * functions, so that backup labels will consistently be recorded in
	 * the same timezone regardless of TimeZone setting.  This matches
	 * elog.c's practice.
	 */
	stamp_time = time(NULL);
	strftime(strfbuf, sizeof(strfbuf),
			 "%Y-%m-%d %H:%M:%S %Z",
			 localtime(&stamp_time));

	/*
	 * Check for existing backup label --- implies a backup is already
	 * running
	 */
	snprintf(labelfilepath, MAXPGPATH, "%s/backup_label", DataDir);
	if (stat(labelfilepath, &stat_buf) != 0)
	{
		if (errno != ENOENT)
			ereport(ERROR,
					(errcode_for_file_access(),
					 errmsg("could not stat file \"%s\": %m",
							labelfilepath)));
	}
	else
		ereport(ERROR,
				(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				 errmsg("a backup is already in progress"),
				 errhint("If you're sure there is no backup in progress, remove file \"%s\" and try again.",
						 labelfilepath)));

	/*
	 * Okay, write the file
	 */
	fp = AllocateFile(labelfilepath, "w");
	if (!fp)
		ereport(ERROR,
				(errcode_for_file_access(),
				 errmsg("could not create file \"%s\": %m",
						labelfilepath)));
	fprintf(fp, "START WAL LOCATION: %X/%X (file %s)\n",
			startpoint.xlogid, startpoint.xrecoff, xlogfilename);
	fprintf(fp, "CHECKPOINT LOCATION: %X/%X\n",
			checkpointloc.xlogid, checkpointloc.xrecoff);
	fprintf(fp, "START TIME: %s\n", strfbuf);
	fprintf(fp, "LABEL: %s\n", backupidstr);
	if (fflush(fp) || ferror(fp) || FreeFile(fp))
		ereport(ERROR,
				(errcode_for_file_access(),
				 errmsg("could not write file \"%s\": %m",
						labelfilepath)));

	/*
	 * We're done.  As a convenience, return the starting WAL offset.
	 */
	snprintf(xlogfilename, sizeof(xlogfilename), "%X/%X",
			 startpoint.xlogid, startpoint.xrecoff);
	result = DatumGetTextP(DirectFunctionCall1(textin,
										 CStringGetDatum(xlogfilename)));
	PG_RETURN_TEXT_P(result);
}

/*
 * pg_stop_backup: finish taking an on-line backup dump
 *
 * We remove the backup label file created by pg_start_backup, and instead
 * create a backup history file in pg_xlog (whence it will immediately be
 * archived).  The backup history file contains the same info found in
 * the label file, plus the backup-end time and WAL offset.
 */
Datum
pg_stop_backup(PG_FUNCTION_ARGS)
{
	text	   *result;
	XLogCtlInsert *Insert = &XLogCtl->Insert;
	XLogRecPtr	startpoint;
	XLogRecPtr	stoppoint;
	time_t		stamp_time;
	char		strfbuf[128];
	char		labelfilepath[MAXPGPATH];
	char		histfilepath[MAXPGPATH];
	char		startxlogfilename[MAXFNAMELEN];
	char		stopxlogfilename[MAXFNAMELEN];
	uint32		_logId;
	uint32		_logSeg;
	FILE	   *lfp;
	FILE	   *fp;
	char		ch;
	int			ich;

	if (!superuser())
		ereport(ERROR,
				(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
				 (errmsg("must be superuser to run a backup"))));

	/*
	 * Get the current end-of-WAL position; it will be unsafe to use this
	 * dump to restore to a point in advance of this time.
	 */
	LWLockAcquire(WALInsertLock, LW_EXCLUSIVE);
	INSERT_RECPTR(stoppoint, Insert, Insert->curridx);
	LWLockRelease(WALInsertLock);

	XLByteToSeg(stoppoint, _logId, _logSeg);
	XLogFileName(stopxlogfilename, ThisTimeLineID, _logId, _logSeg);

	/*
	 * We deliberately use strftime/localtime not the src/timezone
	 * functions, so that backup labels will consistently be recorded in
	 * the same timezone regardless of TimeZone setting.  This matches
	 * elog.c's practice.
	 */
	stamp_time = time(NULL);
	strftime(strfbuf, sizeof(strfbuf),
			 "%Y-%m-%d %H:%M:%S %Z",
			 localtime(&stamp_time));

	/*
	 * Open the existing label file
	 */
	snprintf(labelfilepath, MAXPGPATH, "%s/backup_label", DataDir);
	lfp = AllocateFile(labelfilepath, "r");
	if (!lfp)
	{
		if (errno != ENOENT)
			ereport(ERROR,
					(errcode_for_file_access(),
					 errmsg("could not read file \"%s\": %m",
							labelfilepath)));
		ereport(ERROR,
				(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				 errmsg("a backup is not in progress")));
	}

	/*
	 * Read and parse the START WAL LOCATION line (this code is pretty
	 * crude, but we are not expecting any variability in the file
	 * format).
	 */
	if (fscanf(lfp, "START WAL LOCATION: %X/%X (file %24s)%c",
			   &startpoint.xlogid, &startpoint.xrecoff, startxlogfilename,
			   &ch) != 4 || ch != '\n')
		ereport(ERROR,
				(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				 errmsg("invalid data in file \"%s\"", labelfilepath)));

	/*
	 * Write the backup history file
	 */
	XLByteToSeg(startpoint, _logId, _logSeg);
	BackupHistoryFilePath(histfilepath, ThisTimeLineID, _logId, _logSeg,
						  startpoint.xrecoff % XLogSegSize);
	fp = AllocateFile(histfilepath, "w");
	if (!fp)
		ereport(ERROR,
				(errcode_for_file_access(),
				 errmsg("could not create file \"%s\": %m",
						histfilepath)));
	fprintf(fp, "START WAL LOCATION: %X/%X (file %s)\n",
			startpoint.xlogid, startpoint.xrecoff, startxlogfilename);
	fprintf(fp, "STOP WAL LOCATION: %X/%X (file %s)\n",
			stoppoint.xlogid, stoppoint.xrecoff, stopxlogfilename);
	/* transfer remaining lines from label to history file */
	while ((ich = fgetc(lfp)) != EOF)
		fputc(ich, fp);
	fprintf(fp, "STOP TIME: %s\n", strfbuf);
	if (fflush(fp) || ferror(fp) || FreeFile(fp))
		ereport(ERROR,
				(errcode_for_file_access(),
				 errmsg("could not write file \"%s\": %m",
						histfilepath)));

	/*
	 * Close and remove the backup label file
	 */
	if (ferror(lfp) || FreeFile(lfp))
		ereport(ERROR,
				(errcode_for_file_access(),
				 errmsg("could not read file \"%s\": %m",
						labelfilepath)));
	if (unlink(labelfilepath) != 0)
		ereport(ERROR,
				(errcode_for_file_access(),
				 errmsg("could not remove file \"%s\": %m",
						labelfilepath)));

	/*
	 * Notify archiver that history file may be archived immediately
	 */
	if (XLogArchivingActive())
	{
		BackupHistoryFileName(histfilepath, ThisTimeLineID, _logId, _logSeg,
							  startpoint.xrecoff % XLogSegSize);
		XLogArchiveNotify(histfilepath);
	}

	/*
	 * We're done.  As a convenience, return the ending WAL offset.
	 */
	snprintf(stopxlogfilename, sizeof(stopxlogfilename), "%X/%X",
			 stoppoint.xlogid, stoppoint.xrecoff);
	result = DatumGetTextP(DirectFunctionCall1(textin,
									 CStringGetDatum(stopxlogfilename)));
	PG_RETURN_TEXT_P(result);
}

/*
 * read_backup_label: check to see if a backup_label file is present
 *
 * If we see a backup_label during recovery, we assume that we are recovering
 * from a backup dump file, and we therefore roll forward from the checkpoint
 * identified by the label file, NOT what pg_control says.	This avoids the
 * problem that pg_control might have been archived one or more checkpoints
 * later than the start of the dump, and so if we rely on it as the start
 * point, we will fail to restore a consistent database state.
 *
 * We also attempt to retrieve the corresponding backup history file.
 * If successful, set recoveryMinXlogOffset to constrain valid PITR stopping
 * points.
 *
 * Returns TRUE if a backup_label was found (and fills the checkpoint
 * location into *checkPointLoc); returns FALSE if not.
 */
static bool
read_backup_label(XLogRecPtr *checkPointLoc)
{
	XLogRecPtr	startpoint;
	XLogRecPtr	stoppoint;
	char		labelfilepath[MAXPGPATH];
	char		histfilename[MAXFNAMELEN];
	char		histfilepath[MAXPGPATH];
	char		startxlogfilename[MAXFNAMELEN];
	char		stopxlogfilename[MAXFNAMELEN];
	TimeLineID	tli;
	uint32		_logId;
	uint32		_logSeg;
	FILE	   *lfp;
	FILE	   *fp;
	char		ch;

	/*
	 * See if label file is present
	 */
	snprintf(labelfilepath, MAXPGPATH, "%s/backup_label", DataDir);
	lfp = AllocateFile(labelfilepath, "r");
	if (!lfp)
	{
		if (errno != ENOENT)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not read file \"%s\": %m",
							labelfilepath)));
		return false;			/* it's not there, all is fine */
	}

	/*
	 * Read and parse the START WAL LOCATION and CHECKPOINT lines (this
	 * code is pretty crude, but we are not expecting any variability in
	 * the file format).
	 */
	if (fscanf(lfp, "START WAL LOCATION: %X/%X (file %08X%16s)%c",
			   &startpoint.xlogid, &startpoint.xrecoff, &tli,
			   startxlogfilename, &ch) != 5 || ch != '\n')
		ereport(FATAL,
				(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				 errmsg("invalid data in file \"%s\"", labelfilepath)));
	if (fscanf(lfp, "CHECKPOINT LOCATION: %X/%X%c",
			   &checkPointLoc->xlogid, &checkPointLoc->xrecoff,
			   &ch) != 3 || ch != '\n')
		ereport(FATAL,
				(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				 errmsg("invalid data in file \"%s\"", labelfilepath)));
	if (ferror(lfp) || FreeFile(lfp))
		ereport(FATAL,
				(errcode_for_file_access(),
				 errmsg("could not read file \"%s\": %m",
						labelfilepath)));

	/*
	 * Try to retrieve the backup history file (no error if we can't)
	 */
	XLByteToSeg(startpoint, _logId, _logSeg);
	BackupHistoryFileName(histfilename, tli, _logId, _logSeg,
						  startpoint.xrecoff % XLogSegSize);

	if (InArchiveRecovery)
		RestoreArchivedFile(histfilepath, histfilename, "RECOVERYHISTORY", 0);
	else
		BackupHistoryFilePath(histfilepath, tli, _logId, _logSeg,
							  startpoint.xrecoff % XLogSegSize);

	fp = AllocateFile(histfilepath, "r");
	if (fp)
	{
		/*
		 * Parse history file to identify stop point.
		 */
		if (fscanf(fp, "START WAL LOCATION: %X/%X (file %24s)%c",
			  &startpoint.xlogid, &startpoint.xrecoff, startxlogfilename,
				   &ch) != 4 || ch != '\n')
			ereport(FATAL,
					(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				   errmsg("invalid data in file \"%s\"", histfilename)));
		if (fscanf(fp, "STOP WAL LOCATION: %X/%X (file %24s)%c",
				 &stoppoint.xlogid, &stoppoint.xrecoff, stopxlogfilename,
				   &ch) != 4 || ch != '\n')
			ereport(FATAL,
					(errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
				   errmsg("invalid data in file \"%s\"", histfilename)));
		recoveryMinXlogOffset = stoppoint;
		if (ferror(fp) || FreeFile(fp))
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not read file \"%s\": %m",
							histfilepath)));
	}

	return true;
}

/*
 * remove_backup_label: remove any extant backup_label after successful
 * recovery.  Once we have completed the end-of-recovery checkpoint there
 * is no reason to have to replay from the start point indicated by the
 * label (and indeed we'll probably have removed/recycled the needed WAL
 * segments), so remove the label to prevent trouble in later crash recoveries.
 */
static void
remove_backup_label(void)
{
	char		labelfilepath[MAXPGPATH];

	snprintf(labelfilepath, MAXPGPATH, "%s/backup_label", DataDir);
	if (unlink(labelfilepath) != 0)
		if (errno != ENOENT)
			ereport(FATAL,
					(errcode_for_file_access(),
					 errmsg("could not remove file \"%s\": %m",
							labelfilepath)));
}
