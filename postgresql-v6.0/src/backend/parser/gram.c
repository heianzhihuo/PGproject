/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "gram.y" /* yacc.c:339  */
 /* -*-text-*- */

/*#define YYDEBUG 1*/
/*-------------------------------------------------------------------------
 * 
 * gram.y--
 *    POSTGRES SQL YACC rules/actions
 * 
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /usr/local/cvsroot/postgres95/src/backend/parser/gram.y,v 1.25 1997/01/16 14:56:05 momjian Exp $
 *
 * HISTORY
 *    AUTHOR		DATE		MAJOR EVENT
 *    Andrew Yu		Sept, 1994	POSTQUEL to SQL conversion
 *    Andrew Yu		Oct, 1994	lispy code conversion
 *
 * NOTES
 *    CAPITALS are used to represent terminal symbols.
 *    non-capitals are used to represent non-terminals.
 *
 *    if you use list, make sure the datum is a node so that the printing
 *    routines work
 *
 * WARNING
 *    sometimes we assign constants to makeStrings. Make sure we don't free
 *    those.
 *
 *-------------------------------------------------------------------------
 */
#include <string.h>
#include <ctype.h>

#include "postgres.h"
#include "nodes/parsenodes.h"
#include "parser/gramparse.h"
#include "parser/catalog_utils.h"
#include "parser/parse_query.h"
#include "storage/smgr.h"
#include "utils/acl.h"
#include "catalog/catname.h"
#include "utils/elog.h"
#include "access/xact.h"

static char saved_relname[NAMEDATALEN];  /* need this for complex attributes */
static bool QueryIsRule = FALSE;
static Node *saved_In_Expr;
extern List *parsetree;

/*
 * If you need access to certain yacc-generated variables and find that 
 * they're static by default, uncomment the next line.  (this is not a
 * problem, yet.)
 */
/*#define __YYSCLASS*/

static char *xlateSqlType(char *);
static Node *makeA_Expr(int oper, char *opname, Node *lexpr, Node *rexpr);

/* old versions of flex define this as a macro */
#if defined(yywrap)
#undef yywrap
#endif /* yywrap */

#line 133 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ABORT_TRANS = 258,
    ACL = 259,
    ADD = 260,
    AFTER = 261,
    AGGREGATE = 262,
    ALL = 263,
    ALTER = 264,
    AND = 265,
    APPEND = 266,
    ARCHIVE = 267,
    ARCH_STORE = 268,
    AS = 269,
    ASC = 270,
    BACKWARD = 271,
    BEFORE = 272,
    BEGIN_TRANS = 273,
    BETWEEN = 274,
    BINARY = 275,
    BY = 276,
    CAST = 277,
    CHANGE = 278,
    CLOSE = 279,
    CLUSTER = 280,
    COLUMN = 281,
    COMMIT = 282,
    COPY = 283,
    CREATE = 284,
    CURRENT = 285,
    CURSOR = 286,
    DATABASE = 287,
    DECLARE = 288,
    DELETE = 289,
    DELIMITERS = 290,
    DESC = 291,
    DISTINCT = 292,
    DO = 293,
    DROP = 294,
    END_TRANS = 295,
    EXTEND = 296,
    FETCH = 297,
    FOR = 298,
    FORWARD = 299,
    FROM = 300,
    FUNCTION = 301,
    GRANT = 302,
    GROUP = 303,
    HAVING = 304,
    HEAVY = 305,
    IN = 306,
    INDEX = 307,
    INHERITS = 308,
    INSERT = 309,
    INSTEAD = 310,
    INTO = 311,
    IS = 312,
    ISNULL = 313,
    LANGUAGE = 314,
    LIGHT = 315,
    LISTEN = 316,
    LOAD = 317,
    MERGE = 318,
    MOVE = 319,
    NEW = 320,
    NONE = 321,
    NOT = 322,
    NOTHING = 323,
    NOTIFY = 324,
    NOTNULL = 325,
    OIDS = 326,
    ON = 327,
    OPERATOR = 328,
    OPTION = 329,
    OR = 330,
    ORDER = 331,
    PNULL = 332,
    PRIVILEGES = 333,
    PUBLIC = 334,
    PURGE = 335,
    P_TYPE = 336,
    RENAME = 337,
    REPLACE = 338,
    RETRIEVE = 339,
    RETURNS = 340,
    REVOKE = 341,
    ROLLBACK = 342,
    RULE = 343,
    SELECT = 344,
    SET = 345,
    SETOF = 346,
    STDIN = 347,
    STDOUT = 348,
    STORE = 349,
    TABLE = 350,
    TO = 351,
    TRANSACTION = 352,
    UNIQUE = 353,
    UPDATE = 354,
    USING = 355,
    VACUUM = 356,
    VALUES = 357,
    VERBOSE = 358,
    VERSION = 359,
    VIEW = 360,
    WHERE = 361,
    WITH = 362,
    WORK = 363,
    EXECUTE = 364,
    RECIPE = 365,
    EXPLAIN = 366,
    LIKE = 367,
    IDENT = 368,
    SCONST = 369,
    Op = 370,
    ICONST = 371,
    PARAM = 372,
    FCONST = 373,
    OP = 374,
    UMINUS = 375,
    TYPECAST = 376,
    REDUCE = 377
  };
#endif
/* Tokens.  */
#define ABORT_TRANS 258
#define ACL 259
#define ADD 260
#define AFTER 261
#define AGGREGATE 262
#define ALL 263
#define ALTER 264
#define AND 265
#define APPEND 266
#define ARCHIVE 267
#define ARCH_STORE 268
#define AS 269
#define ASC 270
#define BACKWARD 271
#define BEFORE 272
#define BEGIN_TRANS 273
#define BETWEEN 274
#define BINARY 275
#define BY 276
#define CAST 277
#define CHANGE 278
#define CLOSE 279
#define CLUSTER 280
#define COLUMN 281
#define COMMIT 282
#define COPY 283
#define CREATE 284
#define CURRENT 285
#define CURSOR 286
#define DATABASE 287
#define DECLARE 288
#define DELETE 289
#define DELIMITERS 290
#define DESC 291
#define DISTINCT 292
#define DO 293
#define DROP 294
#define END_TRANS 295
#define EXTEND 296
#define FETCH 297
#define FOR 298
#define FORWARD 299
#define FROM 300
#define FUNCTION 301
#define GRANT 302
#define GROUP 303
#define HAVING 304
#define HEAVY 305
#define IN 306
#define INDEX 307
#define INHERITS 308
#define INSERT 309
#define INSTEAD 310
#define INTO 311
#define IS 312
#define ISNULL 313
#define LANGUAGE 314
#define LIGHT 315
#define LISTEN 316
#define LOAD 317
#define MERGE 318
#define MOVE 319
#define NEW 320
#define NONE 321
#define NOT 322
#define NOTHING 323
#define NOTIFY 324
#define NOTNULL 325
#define OIDS 326
#define ON 327
#define OPERATOR 328
#define OPTION 329
#define OR 330
#define ORDER 331
#define PNULL 332
#define PRIVILEGES 333
#define PUBLIC 334
#define PURGE 335
#define P_TYPE 336
#define RENAME 337
#define REPLACE 338
#define RETRIEVE 339
#define RETURNS 340
#define REVOKE 341
#define ROLLBACK 342
#define RULE 343
#define SELECT 344
#define SET 345
#define SETOF 346
#define STDIN 347
#define STDOUT 348
#define STORE 349
#define TABLE 350
#define TO 351
#define TRANSACTION 352
#define UNIQUE 353
#define UPDATE 354
#define USING 355
#define VACUUM 356
#define VALUES 357
#define VERBOSE 358
#define VERSION 359
#define VIEW 360
#define WHERE 361
#define WITH 362
#define WORK 363
#define EXECUTE 364
#define RECIPE 365
#define EXPLAIN 366
#define LIKE 367
#define IDENT 368
#define SCONST 369
#define Op 370
#define ICONST 371
#define PARAM 372
#define FCONST 373
#define OP 374
#define UMINUS 375
#define TYPECAST 376
#define REDUCE 377

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 69 "gram.y" /* yacc.c:355  */

    double 		dval;
    int			ival;
    char                chr;
    char		*str;
    bool		boolean;
    List		*list;
    Node		*node;
    Value		*value;

    Attr		*attr;

    ColumnDef		*coldef;
    TypeName		*typnam;
    DefElem		*defelt;
    ParamString		*param;
    SortGroupBy		*sortgroupby;
    IndexElem		*ielem;
    RangeVar		*range;
    RelExpr		*relexp;
    TimeRange		*trange;
    A_Indices		*aind;
    ResTarget		*target;
    ParamNo		*paramno;
	
    VersionStmt		*vstmt;
    DefineStmt		*dstmt;
    PurgeStmt		*pstmt;
    RuleStmt		*rstmt;
    AppendStmt		*astmt;

#line 449 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 466 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  140
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1132

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  160
/* YYNRULES -- Number of rules.  */
#define YYNRULES  386
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  648

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   377

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     136,   137,   123,   121,   138,   122,   131,   124,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   126,   127,
     128,   120,   129,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   132,     2,   133,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   125,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   130,   134,   135
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   227,   227,   229,   233,   235,   237,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   280,   290,   306,
     323,   336,   338,   347,   348,   349,   352,   353,   356,   357,
     363,   364,   375,   389,   390,   394,   396,   401,   402,   405,
     406,   407,   410,   413,   416,   419,   422,   423,   434,   441,
     449,   450,   451,   454,   454,   454,   458,   462,   464,   468,
     474,   482,   483,   484,   485,   486,   503,   519,   529,   530,
     531,   534,   537,   538,   548,   556,   560,   564,   569,   572,
     579,   582,   585,   588,   591,   596,   599,   602,   607,   608,
     620,   635,   644,   655,   656,   659,   660,   674,   691,   693,
     696,   697,   707,   723,   752,   765,   766,   769,   770,   773,
     785,   792,   798,   804,   810,   817,   824,   825,   843,   852,
     853,   854,   855,   856,   859,   868,   869,   872,   881,   881,
     883,   884,   885,   886,   887,   888,   889,   892,   896,   898,
     900,   912,   924,   925,   928,   929,   943,   942,   958,   959,
     960,   963,   965,   969,   971,   973,   977,   985,   996,   997,
     998,   999,  1002,  1003,  1015,  1023,  1045,  1051,  1057,  1063,
    1069,  1075,  1082,  1088,  1094,  1101,  1107,  1123,  1140,  1156,
    1172,  1188,  1204,  1210,  1219,  1220,  1230,  1250,  1251,  1252,
    1253,  1254,  1255,  1266,  1274,  1281,  1290,  1291,  1295,  1297,
    1301,  1317,  1335,  1357,  1394,  1412,  1415,  1418,  1419,  1420,
    1423,  1424,  1427,  1429,  1433,  1441,  1449,  1459,  1460,  1461,
    1462,  1463,  1464,  1468,  1469,  1479,  1489,  1499,  1500,  1503,
    1504,  1505,  1517,  1518,  1521,  1523,  1525,  1529,  1530,  1533,
    1534,  1537,  1545,  1553,  1563,  1564,  1575,  1576,  1579,  1581,
    1585,  1591,  1597,  1605,  1606,  1609,  1617,  1625,  1636,  1642,
    1650,  1651,  1654,  1655,  1658,  1660,  1663,  1666,  1668,  1671,
    1674,  1699,  1707,  1712,  1754,  1759,  1761,  1763,  1765,  1767,
    1769,  1771,  1773,  1775,  1777,  1779,  1781,  1783,  1793,  1803,
    1805,  1807,  1809,  1811,  1813,  1815,  1822,  1833,  1840,  1847,
    1849,  1851,  1853,  1855,  1860,  1865,  1865,  1867,  1867,  1869,
    1871,  1873,  1877,  1884,  1892,  1895,  1897,  1901,  1903,  1909,
    1911,  1917,  1925,  1935,  1937,  1939,  1950,  1952,  1954,  1969,
    1976,  1983,  2003,  2005,  2010,  2017,  2024,  2036,  2050,  2051,
    2054,  2060,  2076,  2077,  2078,  2079,  2080,  2081,  2082,  2084,
    2085,  2086,  2088,  2095,  2102,  2109,  2111,  2119,  2126,  2127,
    2130,  2131,  2133,  2135,  2142,  2151,  2152
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ABORT_TRANS", "ACL", "ADD", "AFTER",
  "AGGREGATE", "ALL", "ALTER", "AND", "APPEND", "ARCHIVE", "ARCH_STORE",
  "AS", "ASC", "BACKWARD", "BEFORE", "BEGIN_TRANS", "BETWEEN", "BINARY",
  "BY", "CAST", "CHANGE", "CLOSE", "CLUSTER", "COLUMN", "COMMIT", "COPY",
  "CREATE", "CURRENT", "CURSOR", "DATABASE", "DECLARE", "DELETE",
  "DELIMITERS", "DESC", "DISTINCT", "DO", "DROP", "END_TRANS", "EXTEND",
  "FETCH", "FOR", "FORWARD", "FROM", "FUNCTION", "GRANT", "GROUP",
  "HAVING", "HEAVY", "IN", "INDEX", "INHERITS", "INSERT", "INSTEAD",
  "INTO", "IS", "ISNULL", "LANGUAGE", "LIGHT", "LISTEN", "LOAD", "MERGE",
  "MOVE", "NEW", "NONE", "NOT", "NOTHING", "NOTIFY", "NOTNULL", "OIDS",
  "ON", "OPERATOR", "OPTION", "OR", "ORDER", "PNULL", "PRIVILEGES",
  "PUBLIC", "PURGE", "P_TYPE", "RENAME", "REPLACE", "RETRIEVE", "RETURNS",
  "REVOKE", "ROLLBACK", "RULE", "SELECT", "SET", "SETOF", "STDIN",
  "STDOUT", "STORE", "TABLE", "TO", "TRANSACTION", "UNIQUE", "UPDATE",
  "USING", "VACUUM", "VALUES", "VERBOSE", "VERSION", "VIEW", "WHERE",
  "WITH", "WORK", "EXECUTE", "RECIPE", "EXPLAIN", "LIKE", "IDENT",
  "SCONST", "Op", "ICONST", "PARAM", "FCONST", "OP", "'='", "'+'", "'-'",
  "'*'", "'/'", "'|'", "':'", "';'", "'<'", "'>'", "UMINUS", "'.'", "'['",
  "']'", "TYPECAST", "REDUCE", "'('", "')'", "','", "$accept", "stmtblock",
  "stmtmulti", "stmt", "AddAttrStmt", "columnDef", "ClosePortalStmt",
  "CopyStmt", "copy_dirn", "copy_file_name", "opt_binary", "opt_with_copy",
  "copy_delimiter", "CreateStmt", "OptTableElementList",
  "tableElementList", "OptArchiveType", "archive_type", "OptLocation",
  "OptArchiveLocation", "OptInherit", "DefineStmt", "def_rest", "def_type",
  "def_name", "definition", "def_list", "def_elem", "def_arg",
  "DestroyStmt", "FetchStmt", "opt_direction", "fetch_how_many",
  "GrantStmt", "privileges", "operation_commalist", "operation", "grantee",
  "opt_with_grant", "RevokeStmt", "MoveStmt", "opt_move_where",
  "opt_portal_name", "IndexStmt", "access_method_clause",
  "index_opt_unique", "ExtendStmt", "RecipeStmt", "ProcedureStmt",
  "opt_with", "def_args", "def_name_list", "PurgeStmt", "purge_quals",
  "before_clause", "after_clause", "RemoveStmt", "remove_type",
  "RemoveFuncStmt", "func_argtypes", "RemoveOperStmt", "all_Op", "MathOp",
  "oper_argtypes", "RenameStmt", "opt_name", "opt_column", "RuleStmt",
  "$@1", "OptStmtList", "OptStmtBlock", "OptStmtMulti", "event_object",
  "event", "opt_instead", "NotifyStmt", "ListenStmt", "TransactionStmt",
  "ViewStmt", "LoadStmt", "CreatedbStmt", "DestroydbStmt", "ClusterStmt",
  "VacuumStmt", "opt_verbose", "ExplainStmt", "OptimizableStmt",
  "AppendStmt", "insert_rest", "opt_column_list", "columnList",
  "columnElem", "DeleteStmt", "ReplaceStmt", "CursorStmt", "RetrieveStmt",
  "result", "opt_unique", "sort_clause", "sortby_list", "sortby",
  "OptUseOp", "index_params", "func_index", "index_elem", "opt_type",
  "opt_class", "opt_inh_star", "relation_name_list", "name_list",
  "group_clause", "groupby_list", "groupby", "having_clause",
  "from_clause", "from_list", "from_val", "where_clause", "relation_expr",
  "time_range", "opt_range_start", "opt_range_end", "opt_array_bounds",
  "nest_array_bounds", "typname", "Typename", "a_expr", "$@2", "$@3",
  "opt_indirection", "expr_list", "in_expr_nodes", "not_in_expr_nodes",
  "attr", "attrs", "res_target_list", "res_target_el", "res_target_list2",
  "res_target_el2", "opt_id", "relation_name", "database_name",
  "access_method", "attr_name", "class", "index_name", "var_name", "name",
  "date", "file_name", "recipe_name", "AexprConst", "ParamNo", "NumConst",
  "Iconst", "Sconst", "Id", "SpecialRuleRelation", "Type", "Pnull", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
      61,    43,    45,    42,    47,   124,    58,    59,    60,    62,
     375,    46,    91,    93,   376,   377,    40,    41,    44
};
# endif

