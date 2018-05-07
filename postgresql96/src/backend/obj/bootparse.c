/* original parser id follows */
/* Int_yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140715

#define YYEMPTY        (-1)
#define Int_yyclearin      (Int_yychar = YYEMPTY)
#define Int_yyerrok        (Int_yyerrflag = 0)
#define YYRECOVERING() (Int_yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "Int_yy"

#define YYPURE 0

#line 2 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
/*-------------------------------------------------------------------------
 *
 * backendparse.y--
 *    yacc parser grammer for the "backend" initialization program.
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /usr/local/cvsroot/postgres95/src/backend/bootstrap/bootparse.y,v 1.1.1.1.2.1 1996/08/20 15:25:29 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "access/heapam.h"
#include "access/tupdesc.h"
#include "bootstrap/bootstrap.h"
#include "utils/portal.h" 
#include "storage/smgr.h" 
#include "nodes/pg_list.h"
#include "catalog/catalog.h"
#include "catalog/catname.h"
#include "catalog/heap.h"
#include "catalog/index.h"
#include "commands/rename.h"
#include "commands/defrem.h"
#include "access/transam.h"
#include "access/xact.h"

#define DO_START { StartTransactionCommand();\
		 }

#define DO_END   { CommitTransactionCommand();\
		   if (!Quiet) { EMITPROMPT; }\
		   fflush(stdout); \
		 }

int num_tuples_read = 0;
static Oid objectid;

#line 43 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    List     *list;
    IndexElem	*ielem;
    char     *str;
    int		ival;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 75 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() Int_yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() Int_yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() Int_yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() Int_yylex(void *YYLEX_PARAM)
# define YYLEX Int_yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() Int_yylex(void)
# define YYLEX Int_yylex()
#endif

/* Parameters sent to Int_yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() Int_yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) Int_yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define CONST 257
#define ID 258
#define OPEN 259
#define XCLOSE 260
#define XCREATE 261
#define INSERT_TUPLE 262
#define STRING 263
#define XDEFINE 264
#define XDECLARE 265
#define INDEX 266
#define ON 267
#define USING 268
#define XBUILD 269
#define INDICES 270
#define COMMA 271
#define EQUALS 272
#define LPAREN 273
#define RPAREN 274
#define OBJ_ID 275
#define XBOOTSTRAP 276
#define NULLVAL 277
#define low 278
#define high 279
#define YYERRCODE 256
typedef short YYINT;
static const YYINT Int_yylhs[] = {                           -1,
    0,    0,   10,   10,   11,   11,   11,   11,   11,   11,
   12,   13,   13,   19,   20,   14,   21,   15,   16,   17,
    2,    3,    3,    1,    1,    6,    6,   18,   18,   22,
    7,    7,    9,    9,    9,    8,    8,    8,    4,    5,
};
static const YYINT Int_yylen[] = {                            2,
    1,    0,    1,    2,    1,    1,    1,    1,    1,    1,
    2,    2,    1,    0,    0,    8,    0,    6,   10,    2,
    2,    1,    4,    1,    3,    1,    0,    1,    3,    3,
    3,    0,    1,    2,    3,    1,    1,    1,    1,    1,
};
static const YYINT Int_yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    3,    5,
    6,    7,    8,    9,   10,   40,   11,   12,   26,    0,
    0,   17,    0,   20,    4,    0,    0,    0,    0,   14,
   31,    0,    0,    0,   39,   38,   37,   36,   33,    0,
    0,    0,    0,   28,    0,   18,   34,    0,    0,    0,
    0,   35,    0,   30,   29,   16,    0,    0,    0,    0,
   19,   21,    0,    0,   24,    0,   23,   25,
};
static const YYINT Int_yydgoto[] = {                          7,
   64,   58,   59,   37,   38,   20,   22,   39,   40,    8,
    9,   10,   11,   12,   13,   14,   15,   43,   34,   51,
   28,   44,
};
static const YYINT Int_yysindex[] = {                      -248,
 -242, -242, -274, -265, -247, -246,    0, -248,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -242,
 -245,    0, -242,    0,    0, -243, -242, -241, -236,    0,
    0, -249, -242, -242,    0,    0,    0,    0,    0, -251,
 -235, -238, -234,    0, -249,    0,    0, -242, -242, -242,
 -233,    0, -231,    0,    0,    0, -242, -230, -242, -228,
    0,    0, -242, -256,    0, -242,    0,    0,
};
static const YYINT Int_yyrindex[] = {                        38,
    0,    1, -219, -241,    0,    0,    0,   40,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -227,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -215,
    0,    0,    0,    0,    0,    0,    0,    0,
};
static const YYINT Int_yygindex[] = {                         0,
    0,    0,    0,    0,    2,    0,    0,  -40,    0,    0,
   41,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   -4,
};
#define YYTABLESIZE 270
static const YYINT Int_yytable[] = {                         47,
   13,   19,   17,   18,   52,   35,   16,   35,   16,   21,
    1,    2,    3,    4,   66,   16,    5,   67,   23,   45,
    6,   26,   46,   24,   29,   36,   27,   36,   31,   30,
   33,   32,   48,   49,   41,   42,   50,    2,   27,    1,
   56,   57,   22,   61,   63,   55,   15,    0,   25,   53,
   54,   42,    0,    0,    0,    0,    0,    0,   60,    0,
   62,    0,    0,    0,   65,    0,    0,   68,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   13,
   13,   13,   13,    0,    0,   13,    0,    0,    0,   13,
};
static const YYINT Int_yycheck[] = {                         40,
    0,  276,    1,    2,   45,  257,  258,  257,  258,  275,
  259,  260,  261,  262,  271,  258,  265,  274,  266,  271,
  269,   20,  274,  270,   23,  277,  272,  277,   27,  273,
  267,  273,  268,  272,   33,   34,  271,    0,  258,    0,
  274,  273,  258,  274,  273,   50,  274,   -1,    8,   48,
   49,   50,   -1,   -1,   -1,   -1,   -1,   -1,   57,   -1,
   59,   -1,   -1,   -1,   63,   -1,   -1,   66,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  259,
  260,  261,  262,   -1,   -1,  265,   -1,   -1,   -1,  269,
};
#define YYFINAL 7
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 279
#define YYUNDFTOKEN 304
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const Int_yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"CONST","ID","OPEN","XCLOSE",
"XCREATE","INSERT_TUPLE","STRING","XDEFINE","XDECLARE","INDEX","ON","USING",
"XBUILD","INDICES","COMMA","EQUALS","LPAREN","RPAREN","OBJ_ID","XBOOTSTRAP",
"NULLVAL","low","high",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"illegal-symbol",
};
static const char *const Int_yyrule[] = {
"$accept : TopLevel",
"TopLevel : Queries",
"TopLevel :",
"Queries : Query",
"Queries : Queries Query",
"Query : OpenStmt",
"Query : CloseStmt",
"Query : CreateStmt",
"Query : InsertStmt",
"Query : DeclareIndexStmt",
"Query : BuildIndsStmt",
"OpenStmt : OPEN ident",
"CloseStmt : XCLOSE ident",
"CloseStmt : XCLOSE",
"$$1 :",
"$$2 :",
"CreateStmt : XCREATE optbootstrap ident LPAREN $$1 typelist $$2 RPAREN",
"$$3 :",
"InsertStmt : INSERT_TUPLE optoideq $$3 LPAREN tuplelist RPAREN",
"DeclareIndexStmt : XDECLARE INDEX ident ON ident USING ident LPAREN index_params RPAREN",
"BuildIndsStmt : XBUILD INDICES",
"index_params : index_on ident",
"index_on : ident",
"index_on : ident LPAREN arg_list RPAREN",
"arg_list : ident",
"arg_list : arg_list COMMA ident",
"optbootstrap : XBOOTSTRAP",
"optbootstrap :",
"typelist : typething",
"typelist : typelist COMMA typething",
"typething : ident EQUALS ident",
"optoideq : OBJ_ID EQUALS ident",
"optoideq :",
"tuplelist : tuple",
"tuplelist : tuplelist tuple",
"tuplelist : tuplelist COMMA tuple",
"tuple : ident",
"tuple : const",
"tuple : NULLVAL",
"const : CONST",
"ident : ID",

};
#endif

int      Int_yydebug;
int      Int_yynerrs;

int      Int_yyerrflag;
int      Int_yychar;
YYSTYPE  Int_yyval;
YYSTYPE  Int_yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA Int_yystack;
#line 292 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"


#line 345 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int Int_yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void Int_yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define Int_yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto Int_yyabort
#define YYREJECT goto Int_yyabort
#define YYACCEPT goto Int_yyaccept
#define YYERROR  goto Int_yyerrlab

int
YYPARSE_DECL()
{
    int Int_yym, Int_yyn, Int_yystate;
#if YYDEBUG
    const char *Int_yys;

    if ((Int_yys = getenv("YYDEBUG")) != 0)
    {
        Int_yyn = *Int_yys;
        if (Int_yyn >= '0' && Int_yyn <= '9')
            Int_yydebug = Int_yyn - '0';
    }
#endif

    Int_yynerrs = 0;
    Int_yyerrflag = 0;
    Int_yychar = YYEMPTY;
    Int_yystate = 0;

#if YYPURE
    memset(&Int_yystack, 0, sizeof(Int_yystack));
#endif

    if (Int_yystack.s_base == NULL && Int_yygrowstack(&Int_yystack) == YYENOMEM) goto Int_yyoverflow;
    Int_yystack.s_mark = Int_yystack.s_base;
    Int_yystack.l_mark = Int_yystack.l_base;
    Int_yystate = 0;
    *Int_yystack.s_mark = 0;

Int_yyloop:
    if ((Int_yyn = Int_yydefred[Int_yystate]) != 0) goto Int_yyreduce;
    if (Int_yychar < 0)
    {
        if ((Int_yychar = YYLEX) < 0) Int_yychar = YYEOF;
#if YYDEBUG
        if (Int_yydebug)
        {
            Int_yys = Int_yyname[YYTRANSLATE(Int_yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, Int_yystate, Int_yychar, Int_yys);
        }
#endif
    }
    if ((Int_yyn = Int_yysindex[Int_yystate]) && (Int_yyn += Int_yychar) >= 0 &&
            Int_yyn <= YYTABLESIZE && Int_yycheck[Int_yyn] == Int_yychar)
    {
#if YYDEBUG
        if (Int_yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, Int_yystate, Int_yytable[Int_yyn]);
#endif
        if (Int_yystack.s_mark >= Int_yystack.s_last && Int_yygrowstack(&Int_yystack) == YYENOMEM)
        {
            goto Int_yyoverflow;
        }
        Int_yystate = Int_yytable[Int_yyn];
        *++Int_yystack.s_mark = Int_yytable[Int_yyn];
        *++Int_yystack.l_mark = Int_yylval;
        Int_yychar = YYEMPTY;
        if (Int_yyerrflag > 0)  --Int_yyerrflag;
        goto Int_yyloop;
    }
    if ((Int_yyn = Int_yyrindex[Int_yystate]) && (Int_yyn += Int_yychar) >= 0 &&
            Int_yyn <= YYTABLESIZE && Int_yycheck[Int_yyn] == Int_yychar)
    {
        Int_yyn = Int_yytable[Int_yyn];
        goto Int_yyreduce;
    }
    if (Int_yyerrflag) goto Int_yyinrecovery;

    YYERROR_CALL("syntax error");

    goto Int_yyerrlab;

Int_yyerrlab:
    ++Int_yynerrs;

Int_yyinrecovery:
    if (Int_yyerrflag < 3)
    {
        Int_yyerrflag = 3;
        for (;;)
        {
            if ((Int_yyn = Int_yysindex[*Int_yystack.s_mark]) && (Int_yyn += YYERRCODE) >= 0 &&
                    Int_yyn <= YYTABLESIZE && Int_yycheck[Int_yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (Int_yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *Int_yystack.s_mark, Int_yytable[Int_yyn]);
#endif
                if (Int_yystack.s_mark >= Int_yystack.s_last && Int_yygrowstack(&Int_yystack) == YYENOMEM)
                {
                    goto Int_yyoverflow;
                }
                Int_yystate = Int_yytable[Int_yyn];
                *++Int_yystack.s_mark = Int_yytable[Int_yyn];
                *++Int_yystack.l_mark = Int_yylval;
                goto Int_yyloop;
            }
            else
            {
#if YYDEBUG
                if (Int_yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *Int_yystack.s_mark);
#endif
                if (Int_yystack.s_mark <= Int_yystack.s_base) goto Int_yyabort;
                --Int_yystack.s_mark;
                --Int_yystack.l_mark;
            }
        }
    }
    else
    {
        if (Int_yychar == YYEOF) goto Int_yyabort;
#if YYDEBUG
        if (Int_yydebug)
        {
            Int_yys = Int_yyname[YYTRANSLATE(Int_yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, Int_yystate, Int_yychar, Int_yys);
        }
#endif
        Int_yychar = YYEMPTY;
        goto Int_yyloop;
    }

Int_yyreduce:
#if YYDEBUG
    if (Int_yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, Int_yystate, Int_yyn, Int_yyrule[Int_yyn]);
#endif
    Int_yym = Int_yylen[Int_yyn];
    if (Int_yym)
        Int_yyval = Int_yystack.l_mark[1-Int_yym];
    else
        memset(&Int_yyval, 0, sizeof Int_yyval);
    switch (Int_yyn)
    {
case 11:
#line 89 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ 
		    DO_START;
		    boot_openrel(LexIDStr(Int_yystack.l_mark[0].ival));
		    DO_END; 
		}
break;
case 12:
#line 98 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		    DO_START;
		    closerel(LexIDStr(Int_yystack.l_mark[0].ival));
		    DO_END;
		}
break;
case 13:
#line 104 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		    DO_START;
		    closerel(NULL);
		    DO_END;
		}
break;
case 14:
#line 113 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ 
		    DO_START; 
		    numattr=(int)0;
		}
break;
case 15:
#line 118 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ 
		    if (!Quiet) putchar('\n');
		    DO_END;
		}
break;
case 16:
#line 123 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ 
		    DO_START; 

		    if (Int_yystack.l_mark[-6].ival) {
			extern Relation reldesc;
			TupleDesc tupdesc;

			if (reldesc) {
			    puts("create bootstrap: Warning, open relation");
			    puts("exists, closing first");
			    closerel(NULL);
			}
			if (DebugMode)
			    puts("creating bootstrap relation");
			tupdesc = CreateTupleDesc(numattr,attrtypes);
			reldesc = heap_creatr(LexIDStr(Int_yystack.l_mark[-5].ival),
					      DEFAULT_SMGR,
					      tupdesc);
			if (DebugMode)
			    puts("bootstrap relation created ok");
		    } else {
			Oid id;
			TupleDesc tupdesc;
			/* extern Oid heap_create();*/

			tupdesc = CreateTupleDesc(numattr,attrtypes);
			id = heap_create(LexIDStr(Int_yystack.l_mark[-5].ival),
					 NULL,
					 'n',
					 DEFAULT_SMGR,
					 tupdesc);
			if (!Quiet)
			    printf("CREATED relation %s with OID %d\n",
				   LexIDStr(Int_yystack.l_mark[-5].ival), id);
		    }
		    DO_END;
		    if (DebugMode)
			puts("Commit End");
		}
