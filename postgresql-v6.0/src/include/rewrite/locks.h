/*-------------------------------------------------------------------------
 *
 * locks.h--
 *    
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: locks.h,v 1.3 1996/11/08 06:02:00 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef	LOCKS_H
#define	LOCKS_H

#include "nodes/nodes.h"
#include "nodes/parsenodes.h"
#include "rewrite/prs2lock.h"

extern List *matchLocks(CmdType event, RuleLock *rulelocks, int varno,
			Query *parsetree);

#endif	/* LOCKS_H */