#define YYPACT_NINF -568

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-568)))

#define YYTABLE_NINF -369

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-369)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1011,   -24,    69,    68,   -22,   -22,    40,   163,   112,   -22,
     123,   358,   114,   157,    18,   109,   177,    53,    99,    18,
      53,    53,   109,   141,   244,    53,   184,   180,   184,   311,
    1011,   186,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,    53,
    -568,  -568,  -568,  -568,  -568,   246,  -568,  -568,  -568,    53,
    -568,   -22,   327,  -568,  -568,   -22,    53,  -568,   -22,   327,
     268,  -568,   163,  -568,    53,  -568,   -22,   -22,  -568,   349,
    -568,   -22,  -568,   -22,  -568,  -568,   -22,  -568,  -568,     0,
     247,  -568,  -568,  -568,  -568,  -568,   250,   189,  -568,    53,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,    45,  -568,
     132,   258,  -568,   262,   799,   252,  -568,    53,   -22,   299,
    -568,   219,  -568,   225,    53,   243,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,   220,  -568,  -568,   343,
     224,   350,  -568,   230,   -22,   336,   263,  -568,   234,  -568,
     235,  -568,  -568,   236,  -568,  -568,   263,  -568,  -568,   324,
    -568,  -568,   -22,   196,   242,   264,   324,  -568,    99,    99,
    -568,   376,   367,   -22,   -22,   105,   823,  -568,   823,  -568,
    -568,   823,  -568,   823,   823,   823,   823,   704,   261,   -18,
    -568,   265,   259,   272,   266,  -568,  -568,    16,  -568,    14,
    -568,  -568,  -568,  -568,  -568,  -568,    41,  -568,   337,    -9,
     -68,   322,  -568,   -22,   325,   327,  -568,   341,   372,   823,
    -568,   -22,   -17,   -22,  -568,   -22,  -568,   320,  -568,   -22,
     -13,   324,  -568,  -568,  -568,  -568,  -568,  -568,   374,  -568,
     406,  -568,   876,   290,   178,  -568,    96,    96,    96,   479,
     823,   -22,   105,  -568,    78,  -568,    12,  -568,   823,   823,
     853,   823,   823,   823,   823,   823,   823,   823,   823,  -568,
     329,   799,   377,   119,   745,   -63,   -22,  -568,   261,   -10,
    -568,   295,   266,   115,   401,   402,  -568,  -568,  -568,   -26,
    -568,   292,   236,  1003,   361,  -568,   293,   298,   -63,  -568,
     318,    90,  -568,    53,   352,   719,   306,   236,   307,   315,
     316,  -568,  -568,    -7,   121,  -568,   261,   799,   308,  -568,
    -568,    -7,   -63,   -22,  -568,   876,  -568,   447,   326,   381,
    -568,   105,  -568,   823,   676,   904,   178,   148,   212,   212,
      96,    96,   133,   133,   555,    53,  -568,    53,   263,  -568,
     330,  -568,  -568,   323,  -568,   719,   142,   -22,   333,  -568,
     332,   330,  -568,    29,   263,   160,   346,   -22,  -568,   -22,
    -568,  -568,   375,  -568,  -568,   -22,  -568,   369,  -568,  -568,
    -568,  -568,  -568,   232,   421,   -22,  -568,  1003,  -568,   327,
     379,   244,  -568,   -22,  -568,    64,   -22,  -568,   378,  -568,
    -568,   -22,  -568,    -6,   799,  -568,  -568,   105,   105,  -568,
     471,   347,   904,   823,   261,  -568,   359,  -568,    13,   -20,
     436,   165,  -568,  -568,   823,  -568,   -65,  -568,   264,  -568,
    -568,  -568,   823,  -568,   392,  -568,   455,  -568,  -568,   230,
     482,  -568,  -568,  -568,  -568,   403,   364,   489,  -568,  -568,
    -568,   -22,   368,   799,  -568,  -568,  -568,  -568,   456,  -568,
    -568,   263,   159,  -568,   166,  -568,   105,   105,   636,  -568,
      53,   -22,  -568,  -568,  -568,    99,  -568,   488,   461,  -568,
    -568,   719,   382,   380,   383,   719,   -22,    99,  -568,    99,
      53,   -22,   391,   423,  -568,  -568,   -22,    -6,   441,  -568,
    -568,  -568,   105,  -568,   207,  -568,   261,  -568,  -568,   384,
     385,    71,   823,   443,   -59,  -568,   382,  -568,  -568,  -568,
     465,   263,   397,   394,   136,   409,   519,   396,  -568,  -568,
     408,   399,   404,   263,  -568,  -568,  -568,   105,  -568,    99,
    -568,   400,  -568,  -568,   410,   719,   518,  -568,   382,   414,
    -568,    99,   504,   -22,  -568,  -568,  -568,  -568,  -568,    99,
     428,  -568,   369,   -63,   -43,   -22,   436,  -568,   417,  -568,
      71,   -22,    71,  -568,   382,  -568,   496,  -568,  -568,    99,
    -568,  -568,   -22,  -568,  -568,  -568,   217,   443,  -568,  -568,
    -568,   405,  -568,    25,    22,  -568,  -568,   102,  -568,  -568,
     408,  -568,    71,  -568,  -568,    66,  -568,   -22,  -568,  -568,
     299,  -568,  -568,   -43,  -568,  -568,  -568,  -568,    25,   419,
     299,   430,  -568,  -568,  -568,   432,  -568,  -568
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   192,     0,   193,   359,     0,   194,    47,   121,     0,
       0,     0,   195,     0,    90,     0,     0,     0,     0,    90,
       0,     0,     0,   196,   229,     0,   205,     0,   205,     0,
       2,     3,     7,     8,     9,    10,    12,    13,    16,    17,
      28,    19,    18,    14,    23,    21,    22,    26,    25,    24,
      27,    30,   211,    20,    31,    32,    33,    34,    35,    11,
      36,    15,    29,   210,   212,   209,   208,   207,   186,     0,
     187,   188,   382,    39,   358,     0,   366,   189,    46,     0,
      72,     0,     0,    70,   385,     0,     0,   120,     0,     0,
       0,    71,    47,   368,     0,   139,     0,     0,   141,     0,
     142,     0,   143,     0,   140,   190,     0,    89,    88,    93,
      96,   103,   101,   104,   100,   102,     0,    97,    98,     0,
     383,   384,   185,   361,   360,   381,   198,   370,   114,   184,
     135,     0,   191,   227,     0,     0,   204,   202,     0,     0,
       1,     5,     6,   253,     0,    49,   199,   362,    75,   156,
     150,   151,   152,   153,   154,   155,     0,    74,    73,     0,
       0,     0,    68,     0,     0,     0,   274,   200,     0,   148,
       0,   149,    86,   254,   255,   138,   274,    92,   380,   116,
      91,    95,     0,     0,   217,     0,   116,   113,     0,     0,
     130,   131,   132,     0,     0,     0,     0,   386,     0,   377,
     373,     0,   357,     0,     0,     0,     0,   355,   334,   226,
     353,     0,     0,   295,   375,   372,   374,   315,   376,     0,
     203,   123,   371,   206,     4,   252,     0,   201,     0,     0,
       0,     0,   166,    54,     0,     0,    69,     0,     0,     0,
     221,   146,     0,     0,   122,     0,    87,     0,    99,     0,
       0,   116,   111,   137,   369,   136,   133,   134,     0,   228,
       0,   375,   331,     0,   313,   296,   306,   304,   305,     0,
       0,     0,     0,   325,     0,   319,     0,   321,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,   294,
       0,     0,   267,     0,     0,     0,     0,   348,   334,   267,
     347,     0,     0,   334,     0,   165,    48,    42,    41,     0,
     128,     0,   129,     0,     0,    56,     0,    53,     0,   197,
      80,     0,    77,     0,     0,   273,     0,   145,     0,     0,
     157,   256,   115,     0,     0,   219,   334,     0,     0,   213,
     112,     0,     0,     0,   309,   329,   354,     0,     0,     0,
     320,     0,   327,     0,   330,   311,   310,   303,   297,   298,
     300,   299,   301,   302,     0,     0,   352,     0,   274,   356,
     341,   343,   364,     0,   317,   335,     0,     0,   286,   307,
     290,   342,   350,     0,   274,     0,     0,     0,   164,   163,
      44,    45,    51,    43,   127,     0,   379,   126,    82,    83,
     378,    84,    81,     0,    67,     0,    38,     0,    76,     0,
     119,   229,   144,     0,   147,     0,     0,   105,   108,   107,
     216,     0,   220,   267,     0,   110,   308,     0,     0,   322,
       0,     0,   312,     0,   334,   225,   266,   269,   272,   275,
     258,     0,   316,   318,     0,   291,     0,   292,     0,   346,
     222,   351,     0,    37,     0,   162,     0,    40,    85,     0,
       0,   180,   181,   178,   179,     0,     0,    58,    55,    79,
      78,     0,     0,     0,   159,   160,   158,   106,     0,    94,
     218,   274,     0,   323,     0,   337,     0,     0,     0,   332,
       0,     0,   271,   367,   276,   281,   277,     0,   265,   345,
     344,   336,   289,     0,     0,   349,     0,     0,   125,     0,
       0,     0,     0,    63,   118,   363,     0,   267,     0,   215,
     214,   326,     0,   324,     0,   339,   334,   268,   270,     0,
     280,     0,     0,   231,     0,   284,   289,   293,   161,    50,
       0,   274,   177,     0,     0,     0,    65,     0,   244,   243,
     248,     0,   364,   274,   109,   338,   328,     0,   333,   283,
     279,   257,   259,   263,   261,   264,     0,   224,   289,     0,
     285,     0,     0,     0,    66,    59,    60,    61,    57,     0,
       0,    52,   126,     0,   251,     0,   258,   340,     0,   282,
       0,     0,     0,   287,   289,   124,   183,   176,    62,     0,
     117,   247,     0,   246,   249,   365,     0,   231,   278,   260,
     262,   230,   232,   242,   242,   288,   182,     0,    64,   250,
     248,   223,     0,   240,   241,     0,   236,     0,   234,   168,
       0,   167,   169,   251,   233,   237,   238,   239,   242,     0,
     171,   172,   245,   235,   170,   174,   175,   173
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -568,  -568,  -568,   536,  -568,  -248,  -568,  -568,  -568,  -568,
     475,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   193,   111,  -568,   162,   161,  -568,
    -568,   553,  -568,  -568,   551,  -568,   393,   237,  -568,  -568,
    -568,  -568,  -143,  -568,  -568,  -568,  -568,  -568,  -568,    -5,
    -568,  -568,  -568,  -568,   390,   398,  -568,  -568,  -568,  -568,
    -568,   476,   -97,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   557,  -568,  -135,  -568,  -568,  -568,
    -568,   169,  -568,  -568,  -568,   363,  -568,   172,    -3,  -568,
     -30,  -567,  -568,  -568,  -568,   -11,   -28,  -568,  -168,  -223,
      24,  -568,    21,  -568,  -273,  -568,   124,  -164,  -568,  -568,
    -568,  -568,  -568,  -514,  -568,  -309,   357,  -568,  -568,  -283,
    -568,  -568,  -568,  -208,   319,  -568,   238,  -314,   328,  -568,
      35,   521,  -568,  -411,    26,   -64,   129,    10,  -183,  -568,
    -568,  -163,  -171,  -568,   -99,   -15,    -4,  -568,   613,  -568
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    29,    30,    31,    32,   315,    33,    34,   309,   392,
      79,   229,   457,    35,   316,   317,   513,   578,   546,   581,
     467,    36,   162,    89,   320,   236,   321,   322,   397,    37,
      38,   109,   179,    39,   116,   117,   118,   418,   479,    40,
      41,   186,   246,    42,   472,    90,    43,    44,    45,   460,
     231,   311,    46,   190,   191,   192,    47,   103,    48,   326,
      49,   398,   157,   329,    50,   454,   389,    51,   314,   631,
     639,   640,   541,   465,   617,    52,    53,    54,    55,    56,
      57,    58,    59,    60,   137,    61,    62,    63,   339,   250,
     334,   335,    64,    65,    66,    67,   292,   134,   567,   611,
     612,   626,   547,   548,   549,   584,   603,   226,   172,   173,
     498,   561,   562,   533,   368,   436,   437,   240,   438,   496,
     529,   588,   447,   535,   378,   379,   207,   348,   431,   289,
     376,   484,   524,   208,   370,   299,   300,   209,   210,    73,
     263,   146,   514,   371,   604,    75,   492,   212,   253,   126,
     221,   213,   214,   399,   215,   216,   217,   124,    91,   218
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,    76,   171,   127,   223,    93,   255,   312,   177,   406,
     180,   298,   244,   123,   247,   382,   123,   123,   327,    92,
     386,   123,   570,   423,   261,   258,   384,   491,   377,   187,
     500,   351,   260,   426,   107,   367,   307,   623,   290,   367,
     623,   416,   176,   252,   120,    72,   304,   628,   302,   328,
      72,   178,   122,   422,   593,   129,   130,   178,   624,   120,
     135,   624,   108,   352,   602,   123,   390,   391,   502,   310,
      72,   643,   417,    68,   568,   123,   337,   147,   158,   121,
     615,    93,   123,   120,    93,   158,   251,   308,   125,   338,
     123,    72,   147,    93,   121,   159,    72,    93,   161,    93,
     237,   261,    76,   494,   143,   550,    72,   168,   340,   347,
     482,   174,   495,   175,   145,   123,   178,   110,   121,    80,
     291,   160,   625,   305,   353,   625,    72,    72,   383,   166,
     475,   199,   291,   123,   222,     9,    10,   297,   188,   453,
     123,   185,    72,   111,    81,   349,   199,  -361,    77,   189,
     481,   489,  -368,   627,   184,   350,    16,   468,    82,   517,
      76,   178,   597,   112,    69,    70,    72,   272,    94,   211,
     629,    20,   220,   254,   254,   298,    71,    72,    93,   227,
     261,   635,   197,    78,    72,    83,   575,   178,   430,    93,
     259,    24,   174,    84,   636,   637,   576,   113,   114,   273,
      85,    25,   577,   174,   440,   274,   275,    86,   115,   106,
      87,   105,   302,   125,   400,   303,   171,    88,   277,   125,
     450,   178,   199,   200,   286,   287,    93,   408,   409,   318,
     111,   158,    72,   119,   630,   274,   275,    93,    93,    93,
     174,    93,   369,   558,   553,   336,  -361,   288,   277,   132,
     112,   174,   330,   331,   301,   332,   261,   261,   420,   421,
     279,  -369,  -369,   280,   483,   485,   461,   346,   281,   282,
     283,   284,   285,    72,   601,   156,   286,   287,    72,   443,
     444,   133,   163,   451,   113,   114,   462,   136,   499,   372,
     138,    93,   372,  -369,   393,   115,   520,   291,   401,   282,
     283,   284,   285,   521,   522,   380,   286,   287,   400,   402,
     171,   140,   530,   142,    93,   261,   261,   519,   144,   123,
     164,   463,   182,   523,   525,   181,   211,   183,   380,   419,
     193,   464,     9,    10,   194,   284,   285,   419,    93,   372,
     286,   287,   219,   543,   556,   557,   224,   503,   225,   504,
     228,   261,   380,    16,   620,   243,   230,   232,   410,   555,
     233,   123,   606,   123,   234,    95,   235,   238,    20,   239,
     241,   242,   211,    93,   243,   245,   589,   572,   249,   303,
     178,   372,   188,   318,   189,    93,   261,   445,    24,   586,
      96,   458,   401,   288,   587,   294,   293,   296,    25,   455,
     435,   318,   439,   402,    97,   158,   295,   313,   306,    93,
      98,    93,   477,   323,    24,   324,   333,   336,   301,   341,
     342,   343,   367,   474,   365,   476,   385,   387,   388,   394,
     404,    99,   563,   403,   493,   569,   405,   372,   407,    84,
      72,   411,   148,   412,   424,   413,   100,   149,   150,   151,
     152,   153,   414,   101,   415,   154,   155,   427,   429,   211,
     442,   441,   428,   102,   169,   446,   452,   515,   448,   149,
     150,   151,   152,   153,   466,   456,   459,   154,   155,   471,
     254,   486,   632,   487,   497,   478,   123,   493,   506,   270,
     507,   563,   539,   613,   540,   641,   509,   490,   272,   510,
     511,   512,    93,   518,   516,   645,   123,    93,   211,   531,
     532,   544,   552,   536,   534,   554,   538,   545,   560,   566,
     537,   174,   559,   613,   571,   439,   551,   564,   573,   579,
     273,   574,   580,   582,   583,   585,   274,   275,   590,   592,
    -368,   591,   596,   622,   254,   542,   276,   594,   599,   277,
     608,   616,   644,   262,   278,   264,   595,   646,   265,   647,
     266,   267,   268,   269,   598,   270,   141,   165,   469,   372,
     508,   470,   128,   131,   272,   170,   248,   600,   425,    93,
     605,    93,   257,   473,   618,   139,   564,   610,   614,   256,
     480,   279,   634,   380,   280,   174,   325,   319,   605,   281,
     282,   283,   284,   285,   621,   642,   273,   286,   287,   633,
     607,   609,   274,   275,   527,   381,   344,   167,   614,   366,
     528,   449,   276,   638,   104,   277,     0,   345,   619,   605,
     278,     0,     0,     0,     0,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   270,     0,     0,     0,
       0,   375,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   279,     0,     0,
     280,     0,     0,     0,     0,   281,   282,   283,   284,   285,
       0,   433,     0,   286,   287,     0,   270,   273,   434,     0,
       0,     0,     0,   274,   275,   272,     0,     0,     0,     0,
       0,     0,     0,   276,     0,     0,   277,     0,     0,     0,
     432,   278,     0,     0,   270,     0,     0,     0,   271,     0,
       0,     0,     0,   272,     0,     0,     0,   273,     0,   270,
       0,     0,     0,   274,   275,     0,     0,     0,   272,     0,
       0,     0,     0,   276,     0,     0,   277,     0,   279,     0,
       0,   280,     0,     0,     0,   273,   281,   282,   283,   284,
     285,   274,   275,     0,   286,   287,     0,   195,     0,   526,
     273,   276,     0,     0,   277,   120,   274,   275,     0,   278,
       0,     0,     0,     0,     0,     0,   276,     0,   279,   277,
     488,   280,     0,     0,   278,     0,   281,   282,   283,   284,
     285,   501,     0,     0,   286,   287,     0,     0,     0,   505,
     121,     0,   196,     0,     0,     0,   279,     0,     0,   280,
       0,   195,   197,     0,   281,   282,   283,   284,   285,   120,
       0,   279,   286,   287,   280,     0,     0,     0,     0,   281,
     282,   283,   284,   285,     0,   195,     0,   286,   287,     0,
       0,     0,     0,   120,     0,     0,     0,     0,    72,   125,
     198,   178,   199,   200,   121,     0,   196,   201,   373,     0,
     203,   204,   205,     0,     0,   195,   197,     0,     0,     0,
       0,   206,   374,   120,     0,     0,     0,     0,   121,   565,
     196,     0,     0,     0,     0,   272,     0,     0,     0,     0,
     197,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    72,   125,   198,   178,   199,   200,   121,     0,
       0,   201,   202,   272,   203,   204,   205,   273,     0,     0,
     197,     0,     0,   274,   275,   206,    72,   125,   198,   178,
     199,   200,     0,   276,     0,   201,   277,     0,   203,   204,
     205,     0,     0,     0,     0,   273,     0,     0,     0,   206,
       0,   274,   275,     0,     0,     0,    72,   125,  -369,   178,
     199,   200,     0,     0,   277,   201,     0,     0,   203,   204,
     205,     0,     0,     0,     0,     0,     0,     0,   279,   206,
       0,   280,     0,     0,     0,     0,   281,   282,   283,   284,
     285,     0,     0,     0,   286,   287,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     0,  -369,     0,     0,   280,
       2,     0,     0,     0,     0,   282,   283,   284,   285,     3,
       0,     0,   286,   287,     0,     4,     5,     0,     6,     7,
       8,     0,     0,     0,     9,    10,     0,     0,     0,     0,
      11,    12,    13,    14,     0,     0,     0,     0,    15,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,     0,
       0,     0,    17,    18,     0,    19,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,   395,     0,     0,    22,    23,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,    26,     0,     0,     0,    72,   125,   169,   178,
      27,   396,    28,   149,   150,   151,   152,   153,     0,     0,
       0,   154,   155
};