break;
case 17:
#line 166 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ 
		    DO_START;
		    if (DebugMode)
		        printf("tuple %d<", Int_yystack.l_mark[0].ival);
		    num_tuples_read = 0;
		}
break;
case 18:
#line 173 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		    if (num_tuples_read != numattr)
		        elog(WARN,"incorrect number of values for tuple");
		    if (reldesc == (Relation)NULL) {
		        elog(WARN,"must OPEN RELATION before INSERT\n");
		        err_out();
		    }
		    if (DebugMode)
			puts("Insert Begin");
		    objectid = Int_yystack.l_mark[-4].ival;
		    InsertOneTuple(objectid);
		    if (DebugMode)
			puts("Insert End");
		    if (!Quiet) { putchar('\n'); }
		    DO_END;
		    if (DebugMode)
			puts("Transaction End");
		}
break;
case 19:
#line 195 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		  List *params;

		  DO_START;

		  params = lappend(NIL, (List*)Int_yystack.l_mark[-1].ielem);
		  DefineIndex(LexIDStr(Int_yystack.l_mark[-5].ival), 
			      LexIDStr(Int_yystack.l_mark[-7].ival), 
			      LexIDStr(Int_yystack.l_mark[-3].ival),
			      params, NIL, 0, NIL);
		  DO_END;
		}
