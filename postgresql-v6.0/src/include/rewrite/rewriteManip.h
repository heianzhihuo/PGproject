/*-------------------------------------------------------------------------
 *
 * rewriteManip.h--
 *    
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: rewriteManip.h,v 1.1 1996/08/28 07:24:08 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef	REWRITEMANIP_H
#define	REWRITEMANIP_H

/* RewriteManip.c */
void OffsetVarNodes(Node *node, int offset);
void ChangeVarNodes(Node *node, int old_varno, int new_varno);
void AddQual(Query *parsetree, Node *qual);
void AddNotQual(Query *parsetree, Node *qual);
void FixResdomTypes(List *user_tlist);
void FixNew(RewriteInfo *info, Query *parsetree);

void HandleRIRAttributeRule(Query *parsetree, List *rtable, List *targetlist,
			    int rt_index, int attr_num, int *modified,
			    int *badpostquel);
void HandleViewRule(Query *parsetree, List *rtable, List *targetlist,
		    int rt_index, int *modified);

#endif	/* REWRITEMANIP_H */