static const yytype_int16 yycheck[] =
{
       4,     5,    99,    18,   139,     9,   189,   230,     8,   318,
     109,   219,   176,    17,   182,   298,    20,    21,   241,     9,
     303,    25,   536,   337,   195,   193,   299,    14,    91,   128,
     441,    19,   195,   342,    16,    45,    45,    15,    56,    45,
      15,    48,   106,   186,    30,   113,     5,   614,   219,    66,
     113,   116,    17,   336,   568,    20,    21,   116,    36,    30,
      25,    36,    44,    51,   107,    69,    92,    93,   133,   137,
     113,   638,    79,    97,   133,    79,    89,    81,    82,    65,
     594,    85,    86,    30,    88,    89,   185,    96,   114,   102,
      94,   113,    96,    97,    65,    85,   113,   101,    88,   103,
     164,   272,   106,   123,    69,   516,   113,    97,   251,   272,
     424,   101,   132,   103,    79,   119,   116,     8,    65,     7,
     138,    86,   100,    82,   112,   100,   113,   113,   138,    94,
      66,   117,   138,   137,   138,    33,    34,   123,     6,   387,
     144,    96,   113,    34,    32,    67,   117,   131,   108,    17,
     423,   434,   136,   131,   119,    77,    54,   405,    46,   473,
     164,   116,   573,    54,    95,    97,   113,    19,    45,   134,
      68,    69,   137,   188,   189,   383,   108,   113,   182,   144,
     351,   115,    77,    20,   113,    73,    50,   116,   351,   193,
     194,    89,   182,    81,   128,   129,    60,    88,    89,    51,
      88,    99,    66,   193,   368,    57,    58,    95,    99,    52,
      98,    97,   383,   114,   313,   219,   313,   105,    70,   114,
     384,   116,   117,   118,   128,   129,   230,   137,   138,   233,
      34,   235,   113,    56,   132,    57,    58,   241,   242,   243,
     230,   245,   123,   526,   517,   249,   131,   132,    70,   108,
      54,   241,   242,   243,   219,   245,   427,   428,   137,   138,
     112,   128,   129,   115,   427,   428,    34,   271,   120,   121,
     122,   123,   124,   113,   583,    82,   128,   129,   113,   137,
     138,    37,    89,   123,    88,    89,    54,   103,   123,   293,
     110,   295,   296,   115,   309,    99,   137,   138,   313,   121,
     122,   123,   124,   137,   138,   295,   128,   129,   407,   313,
     407,     0,   495,   127,   318,   486,   487,   481,    72,   323,
      52,    89,    72,   486,   487,    78,   291,   138,   318,   333,
      72,    99,    33,    34,    72,   123,   124,   341,   342,   343,
     128,   129,    90,   511,   137,   138,   127,   446,   123,   448,
     107,   522,   342,    54,   137,   138,   136,    14,   323,   522,
     136,   365,   585,   367,    14,     7,   136,    31,    69,   106,
     136,   136,   337,   377,   138,    51,   559,   541,   136,   383,
     116,   385,     6,   387,    17,   389,   557,   377,    89,   553,
      32,   395,   407,   132,   557,   136,   131,   131,    99,   389,
     365,   405,   367,   407,    46,   409,   134,    85,    71,   413,
      52,   415,   416,    72,    89,    43,    96,   421,   383,    45,
      14,   131,    45,   413,    95,   415,   131,    26,    26,   137,
     137,    73,   531,    72,   438,   534,   138,   441,   120,    81,
     113,    89,   115,   137,   136,   138,    88,   120,   121,   122,
     123,   124,   137,    95,   138,   128,   129,    10,    77,   424,
     137,   131,   136,   105,   115,   132,   120,   471,   136,   120,
     121,   122,   123,   124,    53,   100,   107,   128,   129,   100,
     495,    10,   617,   136,    48,   107,   490,   491,    96,    10,
      35,   590,   507,   592,   509,   630,    14,   138,    19,    96,
     136,    12,   506,    47,   136,   640,   510,   511,   473,    21,
      49,   120,   516,   133,   132,    74,   506,    94,   133,    76,
     137,   511,   138,   622,    59,   490,   516,   531,   131,   120,
      51,   137,    13,   137,   126,   136,    57,    58,   138,    21,
     136,   131,    38,   138,   559,   510,    67,   133,   120,    70,
     133,    55,   133,   196,    75,   198,   571,   127,   201,   127,
     203,   204,   205,   206,   579,    10,    30,    92,   407,   573,
     459,   409,    19,    22,    19,    99,   183,   582,   341,   583,
     584,   585,   192,   411,   599,    28,   590,   591,   592,   191,
     421,   112,   622,   583,   115,   585,   239,   234,   602,   120,
     121,   122,   123,   124,   607,   633,    51,   128,   129,   620,
     586,   590,    57,    58,   490,   296,   137,    96,   622,   291,
     491,   383,    67,   627,    11,    70,    -1,   270,   602,   633,
      75,    -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,    10,    -1,    -1,    -1,
      -1,   294,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,
      -1,   126,    -1,   128,   129,    -1,    10,    51,   133,    -1,
      -1,    -1,    -1,    57,    58,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,
     353,    75,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    51,    -1,    10,
      -1,    -1,    -1,    57,    58,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    70,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    51,   120,   121,   122,   123,
     124,    57,    58,    -1,   128,   129,    -1,    22,    -1,   133,
      51,    67,    -1,    -1,    70,    30,    57,    58,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,   112,    70,
     433,   115,    -1,    -1,    75,    -1,   120,   121,   122,   123,
     124,   444,    -1,    -1,   128,   129,    -1,    -1,    -1,   452,
      65,    -1,    67,    -1,    -1,    -1,   112,    -1,    -1,   115,
      -1,    22,    77,    -1,   120,   121,   122,   123,   124,    30,
      -1,   112,   128,   129,   115,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,    -1,    22,    -1,   128,   129,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    65,    -1,    67,   122,   123,    -1,
     125,   126,   127,    -1,    -1,    22,    77,    -1,    -1,    -1,
      -1,   136,   137,    30,    -1,    -1,    -1,    -1,    65,   532,
      67,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,    65,    -1,
      -1,   122,   123,    19,   125,   126,   127,    51,    -1,    -1,
      77,    -1,    -1,    57,    58,   136,   113,   114,   115,   116,
     117,   118,    -1,    67,    -1,   122,    70,    -1,   125,   126,
     127,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,   136,
      -1,    57,    58,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,    70,   122,    -1,    -1,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   136,
      -1,   115,    -1,    -1,    -1,    -1,   120,   121,   122,   123,
     124,    -1,    -1,    -1,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,   112,    -1,    -1,   115,
       9,    -1,    -1,    -1,    -1,   121,   122,   123,   124,    18,
      -1,    -1,   128,   129,    -1,    24,    25,    -1,    27,    28,
      29,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      39,    40,    41,    42,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    -1,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    91,    -1,    -1,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,    -1,    -1,    -1,   113,   114,   115,   116,
     109,   118,   111,   120,   121,   122,   123,   124,    -1,    -1,
      -1,   128,   129
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     9,    18,    24,    25,    27,    28,    29,    33,
      34,    39,    40,    41,    42,    47,    54,    61,    62,    64,
      69,    80,    86,    87,    89,    99,   101,   109,   111,   140,
     141,   142,   143,   145,   146,   152,   160,   168,   169,   172,
     178,   179,   182,   185,   186,   187,   191,   195,   197,   199,
     203,   206,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   224,   225,   226,   231,   232,   233,   234,    97,    95,
      97,   108,   113,   278,   295,   284,   295,   108,    20,   149,
       7,    32,    46,    73,    81,    88,    95,    98,   105,   162,
     184,   297,   286,   295,    45,     7,    32,    46,    52,    73,
      88,    95,   105,   196,   297,    97,    52,    16,    44,   170,
       8,    34,    54,    88,    89,    99,   173,   174,   175,    56,
      30,    65,   279,   295,   296,   114,   288,   294,   170,   279,
     279,   173,   108,    37,   236,   279,   103,   223,   110,   223,
       0,   142,   127,   279,    72,   279,   280,   295,   115,   120,
     121,   122,   123,   124,   128,   129,   163,   201,   295,   286,
     279,   286,   161,   163,    52,   149,   279,   280,   286,   115,
     200,   201,   247,   248,   286,   286,   284,     8,   116,   171,
     293,    78,    72,   138,   279,    96,   180,   293,     6,    17,
     192,   193,   194,    72,    72,    22,    67,    77,   115,   117,
     118,   122,   123,   125,   126,   127,   136,   265,   272,   276,
     277,   279,   286,   290,   291,   293,   294,   295,   298,    90,
     279,   289,   295,   225,   127,   123,   246,   279,   107,   150,
     136,   189,    14,   136,    14,   136,   164,   284,    31,   106,
     256,   136,   136,   138,   256,    51,   181,   247,   175,   136,
     228,   293,   181,   287,   294,   287,   194,   193,   247,   295,
     290,   291,   265,   279,   265,   265,   265,   265,   265,   265,
      10,    14,    19,    51,    57,    58,    67,    70,    75,   112,
     115,   120,   121,   122,   123,   124,   128,   129,   132,   268,
      56,   138,   235,   131,   136,   134,   131,   123,   272,   274,
     275,   279,   291,   295,     5,    82,    71,    45,    96,   147,
     137,   190,   248,    85,   207,   144,   153,   154,   295,   234,
     163,   165,   166,    72,    43,   265,   198,   248,    66,   202,
     286,   286,   286,    96,   229,   230,   295,    89,   102,   227,
     181,    45,    14,   131,   137,   265,   295,   290,   266,    67,
      77,    19,    51,   112,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,    95,   277,    45,   253,   123,
     273,   282,   295,   123,   137,   265,   269,    91,   263,   264,
     286,   273,   268,   138,   253,   131,   268,    26,    26,   205,
      92,    93,   148,   294,   137,    91,   118,   167,   200,   292,
     293,   294,   295,    72,   137,   138,   264,   120,   137,   138,
     279,    89,   137,   138,   137,   138,    48,    79,   176,   295,
     137,   138,   268,   276,   136,   176,   264,    10,   136,    77,
     290,   267,   265,   126,   133,   279,   254,   255,   257,   279,
     256,   131,   137,   137,   138,   286,   132,   261,   136,   275,
     256,   123,   120,   144,   204,   286,   100,   151,   295,   107,
     188,    34,    54,    89,    99,   212,    53,   159,   144,   167,
     166,   100,   183,   236,   286,    66,   286,   295,   107,   177,
     230,   253,   276,   290,   270,   290,    10,   136,   265,   268,
     138,    14,   285,   295,   123,   132,   258,    48,   249,   123,
     282,   265,   133,   293,   293,   265,    96,    35,   164,    14,
      96,   136,    12,   155,   281,   295,   136,   276,    47,   256,
     137,   137,   138,   290,   271,   290,   133,   255,   285,   259,
     287,    21,    49,   252,   132,   262,   133,   137,   286,   294,
     294,   211,   279,   247,   120,    94,   157,   241,   242,   243,
     282,   286,   295,   253,    74,   290,   137,   138,   268,   138,
     133,   250,   251,   293,   295,   265,    76,   237,   133,   293,
     262,    59,   256,   131,   137,    50,    60,    66,   156,   120,
      13,   158,   137,   126,   244,   136,   256,   290,   260,   287,
     138,   131,    21,   262,   133,   294,    38,   282,   294,   120,
     188,   264,   107,   245,   283,   295,   248,   249,   133,   251,
     295,   238,   239,   293,   295,   262,    55,   213,   294,   283,
     137,   237,   138,    15,    36,   100,   240,   131,   240,    68,
     132,   208,   225,   244,   239,   115,   128,   129,   295,   209,
     210,   225,   245,   240,   133,   225,   127,   127
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   139,   140,   140,   141,   141,   141,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   143,   144,   145,
     146,   147,   147,   148,   148,   148,   149,   149,   150,   150,
     151,   151,   152,   153,   153,   154,   154,   155,   155,   156,
     156,   156,   157,   157,   158,   158,   159,   159,   160,   161,
     162,   162,   162,   163,   163,   163,   164,   165,   165,   166,
     166,   167,   167,   167,   167,   167,   168,   169,   170,   170,
     170,   171,   171,   171,   172,   173,   173,   173,   174,   174,
     175,   175,   175,   175,   175,   176,   176,   176,   177,   177,
     178,   179,   179,   180,   180,   181,   181,   182,   183,   183,
     184,   184,   185,   186,   187,   188,   188,   189,   189,   190,
     191,   192,   192,   192,   192,   192,   193,   194,   195,   196,
     196,   196,   196,   196,   197,   198,   198,   199,   200,   200,
     201,   201,   201,   201,   201,   201,   201,   202,   202,   202,
     202,   203,   204,   204,   205,   205,   207,   206,   208,   208,
     208,   209,   209,   210,   210,   210,   211,   211,   212,   212,
     212,   212,   213,   213,   214,   215,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   217,   218,   219,
     220,   221,   222,   222,   223,   223,   224,   225,   225,   225,
     225,   225,   225,   226,   227,   227,   228,   228,   229,   229,
     230,   231,   232,   233,   234,   235,   235,   236,   236,   236,
     237,   237,   238,   238,   239,   239,   239,   240,   240,   240,
     240,   240,   240,   241,   241,   242,   243,   244,   244,   245,
     245,   245,   246,   246,   247,   248,   248,   249,   249,   250,
     250,   251,   251,   251,   252,   252,   253,   253,   254,   254,
     255,   255,   255,   256,   256,   257,   257,   257,   258,   258,
     259,   259,   260,   260,   261,   261,   261,   262,   262,   262,
     263,   263,   264,   264,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   266,   265,   267,   265,   265,
     265,   265,   268,   268,   268,   269,   269,   270,   270,   271,
     271,   272,   272,   273,   273,   273,   274,   274,   274,   275,
     275,   275,   276,   276,   277,   277,   277,   277,   278,   278,
     279,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   290,   290,   290,   290,   291,   292,   292,
     293,   294,   295,   296,   296,   297,   298
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     7,     2,     2,
       7,     1,     1,     1,     1,     1,     1,     0,     2,     0,
       3,     0,    10,     1,     0,     3,     1,     3,     0,     1,
       1,     1,     3,     0,     3,     0,     4,     0,     3,     2,
       1,     1,     1,     1,     1,     1,     3,     1,     3,     3,
       1,     1,     1,     1,     1,     2,     3,     4,     1,     1,
       0,     1,     1,     0,     7,     2,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     1,     0,     3,
       6,     4,     5,     1,     0,     2,     0,    11,     2,     0,
       1,     0,     4,     3,    11,     2,     0,     3,     2,     1,
       3,     1,     1,     2,     2,     0,     2,     2,     3,     1,
       1,     1,     1,     1,     6,     1,     0,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     9,     1,     0,     1,     0,     0,    13,     1,     1,
       3,     1,     1,     3,     2,     2,     3,     1,     1,     1,
       1,     1,     1,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     5,     2,     3,
       3,     4,     2,     3,     1,     0,     3,     1,     1,     1,
       1,     1,     1,     5,     4,     4,     3,     0,     3,     1,
       2,     4,     6,    12,     9,     3,     0,     1,     3,     0,
       3,     0,     1,     3,     2,     4,     2,     2,     2,     2,
       1,     1,     0,     1,     1,     6,     3,     2,     0,     1,
       2,     0,     1,     0,     1,     1,     3,     3,     0,     1,
       3,     1,     3,     1,     2,     0,     2,     0,     3,     1,
       3,     2,     1,     2,     0,     1,     2,     2,     5,     3,
       1,     0,     1,     0,     3,     4,     0,     3,     4,     0,
       1,     2,     2,     4,     2,     1,     2,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     3,     4,     3,
       3,     3,     4,     2,     2,     1,     4,     3,     4,     2,
       3,     2,     4,     5,     6,     0,     6,     0,     7,     3,
       3,     2,     4,     6,     0,     1,     3,     1,     3,     1,
       3,     3,     3,     1,     3,     3,     3,     1,     1,     4,
       2,     3,     3,     1,     3,     1,     3,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 228 "gram.y" /* yacc.c:1646  */
    { parsetree = (yyvsp[0].list); }
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 230 "gram.y" /* yacc.c:1646  */
    { parsetree = lcons((yyvsp[0].node),NIL); }
#line 2175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 234 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[-1].node)); }
#line 2181 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 236 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 238 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[-1].node),NIL); }
#line 2193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 281 "gram.y" /* yacc.c:1646  */
    {
		    AddAttrStmt *n = makeNode(AddAttrStmt);
		    n->relname = (yyvsp[-4].str);
		    n->inh = (yyvsp[-3].boolean);
		    n->colDef = (yyvsp[0].coldef);
		    (yyval.node) = (Node *)n;
		}
#line 2205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 291 "gram.y" /* yacc.c:1646  */
    {  
		    (yyval.coldef) = makeNode(ColumnDef);
		    (yyval.coldef)->colname = (yyvsp[-1].str);
		    (yyval.coldef)->typename = (yyvsp[0].typnam);
		}
#line 2215 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 307 "gram.y" /* yacc.c:1646  */
    {  
		    ClosePortalStmt *n = makeNode(ClosePortalStmt);
		    n->portalname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 2225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 324 "gram.y" /* yacc.c:1646  */
    {
		    CopyStmt *n = makeNode(CopyStmt);
		    n->binary = (yyvsp[-5].boolean);
		    n->relname = (yyvsp[-4].str);
		    n->oids = (yyvsp[-3].boolean);
		    n->direction = (yyvsp[-2].ival);
		    n->filename = (yyvsp[-1].str);
		    n->delimiter = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 2240 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 337 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = TO; }
#line 2246 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 339 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = FROM; }
#line 2252 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 347 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 2258 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 348 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = NULL; }
#line 2264 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 349 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = NULL; }
#line 2270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 352 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = TRUE; }
#line 2276 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 353 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = FALSE; }
#line 2282 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 356 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = TRUE; }
#line 2288 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 357 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = FALSE; }
#line 2294 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 363 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str);}
#line 2300 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 364 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "\t"; }
#line 2306 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 377 "gram.y" /* yacc.c:1646  */
    {
		    CreateStmt *n = makeNode(CreateStmt);
		    n->relname = (yyvsp[-7].str);
		    n->tableElts = (yyvsp[-5].list);
		    n->inhRelnames = (yyvsp[-3].list);
		    n->archiveType = (yyvsp[-2].ival);
		    n->location = (yyvsp[-1].ival);
		    n->archiveLoc = (yyvsp[0].ival);
		    (yyval.node) = (Node *)n;
		}
#line 2321 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 389 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 2327 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 390 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 2333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 395 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].coldef)); }
#line 2339 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 397 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].coldef), NIL); }
#line 2345 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 401 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].ival); }
#line 2351 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 402 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = ARCH_NONE; }
#line 2357 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 405 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = ARCH_HEAVY; }
#line 2363 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 406 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = ARCH_LIGHT; }
#line 2369 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 407 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = ARCH_NONE; }
#line 2375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 411 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = smgrin((yyvsp[0].str));  }
#line 2381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 413 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = -1;  }
#line 2387 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 417 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = smgrin((yyvsp[0].str));  }
#line 2393 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 419 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = -1;  }
#line 2399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 422 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[-1].list); }
#line 2405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 423 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 2411 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 435 "gram.y" /* yacc.c:1646  */
    {
		    (yyvsp[0].dstmt)->defType = (yyvsp[-1].ival);
		    (yyval.node) = (Node *)(yyvsp[0].dstmt);
		}
#line 2420 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 442 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.dstmt) = makeNode(DefineStmt);
		    (yyval.dstmt)->defname = (yyvsp[-1].str);
		    (yyval.dstmt)->definition = (yyvsp[0].list);
		}
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 449 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = OPERATOR; }
#line 2436 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 450 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = P_TYPE; }
#line 2442 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 451 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = AGGREGATE; }
#line 2448 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 458 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[-1].list); }
#line 2454 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 463 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].defelt), NIL); }
#line 2460 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 465 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].defelt)); }
#line 2466 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 469 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.defelt) = makeNode(DefElem);
		    (yyval.defelt)->defname = (yyvsp[-2].str);
		    (yyval.defelt)->arg = (Node *)(yyvsp[0].node);
		}