break;
case 20:
#line 210 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ build_indices(); }
break;
case 21:
#line 214 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		    IndexElem *n = (IndexElem*)Int_yystack.l_mark[-1].ielem;
		    n->class = LexIDStr(Int_yystack.l_mark[0].ival);
		    Int_yyval.ielem = n;
		}
break;
case 22:
#line 222 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		    IndexElem *n = makeNode(IndexElem);
		    n->name = LexIDStr(Int_yystack.l_mark[0].ival);
		    Int_yyval.ielem = n;
		}
break;
case 23:
#line 228 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		    IndexElem *n = makeNode(IndexElem);
		    n->name = LexIDStr(Int_yystack.l_mark[-3].ival);
		    n->args = (List*)Int_yystack.l_mark[-1].list;
		    Int_yyval.ielem = n;
		}
break;
case 24:
#line 237 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		  Int_yyval.list = lappend(NIL, makeString(LexIDStr(Int_yystack.l_mark[0].ival)));
		}
break;
case 25:
#line 241 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{
		  Int_yyval.list = lappend((List*)Int_yystack.l_mark[-2].list, makeString(LexIDStr(Int_yystack.l_mark[0].ival)));
		}
break;
case 26:
#line 246 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ Int_yyval.ival = 1; }
break;
case 27:
#line 247 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ Int_yyval.ival = 0; }
break;
case 30:
#line 257 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ 
		   if(++numattr > MAXATTR)
			elog(FATAL,"Too many attributes\n");
		   DefineAttr(LexIDStr(Int_yystack.l_mark[-2].ival),LexIDStr(Int_yystack.l_mark[0].ival),numattr-1);
		   if (DebugMode)
		       printf("\n");
		}
