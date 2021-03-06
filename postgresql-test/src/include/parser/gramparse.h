/*-------------------------------------------------------------------------
 *
 * gramparse.h--
 *    scanner support routines.  used by both the bootstrap lexer
 * as well as the normal lexer
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: gramparse.h,v 1.2 1996/11/10 03:05:41 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef GRAMPARSE_H
#define GRAMPARSE_H		/* include once only */

/* from scan.l */
extern void init_io(void); 

/* from gram.y */
extern void parser_init(Oid *typev, int nargs); 
extern int yyparse(void);

#endif /* GRAMPARSE_H */