#line 2476 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 475 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.defelt) = makeNode(DefElem);
		    (yyval.defelt)->defname = (yyvsp[0].str);
		    (yyval.defelt)->arg = (Node *)NULL;
		}
#line 2486 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 482 "gram.y" /* yacc.c:1646  */
    {  (yyval.node) = (Node *)makeString((yyvsp[0].str)); }
#line 2492 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 483 "gram.y" /* yacc.c:1646  */
    {  (yyval.node) = (Node *)makeString((yyvsp[0].str)); }
#line 2498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 484 "gram.y" /* yacc.c:1646  */
    {  (yyval.node) = (Node *)(yyvsp[0].value); /* already a Value */ }
#line 2504 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 485 "gram.y" /* yacc.c:1646  */
    {  (yyval.node) = (Node *)makeString((yyvsp[0].str)); }
#line 2510 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 486 "gram.y" /* yacc.c:1646  */
    { 
				   TypeName *n = makeNode(TypeName);
				   n->name = (yyvsp[0].str);
				   n->setof = TRUE;
				   n->arrayBounds = NULL;
				   (yyval.node) = (Node *)n;
				}
#line 2522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 504 "gram.y" /* yacc.c:1646  */
    { 
		    DestroyStmt *n = makeNode(DestroyStmt);
		    n->relNames = (yyvsp[0].list);
		    (yyval.node) = (Node *)n;
		}
