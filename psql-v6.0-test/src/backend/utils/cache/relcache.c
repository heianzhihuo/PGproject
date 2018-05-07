
#include <stdio.h>		/* for sprintf() */
#include <errno.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>
 
#include "postgres.h"
#include "miscadmin.h"

#include <storage/smgr.h>

#include "access/skey.h"
#include "access/tupdesc.h"

#include "storage/buf.h"
#include "storage/fd.h"		/* for SEEK_ */
#include "storage/lmgr.h"
#include "storage/bufmgr.h"

#include "utils/memutils.h"
#include "utils/mcxt.h"
#include "utils/rel.h"
#include "utils/hsearch.h"
#include "utils/relcache.h"


#include "catalog/catname.h"


/* ----------------
 *	global variables
 *
 * 	Relations are cached two ways, by name and by id,
 *	thus there are two hash tables for referencing them. 
 * ----------------
 */
HTAB 	*RelationNameCache;
HTAB	*RelationIdCache;

/* ----------------
 *	RelationBuildDescInfo exists so code can be shared
 *      between RelationIdGetRelation() and RelationNameGetRelation()
 * ----------------
 */
typedef struct RelationBuildDescInfo {
    int infotype;		/* lookup by id or by name */
#define INFO_RELID 1
#define INFO_RELNAME 2
    union {
	Oid info_id;	/* relation object id */
	char *info_name;	/* relation name */
    } i;
} RelationBuildDescInfo;

typedef struct relidcacheent {
    Oid reloid;
    Relation reldesc;
} RelIdCacheEnt;

typedef struct relnamecacheent {
    NameData relname;
    Relation reldesc;
} RelNameCacheEnt;

/* -----------------
 *	macros to manipulate name cache and id cache
 * -----------------
 */
#define RelationCacheInsert(RELATION)	\
    {   RelIdCacheEnt *idhentry; RelNameCacheEnt *namehentry; \
	char *relname; Oid reloid; bool found; \
	relname = (RELATION->rd_rel->relname).data; \
	namehentry = (RelNameCacheEnt*)hash_search(RelationNameCache, \
					           relname, \
						   HASH_ENTER, \
						   &found); \
	if (namehentry == NULL) { \
	    elog(FATAL, "can't insert into relation descriptor cache"); \
	  } \
	if (found && !IsBootstrapProcessingMode()) { \
	    /* used to give notice -- now just keep quiet */ ; \
	  } \
	namehentry->reldesc = RELATION; \
	reloid = RELATION->rd_id; \
	idhentry = (RelIdCacheEnt*)hash_search(RelationIdCache, \
					       (char *)&reloid, \
					       HASH_ENTER, \
					       &found); \
	if (idhentry == NULL) { \
	    elog(FATAL, "can't insert into relation descriptor cache"); \
	  } \
	if (found && !IsBootstrapProcessingMode()) { \
	    /* used to give notice -- now just keep quiet */ ; \
	  } \
	idhentry->reldesc = RELATION; \
    }
#define RelationNameCacheLookup(NAME, RELATION)	\
    {   RelNameCacheEnt *hentry; bool found; \
	hentry = (RelNameCacheEnt*)hash_search(RelationNameCache, \
					       (char *)NAME,HASH_FIND,&found); \
	if (hentry == NULL) { \
	    elog(FATAL, "error in CACHE"); \
	  } \
	if (found) { \
	    RELATION = hentry->reldesc; \
	  } \
	else { \
	    RELATION = NULL; \
	  } \
    }
#define RelationIdCacheLookup(ID, RELATION)	\
    {   RelIdCacheEnt *hentry; bool found; \
	hentry = (RelIdCacheEnt*)hash_search(RelationIdCache, \
					     (char *)&(ID),HASH_FIND, &found); \
	if (hentry == NULL) { \
	    elog(FATAL, "error in CACHE"); \
	  } \
	if (found) { \
	    RELATION = hentry->reldesc; \
	  } \
	else { \
	    RELATION = NULL; \
	  } \
    }
#define RelationCacheDelete(RELATION)	\
    {   RelNameCacheEnt *namehentry; RelIdCacheEnt *idhentry; \
	char *relname; Oid reloid; bool found; \
	relname = (RELATION->rd_rel->relname).data; \
	namehentry = (RelNameCacheEnt*)hash_search(RelationNameCache, \
					           relname, \
						   HASH_REMOVE, \
						   &found); \
	if (namehentry == NULL) { \
	    elog(FATAL, "can't delete from relation descriptor cache"); \
	  } \
	if (!found) { \
	    elog(NOTICE, "trying to delete a reldesc that does not exist."); \
	  } \
	reloid = RELATION->rd_id; \
	idhentry = (RelIdCacheEnt*)hash_search(RelationIdCache, \
					       (char *)&reloid, \
					       HASH_REMOVE, &found); \
	if (idhentry == NULL) { \
	    elog(FATAL, "can't delete from relation descriptor cache"); \
	  } \
	if (!found) { \
	    elog(NOTICE, "trying to delete a reldesc that does not exist."); \
	  } \
    }



/* --------------------------------
 *	RelationIdCacheGetRelation
 *
 *	only try to get the reldesc by looking up the cache
 *	do not go to the disk.  this is used by BlockPrepareFile()
 *	and RelationIdGetRelation below.
 * --------------------------------
 */
Relation
RelationIdCacheGetRelation(Oid relationId)
{
    Relation	rd;
    
    RelationIdCacheLookup(relationId, rd);
    
    if (RelationIsValid(rd)) {
	if (rd->rd_fd == -1) {
	    rd->rd_fd = smgropen(rd->rd_rel->relsmgr, rd);
	    Assert(rd->rd_fd != -1);
	}
	
	RelationIncrementReferenceCount(rd);
	RelationSetLockForDescriptorOpen(rd);
	
    }
    
    return(rd);
}