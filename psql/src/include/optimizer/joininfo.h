/*-------------------------------------------------------------------------
 *
 * joininfo.h--
 *    prototypes for joininfo.c.
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: joininfo.h,v 1.1 1996/08/28 07:23:14 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef JOININFO_H
#define JOININFO_H

extern JInfo *joininfo_member(List *join_relids, List *joininfo_list);
extern JInfo *find_joininfo_node(Rel *this_rel, List *join_relids);
extern Var *other_join_clause_var(Var *var, Expr *clause);

#endif	/* JOININFO_H */