#line 2532 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 520 "gram.y" /* yacc.c:1646  */
    {
		    FetchStmt *n = makeNode(FetchStmt);
		    n->direction = (yyvsp[-2].ival);
		    n->howMany = (yyvsp[-1].ival);
		    n->portalname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
	        }
#line 2544 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 529 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = FORWARD; }
#line 2550 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 530 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = BACKWARD; }
#line 2556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 531 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = FORWARD; /* default */ }
#line 2562 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 535 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].ival);
		 if ((yyvsp[0].ival) <= 0) elog(WARN,"Please specify nonnegative count for fetch"); }
#line 2569 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 537 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; /* 0 means fetch all tuples*/}
#line 2575 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 538 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; /*default*/ }
#line 2581 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 549 "gram.y" /* yacc.c:1646  */
    {
		(yyval.node) = (Node*)makeAclStmt((yyvsp[-5].str),(yyvsp[-3].list),(yyvsp[-1].str),'+');
		free((yyvsp[-5].str));
		free((yyvsp[-1].str));
           }
#line 2591 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 557 "gram.y" /* yacc.c:1646  */
    {
		 (yyval.str) = aclmakepriv("rwaR",0);
		}
#line 2599 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 561 "gram.y" /* yacc.c:1646  */
    {
		 (yyval.str) = aclmakepriv("rwaR",0);
		}
#line 2607 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 564 "gram.y" /* yacc.c:1646  */
    {
		(yyval.str) = (yyvsp[0].str);
		}
#line 2615 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 569 "gram.y" /* yacc.c:1646  */
    {
			(yyval.str) = aclmakepriv("",(yyvsp[0].chr));
			}
#line 2623 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 573 "gram.y" /* yacc.c:1646  */
    {
				(yyval.str) = aclmakepriv((yyvsp[-2].str),(yyvsp[0].chr));
				free((yyvsp[-2].str));
			}
#line 2632 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 579 "gram.y" /* yacc.c:1646  */
    {
		(yyval.chr) = ACL_MODE_RD_CHR;
		}
#line 2640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 582 "gram.y" /* yacc.c:1646  */
    {
		(yyval.chr) = ACL_MODE_AP_CHR;
		}
#line 2648 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 585 "gram.y" /* yacc.c:1646  */
    {
		(yyval.chr) = ACL_MODE_WR_CHR;
		}
#line 2656 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 588 "gram.y" /* yacc.c:1646  */
    {
		(yyval.chr) = ACL_MODE_WR_CHR;
		}
#line 2664 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 591 "gram.y" /* yacc.c:1646  */
    {
		(yyval.chr) = ACL_MODE_RU_CHR;
		}
#line 2672 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 596 "gram.y" /* yacc.c:1646  */
    {
		(yyval.str) = aclmakeuser("A","");
		}
#line 2680 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 599 "gram.y" /* yacc.c:1646  */
    {
		(yyval.str) = aclmakeuser("G",(yyvsp[0].str));
		}
#line 2688 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 602 "gram.y" /* yacc.c:1646  */
    {
		(yyval.str) = aclmakeuser("U",(yyvsp[0].str));
	 	}
#line 2696 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 609 "gram.y" /* yacc.c:1646  */
    {
                    yyerror("WITH GRANT OPTION is not supported.  Only relation owners can set privileges");
                }
#line 2704 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 621 "gram.y" /* yacc.c:1646  */
    {
		(yyval.node) = (Node*)makeAclStmt((yyvsp[-4].str),(yyvsp[-2].list),(yyvsp[0].str),'-');
		free((yyvsp[-4].str));
		free((yyvsp[0].str));
              }
#line 2714 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 636 "gram.y" /* yacc.c:1646  */
    { 
		    MoveStmt *n = makeNode(MoveStmt);
		    n->direction = (yyvsp[-2].ival);
		    n->to = FALSE;
		    n->where = (yyvsp[-1].ival);
		    n->portalname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 2727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 645 "gram.y" /* yacc.c:1646  */
    { 
		    MoveStmt *n = makeNode(MoveStmt);
		    n->direction = (yyvsp[-3].ival);
		    n->to = TRUE;
		    n->where = (yyvsp[-1].ival);
		    n->portalname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 2740 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 655 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].ival); }
#line 2746 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 656 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; /* default */ }
#line 2752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 659 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str);}
#line 2758 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 660 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = NULL; }
#line 2764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 676 "gram.y" /* yacc.c:1646  */
    {
		    /* should check that access_method is valid,
		       etc ... but doesn't */
		    IndexStmt *n = makeNode(IndexStmt);
		    n->unique = (yyvsp[-9].boolean);
		    n->idxname = (yyvsp[-7].str);
		    n->relname = (yyvsp[-5].str);
		    n->accessMethod = (yyvsp[-4].str);
		    n->indexParams = (yyvsp[-2].list);
		    n->withClause = (yyvsp[0].list);
		    n->whereClause = NULL;
		    (yyval.node) = (Node *)n;
		}
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 691 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 2788 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 693 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "btree"; }
#line 2794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 696 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = TRUE; }
#line 2800 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 697 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = FALSE; }
#line 2806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 708 "gram.y" /* yacc.c:1646  */
    {
		    ExtendStmt *n = makeNode(ExtendStmt);
		    n->idxname = (yyvsp[-1].str);
		    n->whereClause = (yyvsp[0].node);
		    (yyval.node) = (Node *)n;
		}
#line 2817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 724 "gram.y" /* yacc.c:1646  */
    {
		    RecipeStmt *n;
		    if (!IsTransactionBlock())
			elog(WARN, "EXECUTE RECIPE may only be used in begin/end transaction blocks.");

		    n = makeNode(RecipeStmt);
		    n->recipeName = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 2831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 754 "gram.y" /* yacc.c:1646  */
    {
		    ProcedureStmt *n = makeNode(ProcedureStmt);
		    n->funcname = (yyvsp[-8].str);
		    n->defArgs = (yyvsp[-7].list);
		    n->returnType = (Node *)(yyvsp[-5].node);
		    n->withClause = (yyvsp[-4].list);
		    n->as = (yyvsp[-2].str);
		    n->language = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 2846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 765 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 2852 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 766 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 2858 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 769 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[-1].list); }
#line 2864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 770 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 2870 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 786 "gram.y" /* yacc.c:1646  */
    { 
		    (yyvsp[0].pstmt)->relname = (yyvsp[-1].str);
		    (yyval.node) = (Node *)(yyvsp[0].pstmt);
		}
#line 2879 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 793 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.pstmt) = makeNode(PurgeStmt);
		    (yyval.pstmt)->beforeDate = (yyvsp[0].str);
		    (yyval.pstmt)->afterDate = NULL;
		}
