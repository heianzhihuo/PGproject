/*-------------------------------------------------------------------------
 *
 * rewriteManip.h
 *		Querytree manipulation subroutines for query rewriter.
 *
 *
 * Portions Copyright (c) 1996-2005, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/rewrite/rewriteManip.h,v 1.39 2004/12/31 22:03:41 pgsql Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef REWRITEMANIP_H
#define REWRITEMANIP_H

#include "nodes/parsenodes.h"


extern void OffsetVarNodes(Node *node, int offset, int sublevels_up);
extern void ChangeVarNodes(Node *node, int old_varno, int new_varno,
			   int sublevels_up);
extern void IncrementVarSublevelsUp(Node *node, int delta_sublevels_up,
						int min_sublevels_up);

extern bool rangeTableEntry_used(Node *node, int rt_index,
					 int sublevels_up);
extern bool attribute_used(Node *node, int rt_index, int attno,
			   int sublevels_up);

extern Query *getInsertSelectQuery(Query *parsetree, Query ***subquery_ptr);

extern void AddQual(Query *parsetree, Node *qual);
extern void AddHavingQual(Query *parsetree, Node *havingQual);
extern void AddInvertedQual(Query *parsetree, Node *qual);

extern bool checkExprHasAggs(Node *node);
extern bool checkExprHasSubLink(Node *node);

extern Node *ResolveNew(Node *node, int target_varno, int sublevels_up,
		   List *target_rtable,
		   List *targetlist, int event, int update_varno);

#endif   /* REWRITEMANIP_H */