break;
case 31:
#line 267 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ Int_yyval.ival = atol(LexIDStr(Int_yystack.l_mark[0].ival)); 		}
break;
case 32:
#line 268 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ extern Oid newoid(); Int_yyval.ival = newoid();	}
break;
case 36:
#line 278 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{InsertOneValue(objectid, LexIDStr(Int_yystack.l_mark[0].ival), num_tuples_read++); }
break;
case 37:
#line 279 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{InsertOneValue(objectid, LexIDStr(Int_yystack.l_mark[0].ival), num_tuples_read++); }
break;
case 38:
#line 281 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ InsertOneNull(num_tuples_read++); }
break;
case 39:
#line 285 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ Int_yyval.ival=Int_yylval.ival; }
break;
case 40:
#line 289 "/home/ubuntu/Postgresql/postgresql96/src/backend/bootstrap/bootparse.y"
	{ Int_yyval.ival=Int_yylval.ival; }
break;
#line 759 "y.tab.c"
    }
    Int_yystack.s_mark -= Int_yym;
    Int_yystate = *Int_yystack.s_mark;
    Int_yystack.l_mark -= Int_yym;
    Int_yym = Int_yylhs[Int_yyn];
    if (Int_yystate == 0 && Int_yym == 0)
    {
#if YYDEBUG
        if (Int_yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        Int_yystate = YYFINAL;
        *++Int_yystack.s_mark = YYFINAL;
        *++Int_yystack.l_mark = Int_yyval;
        if (Int_yychar < 0)
        {
            if ((Int_yychar = YYLEX) < 0) Int_yychar = YYEOF;
#if YYDEBUG
            if (Int_yydebug)
            {
                Int_yys = Int_yyname[YYTRANSLATE(Int_yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, Int_yychar, Int_yys);
            }
#endif
        }
        if (Int_yychar == YYEOF) goto Int_yyaccept;
        goto Int_yyloop;
    }
    if ((Int_yyn = Int_yygindex[Int_yym]) && (Int_yyn += Int_yystate) >= 0 &&
            Int_yyn <= YYTABLESIZE && Int_yycheck[Int_yyn] == Int_yystate)
        Int_yystate = Int_yytable[Int_yyn];
    else
        Int_yystate = Int_yydgoto[Int_yym];
#if YYDEBUG
    if (Int_yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *Int_yystack.s_mark, Int_yystate);
#endif
    if (Int_yystack.s_mark >= Int_yystack.s_last && Int_yygrowstack(&Int_yystack) == YYENOMEM)
    {
        goto Int_yyoverflow;
    }
    *++Int_yystack.s_mark = (YYINT) Int_yystate;
    *++Int_yystack.l_mark = Int_yyval;
    goto Int_yyloop;

Int_yyoverflow:
    YYERROR_CALL("yacc stack overflow");

Int_yyabort:
    Int_yyfreestack(&Int_yystack);
    return (1);

Int_yyaccept:
    Int_yyfreestack(&Int_yystack);
    return (0);
}