#line 2889 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 799 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.pstmt) = makeNode(PurgeStmt);
		    (yyval.pstmt)->beforeDate = NULL;
		    (yyval.pstmt)->afterDate = (yyvsp[0].str);
		}
#line 2899 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 805 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.pstmt) = makeNode(PurgeStmt);
		    (yyval.pstmt)->beforeDate = (yyvsp[-1].str);
		    (yyval.pstmt)->afterDate = (yyvsp[0].str);
		}
#line 2909 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 811 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.pstmt) = makeNode(PurgeStmt);
		    (yyval.pstmt)->beforeDate = (yyvsp[0].str);
		    (yyval.pstmt)->afterDate = (yyvsp[-1].str);
		}
#line 2919 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 817 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.pstmt) = makeNode(PurgeStmt);
		    (yyval.pstmt)->beforeDate = NULL;
		    (yyval.pstmt)->afterDate = NULL;
		}
#line 2929 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 824 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 2935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 825 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 2941 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 844 "gram.y" /* yacc.c:1646  */
    {
		    RemoveStmt *n = makeNode(RemoveStmt);
		    n->removeType = (yyvsp[-1].ival);
		    n->name = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 2952 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 852 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = AGGREGATE; }
#line 2958 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 853 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = P_TYPE; }
#line 2964 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 854 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = INDEX; }
#line 2970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 855 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = RULE; }
#line 2976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 856 "gram.y" /* yacc.c:1646  */
    {  (yyval.ival) = VIEW; }
#line 2982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 860 "gram.y" /* yacc.c:1646  */
    {
		    RemoveFuncStmt *n = makeNode(RemoveFuncStmt);
		    n->funcname = (yyvsp[-3].str);
		    n->args = (yyvsp[-1].list);
		    (yyval.node) = (Node *)n;
	        }
#line 2993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 868 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 2999 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 869 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 3005 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 873 "gram.y" /* yacc.c:1646  */
    {
		    RemoveOperStmt *n = makeNode(RemoveOperStmt);
		    n->opname = (yyvsp[-3].str);
		    n->args = (yyvsp[-1].list);
		    (yyval.node) = (Node *)n;
		}
#line 3016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 883 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "+"; }
#line 3022 "y.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 884 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "-"; }
#line 3028 "y.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 885 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "*"; }
#line 3034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 886 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "/"; }
#line 3040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 887 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "<"; }
#line 3046 "y.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 888 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = ">"; }
#line 3052 "y.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 889 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "="; }
#line 3058 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 893 "gram.y" /* yacc.c:1646  */
    { 
		   elog(WARN, "parser: argument type missing (use NONE for unary operators)");
		}
#line 3066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 897 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = makeList(makeString((yyvsp[-2].str)), makeString((yyvsp[0].str)), -1); }
#line 3072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 899 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = makeList(NULL, makeString((yyvsp[0].str)), -1); }
#line 3078 "y.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 901 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = makeList(makeString((yyvsp[-2].str)), NULL, -1); }
#line 3084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 914 "gram.y" /* yacc.c:1646  */
    { 
		    RenameStmt *n = makeNode(RenameStmt);
		    n->relname = (yyvsp[-6].str);
		    n->inh = (yyvsp[-5].boolean);
		    n->column = (yyvsp[-2].str);
		    n->newname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 3097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 924 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 3103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 925 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = NULL; }
#line 3109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 928 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = COLUMN; }
#line 3115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 929 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 3121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 943 "gram.y" /* yacc.c:1646  */
    { QueryIsRule=TRUE; }
#line 3127 "y.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 946 "gram.y" /* yacc.c:1646  */
    {
		    RuleStmt *n = makeNode(RuleStmt);
		    n->rulename = (yyvsp[-10].str);
		    n->event = (yyvsp[-6].ival);
		    n->object = (yyvsp[-4].attr);
		    n->whereClause = (yyvsp[-3].node);
		    n->instead = (yyvsp[-1].boolean);
		    n->actions = (yyvsp[0].list);
		    (yyval.node) = (Node *)n;
		}
#line 3142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 958 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 3148 "y.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 959 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].node), NIL); }
#line 3154 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 960 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[-1].list); }
#line 3160 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 964 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = (yyvsp[0].list); }
#line 3166 "y.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 966 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].node), NIL); }
#line 3172 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 970 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[-1].node)); }
#line 3178 "y.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 972 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = lappend((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3184 "y.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 974 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[-1].node), NIL); }
#line 3190 "y.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 978 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.attr) = makeNode(Attr);
		    (yyval.attr)->relname = (yyvsp[-2].str);
		    (yyval.attr)->paramNo = NULL;
		    (yyval.attr)->attrs = lcons(makeString((yyvsp[0].str)), NIL);
		    (yyval.attr)->indirection = NIL;
		}
#line 3202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 986 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.attr) = makeNode(Attr);
		    (yyval.attr)->relname = (yyvsp[0].str);
		    (yyval.attr)->paramNo = NULL;
		    (yyval.attr)->attrs = NIL;
		    (yyval.attr)->indirection = NIL;
		}
#line 3214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 996 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = CMD_SELECT; }
#line 3220 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 997 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = CMD_UPDATE; }
#line 3226 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 998 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = CMD_DELETE; }
#line 3232 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 999 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = CMD_INSERT; }
#line 3238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1002 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = TRUE; }
#line 3244 "y.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 1003 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = FALSE; }
#line 3250 "y.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 1016 "gram.y" /* yacc.c:1646  */
    {
		    NotifyStmt *n = makeNode(NotifyStmt);
		    n->relname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 3260 "y.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 1024 "gram.y" /* yacc.c:1646  */
    {
		    ListenStmt *n = makeNode(ListenStmt);
		    n->relname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 3270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 1046 "gram.y" /* yacc.c:1646  */
    { 
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = ABORT_TRANS; 
		    (yyval.node) = (Node *)n;
		}
#line 3280 "y.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 1052 "gram.y" /* yacc.c:1646  */
    { 
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = BEGIN_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3290 "y.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 1058 "gram.y" /* yacc.c:1646  */
    {
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = BEGIN_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3300 "y.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 1064 "gram.y" /* yacc.c:1646  */
    {
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = END_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 1070 "gram.y" /* yacc.c:1646  */
    { 
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = END_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3320 "y.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 1076 "gram.y" /* yacc.c:1646  */
    {
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = ABORT_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3330 "y.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 1083 "gram.y" /* yacc.c:1646  */
    { 
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = ABORT_TRANS; 
		    (yyval.node) = (Node *)n;
		}
#line 3340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 1089 "gram.y" /* yacc.c:1646  */
    { 
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = BEGIN_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3350 "y.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 1095 "gram.y" /* yacc.c:1646  */
    {
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = END_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3360 "y.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 1102 "gram.y" /* yacc.c:1646  */
    { 
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = END_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3370 "y.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1108 "gram.y" /* yacc.c:1646  */
    {
		    TransactionStmt *n = makeNode(TransactionStmt); 
		    n->command = ABORT_TRANS;
		    (yyval.node) = (Node *)n;
		}
#line 3380 "y.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 1124 "gram.y" /* yacc.c:1646  */
    { 
		    ViewStmt *n = makeNode(ViewStmt);
		    n->viewname = (yyvsp[-2].str);
		    n->query = (Query *)(yyvsp[0].node);
		    (yyval.node) = (Node *)n;
		}
#line 3391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 1141 "gram.y" /* yacc.c:1646  */
    { 
		    LoadStmt *n = makeNode(LoadStmt);
		    n->filename = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 3401 "y.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 1157 "gram.y" /* yacc.c:1646  */
    {
		    CreatedbStmt *n = makeNode(CreatedbStmt);
		    n->dbname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 3411 "y.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 1173 "gram.y" /* yacc.c:1646  */
    {
		    DestroydbStmt *n = makeNode(DestroydbStmt);
		    n->dbname = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 3421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1189 "gram.y" /* yacc.c:1646  */
    {
		   ClusterStmt *n = makeNode(ClusterStmt);
		   n->relname = (yyvsp[0].str);
		   n->indexname = (yyvsp[-2].str);
		   (yyval.node) = (Node*)n;
		}
#line 3432 "y.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1205 "gram.y" /* yacc.c:1646  */
    {
                   VacuumStmt *n = makeNode(VacuumStmt);
                   n->verbose = (yyvsp[0].boolean);
                   (yyval.node) = (Node *)n;
               }
#line 3442 "y.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 1211 "gram.y" /* yacc.c:1646  */
    {
                   VacuumStmt *n = makeNode(VacuumStmt);
                   n->verbose = (yyvsp[-1].boolean);
                   n->vacrel = (yyvsp[0].str);
                   (yyval.node) = (Node *)n;
               }
#line 3453 "y.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 1219 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = TRUE; }
#line 3459 "y.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1220 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = FALSE; }
#line 3465 "y.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1231 "gram.y" /* yacc.c:1646  */
    {
		    ExplainStmt *n = makeNode(ExplainStmt);
		    n->verbose = (yyvsp[-1].boolean);
		    n->query = (Query*)(yyvsp[0].node);
		    (yyval.node) = (Node *)n;
		}
#line 3476 "y.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1267 "gram.y" /* yacc.c:1646  */
    {
		    (yyvsp[0].astmt)->relname = (yyvsp[-2].str);
		    (yyvsp[0].astmt)->cols = (yyvsp[-1].list);
		    (yyval.node) = (Node *)(yyvsp[0].astmt);
                }
#line 3486 "y.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1275 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.astmt) = makeNode(AppendStmt);
		    (yyval.astmt)->targetList = (yyvsp[-1].list);
		    (yyval.astmt)->fromClause = NIL;
		    (yyval.astmt)->whereClause = NULL;
		}
#line 3497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1282 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.astmt) = makeNode(AppendStmt);
		    (yyval.astmt)->targetList = (yyvsp[-2].list);
		    (yyval.astmt)->fromClause = (yyvsp[-1].list);
		    (yyval.astmt)->whereClause = (yyvsp[0].node);
		}
#line 3508 "y.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1290 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[-1].list); }
#line 3514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1291 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 3520 "y.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1296 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].node)); }
#line 3526 "y.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1298 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].node), NIL); }
#line 3532 "y.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1302 "gram.y" /* yacc.c:1646  */
    {
		    Ident *id = makeNode(Ident);
		    id->name = (yyvsp[-1].str);
		    id->indirection = (yyvsp[0].list);
		    (yyval.node) = (Node *)id;
		}
#line 3543 "y.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1319 "gram.y" /* yacc.c:1646  */
    {
		    DeleteStmt *n = makeNode(DeleteStmt);
		    n->relname = (yyvsp[-1].str);
		    n->whereClause = (yyvsp[0].node);
		    (yyval.node) = (Node *)n;
		}
#line 3554 "y.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1339 "gram.y" /* yacc.c:1646  */
    {
		    ReplaceStmt *n = makeNode(ReplaceStmt);
		    n->relname = (yyvsp[-4].str);
		    n->targetList = (yyvsp[-2].list);
		    n->fromClause = (yyvsp[-1].list);
		    n->whereClause = (yyvsp[0].node);
		    (yyval.node) = (Node *)n;
                }
#line 3567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1360 "gram.y" /* yacc.c:1646  */
    {
		    CursorStmt *n = makeNode(CursorStmt);

		    /* from PORTAL name */
		    /*
		     *  15 august 1991 -- since 3.0 postgres does locking
		     *  right, we discovered that portals were violating
		     *  locking protocol.  portal locks cannot span xacts.
		     *  as a short-term fix, we installed the check here. 
		     *				-- mao
		     */
		    if (!IsTransactionBlock())
			elog(WARN, "Named portals may only be used in begin/end transaction blocks.");

		    n->portalname = (yyvsp[-10].str);
		    n->binary = (yyvsp[-9].boolean);
		    n->unique = (yyvsp[-5].str);
		    n->targetList = (yyvsp[-4].list);
		    n->fromClause = (yyvsp[-3].list);
		    n->whereClause = (yyvsp[-2].node);
		    n->groupClause = (yyvsp[-1].list);
		    n->sortClause = (yyvsp[0].list);
		    (yyval.node) = (Node *)n;
		}
#line 3596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1398 "gram.y" /* yacc.c:1646  */
    {
		    RetrieveStmt *n = makeNode(RetrieveStmt);
		    n->unique = (yyvsp[-7].str);
		    n->targetList = (yyvsp[-6].list);
		    n->into = (yyvsp[-5].str);
		    n->fromClause = (yyvsp[-4].list);
		    n->whereClause = (yyvsp[-3].node);
		    n->groupClause = (yyvsp[-2].list);
		    n->havingClause = (yyvsp[-1].node);
		    n->sortClause = (yyvsp[0].list);
		    (yyval.node) = (Node *)n;
		}
#line 3613 "y.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1413 "gram.y" /* yacc.c:1646  */
    {  (yyval.str)= (yyvsp[0].str);  /* should check for archive level */  }
#line 3619 "y.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1415 "gram.y" /* yacc.c:1646  */
    {  (yyval.str) = NULL;  }
#line 3625 "y.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1418 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "*"; }
#line 3631 "y.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1419 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 3637 "y.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1420 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = NULL;}
#line 3643 "y.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1423 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 3649 "y.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1424 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 3655 "y.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1428 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].sortgroupby), NIL); }
#line 3661 "y.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1430 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].sortgroupby)); }
#line 3667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1434 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.sortgroupby) = makeNode(SortGroupBy);
		    (yyval.sortgroupby)->resno = 0;
		    (yyval.sortgroupby)->range = NULL;
		    (yyval.sortgroupby)->name = (yyvsp[-1].str);
		    (yyval.sortgroupby)->useOp = (yyvsp[0].str);
		}
#line 3679 "y.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1442 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.sortgroupby) = makeNode(SortGroupBy);
		    (yyval.sortgroupby)->resno = 0;
		    (yyval.sortgroupby)->range = (yyvsp[-3].str);
		    (yyval.sortgroupby)->name = (yyvsp[-1].str);
		    (yyval.sortgroupby)->useOp = (yyvsp[0].str);
		}
#line 3691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1450 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.sortgroupby) = makeNode(SortGroupBy);
		    (yyval.sortgroupby)->resno = (yyvsp[-1].ival);
		    (yyval.sortgroupby)->range = NULL;
		    (yyval.sortgroupby)->name = NULL;
		    (yyval.sortgroupby)->useOp = (yyvsp[0].str);
		}
#line 3703 "y.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1459 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 3709 "y.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1460 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "<"; }
#line 3715 "y.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1461 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = ">"; }
#line 3721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1462 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "<"; }
#line 3727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1463 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = ">"; }
#line 3733 "y.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1464 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "<"; /*default*/ }
#line 3739 "y.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1468 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].ielem),NIL); }
#line 3745 "y.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1469 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].ielem),NIL); }
#line 3751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1480 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.ielem) = makeNode(IndexElem);
		    (yyval.ielem)->name = (yyvsp[-5].str);
		    (yyval.ielem)->args = (yyvsp[-3].list);
		    (yyval.ielem)->class = (yyvsp[0].str);
	            (yyval.ielem)->tname = (yyvsp[-1].typnam);
		}
#line 3763 "y.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1490 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.ielem) = makeNode(IndexElem);
		    (yyval.ielem)->name = (yyvsp[-2].str);
		    (yyval.ielem)->args = NIL;
		    (yyval.ielem)->class = (yyvsp[0].str);
 	            (yyval.ielem)->tname = (yyvsp[-1].typnam);
		}
#line 3775 "y.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1499 "gram.y" /* yacc.c:1646  */
    { (yyval.typnam) = (yyvsp[0].typnam);}
#line 3781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1500 "gram.y" /* yacc.c:1646  */
    { (yyval.typnam) = NULL;}
#line 3787 "y.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1504 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 3793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1505 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = NULL; }
#line 3799 "y.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1517 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = TRUE; }
#line 3805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1518 "gram.y" /* yacc.c:1646  */
    { (yyval.boolean) = FALSE; }
#line 3811 "y.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1524 "gram.y" /* yacc.c:1646  */
    { (yyval.list)=lcons(makeString((yyvsp[0].str)),NIL); }
#line 3817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1526 "gram.y" /* yacc.c:1646  */
    { (yyval.list)=lappend((yyvsp[-2].list),makeString((yyvsp[0].str))); }
#line 3823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1529 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 3829 "y.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1530 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 3835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1533 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].sortgroupby), NIL); }
#line 3841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1534 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].sortgroupby)); }
#line 3847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1538 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.sortgroupby) = makeNode(SortGroupBy);
		    (yyval.sortgroupby)->resno = 0;
		    (yyval.sortgroupby)->range = NULL;
		    (yyval.sortgroupby)->name = (yyvsp[0].str);
		    (yyval.sortgroupby)->useOp = NULL;
		}
#line 3859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1546 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.sortgroupby) = makeNode(SortGroupBy);
		    (yyval.sortgroupby)->resno = 0;
		    (yyval.sortgroupby)->range = (yyvsp[-2].str);
		    (yyval.sortgroupby)->name = (yyvsp[0].str);
		    (yyval.sortgroupby)->useOp = NULL;
		}
#line 3871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1554 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.sortgroupby) = makeNode(SortGroupBy);
		    (yyval.sortgroupby)->resno = (yyvsp[0].ival);
		    (yyval.sortgroupby)->range = NULL;
		    (yyval.sortgroupby)->name = NULL;
		    (yyval.sortgroupby)->useOp = NULL;
		}
#line 3883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1563 "gram.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 3889 "y.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1564 "gram.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 3895 "y.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1575 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 3901 "y.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1576 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = NIL; }
#line 3907 "y.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1580 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].range)); }
#line 3913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1582 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].range), NIL); }
#line 3919 "y.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1586 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.range) = makeNode(RangeVar);
		    (yyval.range)->relExpr = (yyvsp[-2].relexp);
		    (yyval.range)->name = (yyvsp[0].str);
		}
#line 3929 "y.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1592 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.range) = makeNode(RangeVar);
		    (yyval.range)->relExpr = (yyvsp[-1].relexp);
		    (yyval.range)->name = (yyvsp[0].str);
		}
#line 3939 "y.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1598 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.range) = makeNode(RangeVar);
		    (yyval.range)->relExpr = (yyvsp[0].relexp);
		    (yyval.range)->name = NULL;
		}
#line 3949 "y.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1605 "gram.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 3955 "y.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1606 "gram.y" /* yacc.c:1646  */
    { (yyval.node) = NULL;  /* no qualifiers */ }
#line 3961 "y.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1610 "gram.y" /* yacc.c:1646  */
    { 
		    /* normal relations */
		    (yyval.relexp) = makeNode(RelExpr);
		    (yyval.relexp)->relname = (yyvsp[0].str);
		    (yyval.relexp)->inh = FALSE;
		    (yyval.relexp)->timeRange = NULL;
		}
#line 3973 "y.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1618 "gram.y" /* yacc.c:1646  */
    { 
		    /* inheiritance query */
		    (yyval.relexp) = makeNode(RelExpr);
		    (yyval.relexp)->relname = (yyvsp[-1].str);
		    (yyval.relexp)->inh = TRUE;
		    (yyval.relexp)->timeRange = NULL;
		}
#line 3985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1626 "gram.y" /* yacc.c:1646  */
    { 
		    /* time-qualified query */
		    (yyval.relexp) = makeNode(RelExpr);
		    (yyval.relexp)->relname = (yyvsp[-1].str);
		    (yyval.relexp)->inh = FALSE;
		    (yyval.relexp)->timeRange = (yyvsp[0].trange);
		}
#line 3997 "y.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1637 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.trange) = makeNode(TimeRange);
		    (yyval.trange)->startDate = (yyvsp[-3].str);
		    (yyval.trange)->endDate = (yyvsp[-1].str);
		}
#line 4007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1643 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.trange) = makeNode(TimeRange);
		    (yyval.trange)->startDate = (yyvsp[-1].str);
		    (yyval.trange)->endDate = NULL;
		}
#line 4017 "y.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1651 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "epoch"; }
#line 4023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1655 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = "now"; }
#line 4029 "y.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1659 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = lcons(makeInteger(-1), (yyvsp[0].list)); }
#line 4035 "y.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1661 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = lcons(makeInteger((yyvsp[-2].ival)), (yyvsp[0].list)); }
#line 4041 "y.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1663 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = NIL; }
#line 4047 "y.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1667 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = lcons(makeInteger(-1), (yyvsp[0].list)); }
#line 4053 "y.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1669 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = lcons(makeInteger((yyvsp[-2].ival)), (yyvsp[0].list)); }
#line 4059 "y.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1671 "gram.y" /* yacc.c:1646  */
    {  (yyval.list) = NIL; }
#line 4065 "y.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1675 "gram.y" /* yacc.c:1646  */
    {
		    char *tname = xlateSqlType((yyvsp[0].str));
		    (yyval.typnam) = makeNode(TypeName);
		    (yyval.typnam)->name = tname;

		    /* Is this the name of a complex type? If so, implement
		     * it as a set.
		     */
		    if (!strcmp(saved_relname, tname)) {
		 	/* This attr is the same type as the relation 
			 * being defined. The classic example: create
			 * emp(name=text,mgr=emp)
			 */
			(yyval.typnam)->setof = TRUE;
		    }else if (get_typrelid((Type)type(tname))
				!= InvalidOid) {
			 /* (Eventually add in here that the set can only 
			  * contain one element.)
			  */
			(yyval.typnam)->setof = TRUE;
	    	    } else {
			(yyval.typnam)->setof = FALSE;
		    }
		}
#line 4094 "y.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1700 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.typnam) = makeNode(TypeName);
		    (yyval.typnam)->name = (yyvsp[0].str);
		    (yyval.typnam)->setof = TRUE;
		}
#line 4104 "y.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1708 "gram.y" /* yacc.c:1646  */
    { 
		    (yyval.typnam) = (yyvsp[-1].typnam);
		    (yyval.typnam)->arrayBounds = (yyvsp[0].list);
		}
#line 4113 "y.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1713 "gram.y" /* yacc.c:1646  */
    {
		    /*
		     * The following implements char() and varchar().
		     * We do it here instead of the 'typname:' production
		     * because we don't want to allow arrays of varchar().
		     * I haven't thought about whether that will work or not.
		      *                             - ay 6/95
		     */
		    (yyval.typnam) = makeNode(TypeName);
		    if (!strcasecmp((yyvsp[-3].str), "char")) {
			(yyval.typnam)->name = "bpchar"; /*  strdup("bpchar"); */
		    } else if (!strcasecmp((yyvsp[-3].str), "varchar")) {
			(yyval.typnam)->name = "varchar"; /* strdup("varchar"); */
		    } else {
			yyerror("parse error");
		    }
		    if ((yyvsp[-1].ival) < 1) {
			elog(WARN, "length for '%s' type must be at least 1",
			     (yyvsp[-3].str));
		    } else if ((yyvsp[-1].ival) > 4096) {
			/* we can store a char() of length up to the size
			   of a page (8KB) - page headers and friends but
			   just to be safe here...  - ay 6/95 */
			elog(WARN, "length for '%s' type cannot exceed 4096",
			     (yyvsp[-3].str));
		    }
		    /* we actually implement this sort of like a varlen, so
		       the first 4 bytes is the length. (the difference
		       between this and "text" is that we blank-pad and 
		       truncate where necessary */
		    (yyval.typnam)->typlen = 4 + (yyvsp[-1].ival);
		}
#line 4150 "y.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1755 "gram.y" /* yacc.c:1646  */
    {
		    (yyvsp[-1].attr)->indirection = (yyvsp[0].list);
		    (yyval.node) = (Node *)(yyvsp[-1].attr);
		}
#line 4159 "y.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1760 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = (yyvsp[0].node);  }
#line 4165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1762 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "-", NULL, (yyvsp[0].node)); }
#line 4171 "y.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1764 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "+", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1766 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "-", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4183 "y.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1768 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "/", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1770 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "*", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1772 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "<", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4201 "y.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1774 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, ">", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4207 "y.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1776 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "=", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1778 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, ":", NULL, (yyvsp[0].node)); }
#line 4219 "y.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1780 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, ";", NULL, (yyvsp[0].node)); }
#line 4225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1782 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "|", NULL, (yyvsp[0].node)); }
#line 4231 "y.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1784 "gram.y" /* yacc.c:1646  */
    { 
		    /* AexprConst can be either A_Const or ParamNo */
		    if (nodeTag((yyvsp[-2].node)) == T_A_Const) {
			((A_Const *)(yyvsp[-2].node))->typename = (yyvsp[0].typnam);
		    }else {
			((ParamNo *)(yyvsp[-2].node))->typename = (yyvsp[0].typnam);
		    }
		    (yyval.node) = (Node *)(yyvsp[-2].node);
		}
#line 4245 "y.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1794 "gram.y" /* yacc.c:1646  */
    {
		    /* AexprConst can be either A_Const or ParamNo */
		    if (nodeTag((yyvsp[-2].node)) == T_A_Const) {
			((A_Const *)(yyvsp[-2].node))->typename = (yyvsp[0].typnam);
		    }else {
			((ParamNo *)(yyvsp[-2].node))->typename = (yyvsp[0].typnam);
		    }
		    (yyval.node) = (Node *)(yyvsp[-2].node);
		}
#line 4259 "y.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1804 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = (yyvsp[-1].node); }
#line 4265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1806 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, (yyvsp[-1].str), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1808 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "~~", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1810 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "!~~", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 4283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1812 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, (yyvsp[-1].str), NULL, (yyvsp[0].node)); }
#line 4289 "y.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1814 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, (yyvsp[0].str), (yyvsp[-1].node), NULL); }
#line 4295 "y.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1816 "gram.y" /* yacc.c:1646  */
    {   /* could be a column name or a relation_name */
		    Ident *n = makeNode(Ident);
		    n->name = (yyvsp[0].str);
		    n->indirection = NULL;
		    (yyval.node) = (Node *)n;
		}
#line 4306 "y.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1823 "gram.y" /* yacc.c:1646  */
    {
		    FuncCall *n = makeNode(FuncCall);
		    Ident *star = makeNode(Ident);

		    /* cheap hack for aggregate (eg. count) */
		    star->name = "oid"; 
		    n->funcname = (yyvsp[-3].str);
		    n->args = lcons(star, NIL);
		    (yyval.node) = (Node *)n;
		}
#line 4321 "y.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1834 "gram.y" /* yacc.c:1646  */
    {
		    FuncCall *n = makeNode(FuncCall);
		    n->funcname = (yyvsp[-2].str);
		    n->args = NIL;
		    (yyval.node) = (Node *)n;
		}
#line 4332 "y.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1841 "gram.y" /* yacc.c:1646  */
    {
		    FuncCall *n = makeNode(FuncCall);
		    n->funcname = (yyvsp[-3].str);
		    n->args = (yyvsp[-1].list);
		    (yyval.node) = (Node *)n;
		}
#line 4343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1848 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(ISNULL, NULL, (yyvsp[-1].node), NULL); }
#line 4349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1850 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(ISNULL, NULL, (yyvsp[-2].node), NULL); }
#line 4355 "y.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1852 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(NOTNULL, NULL, (yyvsp[-1].node), NULL); }
#line 4361 "y.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1854 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(NOTNULL, NULL, (yyvsp[-3].node), NULL); }
#line 4367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1856 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(AND, NULL,
			makeA_Expr(OP, ">=", (yyvsp[-4].node), (yyvsp[-2].node)),
			makeA_Expr(OP, "<=", (yyvsp[-4].node),(yyvsp[0].node)));
		}
#line 4376 "y.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1861 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OR, NULL,
			makeA_Expr(OP, "<", (yyvsp[-5].node), (yyvsp[-2].node)),
			makeA_Expr(OP, ">", (yyvsp[-5].node), (yyvsp[0].node)));
		}
#line 4385 "y.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1865 "gram.y" /* yacc.c:1646  */
    { saved_In_Expr = (yyvsp[-1].node); }
#line 4391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1866 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = (yyvsp[-1].node); }
#line 4397 "y.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1867 "gram.y" /* yacc.c:1646  */
    { saved_In_Expr = (yyvsp[-2].node); }
#line 4403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1868 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = (yyvsp[-1].node); }
#line 4409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1870 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(AND, NULL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4415 "y.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1872 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OR, NULL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 4421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1874 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(NOT, NULL, NULL, (yyvsp[0].node)); }
#line 4427 "y.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1878 "gram.y" /* yacc.c:1646  */
    {
		    A_Indices *ai = makeNode(A_Indices);
		    ai->lidx = NULL;
		    ai->uidx = (yyvsp[-2].node);
		    (yyval.list) = lcons(ai, (yyvsp[0].list));
		}
#line 4438 "y.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1885 "gram.y" /* yacc.c:1646  */
    {
		    A_Indices *ai = makeNode(A_Indices);
		    ai->lidx = (yyvsp[-4].node);
		    ai->uidx = (yyvsp[-2].node);
		    (yyval.list) = lcons(ai, (yyvsp[0].list));
		}
#line 4449 "y.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1892 "gram.y" /* yacc.c:1646  */
    {   (yyval.list) = NIL; }
#line 4455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1896 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons((yyvsp[0].node), NIL); }
#line 4461 "y.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1898 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].node)); }
#line 4467 "y.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1902 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "=", saved_In_Expr, (yyvsp[0].node)); }
#line 4473 "y.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1904 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OR, NULL, (yyvsp[-2].node),
			makeA_Expr(OP, "=", saved_In_Expr, (yyvsp[0].node)));
		}
#line 4481 "y.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1910 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(OP, "<>", saved_In_Expr, (yyvsp[0].node)); }
#line 4487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1912 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = makeA_Expr(AND, NULL, (yyvsp[-2].node),
			makeA_Expr(OP, "<>", saved_In_Expr, (yyvsp[0].node)));
		}
#line 4495 "y.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1918 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.attr) = makeNode(Attr);
		    (yyval.attr)->relname = (yyvsp[-2].str);
		    (yyval.attr)->paramNo = NULL;
		    (yyval.attr)->attrs = (yyvsp[0].list);
		    (yyval.attr)->indirection = NULL;
		}
#line 4507 "y.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1926 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.attr) = makeNode(Attr);
		    (yyval.attr)->relname = NULL;
		    (yyval.attr)->paramNo = (yyvsp[-2].paramno);
		    (yyval.attr)->attrs = (yyvsp[0].list);
		    (yyval.attr)->indirection = NULL;
		}
#line 4519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1936 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lcons(makeString((yyvsp[0].str)), NIL); }
#line 4525 "y.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1938 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), makeString((yyvsp[0].str))); }
#line 4531 "y.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1940 "gram.y" /* yacc.c:1646  */
    { (yyval.list) = lappend((yyvsp[-2].list), makeString("*")); }
#line 4537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1951 "gram.y" /* yacc.c:1646  */
    {   (yyval.list) = lappend((yyvsp[-2].list),(yyvsp[0].target));  }
#line 4543 "y.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1953 "gram.y" /* yacc.c:1646  */
    {   (yyval.list) = lcons((yyvsp[0].target), NIL);  }
#line 4549 "y.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1955 "gram.y" /* yacc.c:1646  */
    {
		    ResTarget *rt = makeNode(ResTarget);
		    Attr *att = makeNode(Attr);
		    att->relname = "*";
		    att->paramNo = NULL;
		    att->attrs = NULL;
		    att->indirection = NIL;
		    rt->name = NULL;
		    rt->indirection = NULL;
		    rt->val = (Node *)att;
		    (yyval.list) = lcons(rt, NIL);
		}
#line 4566 "y.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1970 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.target) = makeNode(ResTarget);
		    (yyval.target)->name = (yyvsp[-3].str);
		    (yyval.target)->indirection = (yyvsp[-2].list);
		    (yyval.target)->val = (Node *)(yyvsp[0].node);
		}
#line 4577 "y.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1977 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.target) = makeNode(ResTarget);
		    (yyval.target)->name = NULL;
		    (yyval.target)->indirection = (yyvsp[0].list);
		    (yyval.target)->val = (Node *)(yyvsp[-1].attr);
		}
#line 4588 "y.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1984 "gram.y" /* yacc.c:1646  */
    {   
		    Attr *att = makeNode(Attr);
		    att->relname = (yyvsp[-2].str);
		    att->paramNo = NULL;
		    att->attrs = lcons(makeString("*"), NIL);
		    att->indirection = NIL;
		    (yyval.target) = makeNode(ResTarget);
		    (yyval.target)->name = NULL;
		    (yyval.target)->indirection = NULL;
		    (yyval.target)->val = (Node *)att;
		}
#line 4604 "y.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 2004 "gram.y" /* yacc.c:1646  */
    {   (yyval.list) = lappend((yyvsp[-2].list), (yyvsp[0].target));  }
#line 4610 "y.tab.c" /* yacc.c:1646  */
    break;

  case 353:
#line 2006 "gram.y" /* yacc.c:1646  */
    {   (yyval.list) = lcons((yyvsp[0].target), NIL);  }
#line 4616 "y.tab.c" /* yacc.c:1646  */
    break;

  case 354:
#line 2011 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.target) = makeNode(ResTarget);
		    (yyval.target)->name = (yyvsp[0].str);
		    (yyval.target)->indirection = NULL;
		    (yyval.target)->val = (Node *)(yyvsp[-2].node);
		}
#line 4627 "y.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 2018 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.target) = makeNode(ResTarget);
		    (yyval.target)->name = NULL;
		    (yyval.target)->indirection = NULL;
		    (yyval.target)->val = (Node *)(yyvsp[0].node);
		}
#line 4638 "y.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 2025 "gram.y" /* yacc.c:1646  */
    {
		    Attr *att = makeNode(Attr);
		    att->relname = (yyvsp[-2].str);
		    att->paramNo = NULL;
		    att->attrs = lcons(makeString("*"), NIL);
		    att->indirection = NIL;
		    (yyval.target) = makeNode(ResTarget);
		    (yyval.target)->name = NULL;
		    (yyval.target)->indirection = NULL;
		    (yyval.target)->val = (Node *)att;
		}
#line 4654 "y.tab.c" /* yacc.c:1646  */
    break;

  case 357:
#line 2037 "gram.y" /* yacc.c:1646  */
    {
		    Attr *att = makeNode(Attr);
		    att->relname = "*";
		    att->paramNo = NULL;
		    att->attrs = NULL;
		    att->indirection = NIL;
		    (yyval.target) = makeNode(ResTarget);
		    (yyval.target)->name = NULL;
		    (yyval.target)->indirection = NULL;
		    (yyval.target)->val = (Node *)att;
		}
#line 4670 "y.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 2050 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 2051 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = NULL; }
#line 4682 "y.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 2055 "gram.y" /* yacc.c:1646  */
    {
                   (yyval.str) = (yyvsp[0].str);
                   strncpy(saved_relname, (yyvsp[0].str), NAMEDATALEN);
                   saved_relname[NAMEDATALEN-1] = '\0';
	        }
#line 4692 "y.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 2061 "gram.y" /* yacc.c:1646  */
    {
		    /* disallow refs to magic system tables */
  		    if (strcmp(LogRelationName, (yyvsp[0].str)) == 0
  		       || strcmp(VariableRelationName, (yyvsp[0].str)) == 0
  		       || strcmp(TimeRelationName, (yyvsp[0].str)) == 0
  		       || strcmp(MagicRelationName, (yyvsp[0].str)) == 0) {
			elog(WARN, "%s cannot be accessed by users", (yyvsp[0].str));
		    } else {
			(yyval.str) = (yyvsp[0].str);
		    }
                    strncpy(saved_relname, (yyvsp[0].str), NAMEDATALEN);
                    saved_relname[NAMEDATALEN-1] = '\0';
		}
#line 4710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 2076 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4716 "y.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 2077 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4722 "y.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 2078 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4728 "y.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 2079 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4734 "y.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 2080 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4740 "y.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 2081 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4746 "y.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 2082 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 2084 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4758 "y.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 2085 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 2086 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 2089 "gram.y" /* yacc.c:1646  */
    {  
		    A_Const *n = makeNode(A_Const);
		    n->val.type = T_Integer;
		    n->val.val.ival = (yyvsp[0].ival);
		    (yyval.node) = (Node *)n;
		}
#line 4781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 2096 "gram.y" /* yacc.c:1646  */
    {  
		    A_Const *n = makeNode(A_Const);
		    n->val.type = T_Float;
		    n->val.val.dval = (yyvsp[0].dval);
		    (yyval.node) = (Node *)n;
		}
#line 4792 "y.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 2103 "gram.y" /* yacc.c:1646  */
    {  
		    A_Const *n = makeNode(A_Const);
		    n->val.type = T_String;
		    n->val.val.str = (yyvsp[0].str);
		    (yyval.node) = (Node *)n;
		}
#line 4803 "y.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 2110 "gram.y" /* yacc.c:1646  */
    {   (yyval.node) = (Node *)(yyvsp[0].paramno);  }
#line 4809 "y.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 2112 "gram.y" /* yacc.c:1646  */
    {	
		    A_Const *n = makeNode(A_Const);
		    n->val.type = T_Null;
		    (yyval.node) = (Node *)n;
		}
#line 4819 "y.tab.c" /* yacc.c:1646  */
    break;

  case 377:
#line 2120 "gram.y" /* yacc.c:1646  */
    {
		    (yyval.paramno) = makeNode(ParamNo);
		    (yyval.paramno)->number = (yyvsp[0].ival);
		}
#line 4828 "y.tab.c" /* yacc.c:1646  */
    break;

  case 378:
#line 2126 "gram.y" /* yacc.c:1646  */
    { (yyval.value) = makeInteger((yyvsp[0].ival)); }
#line 4834 "y.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 2127 "gram.y" /* yacc.c:1646  */
    { (yyval.value) = makeFloat((yyvsp[0].dval)); }
#line 4840 "y.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 2130 "gram.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].ival); }
#line 4846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 2131 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4852 "y.tab.c" /* yacc.c:1646  */
    break;

  case 382:
#line 2133 "gram.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 4858 "y.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 2136 "gram.y" /* yacc.c:1646  */
    { 
		    if (QueryIsRule)
			(yyval.str) = "*CURRENT*";
		    else 
			elog(WARN,"CURRENT used in non-rule query");
		}
#line 4869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 2143 "gram.y" /* yacc.c:1646  */
    { 
		    if (QueryIsRule)
			(yyval.str) = "*NEW*";
		    else 
			elog(WARN,"NEW used in non-rule query"); 
		}
#line 4880 "y.tab.c" /* yacc.c:1646  */
    break;


#line 4884 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 2155 "gram.y" /* yacc.c:1906  */


static Node *makeA_Expr(int oper, char *opname, Node *lexpr, Node *rexpr)
{
    A_Expr *a = makeNode(A_Expr);
    a->oper = oper;
    a->opname = opname;
    a->lexpr = lexpr;
    a->rexpr = rexpr;
    return (Node *)a;
}

static char *
xlateSqlType(char *name)
{
    if (!strcasecmp(name,"int") ||
	!strcasecmp(name,"integer"))
	return "int4"; /* strdup("int4") --   strdup leaks memory here */
    else if (!strcasecmp(name, "smallint"))
	return "int2";
    else if (!strcasecmp(name, "float") ||
	     !strcasecmp(name, "real"))
	return "float8";
    else
	return name;
}

void parser_init(Oid *typev, int nargs)
{
    QueryIsRule = false;
    saved_relname[0]= '\0';
    saved_In_Expr = NULL;
    
    param_type_init(typev, nargs);
}

