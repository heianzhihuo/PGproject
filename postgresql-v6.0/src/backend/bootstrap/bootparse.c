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

/* All symbols defined below should begin with Int_yy or YY, to avoid
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
#line 1 "bootparse.y" /* yacc.c:339  */

/*-------------------------------------------------------------------------
 *
 * backendparse.y--
 *    yacc parser grammer for the "backend" initialization program.
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /usr/local/cvsroot/postgres95/src/backend/bootstrap/bootparse.y,v 1.4 1996/11/13 20:47:45 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "catalog/pg_attribute.h"
#include "access/attnum.h" 
#include "nodes/pg_list.h"
#include "access/tupdesc.h" 
#include "storage/fd.h"
#include "catalog/pg_am.h" 
#include "catalog/pg_class.h"
#include "nodes/nodes.h"
#include "rewrite/prs2lock.h"
#include "access/skey.h"
#include "access/strat.h"
#include "utils/rel.h"

#include "nodes/primnodes.h"
#include <time.h>
#include "utils/nabstime.h"
#include "storage/block.h"
#include "storage/off.h"
#include "storage/itemptr.h"
#include "access/htup.h"
#include "utils/tqual.h"
#include "nodes/parsenodes.h"

#include "access/xact.h"

#include <stdio.h>

#include "catalog/heap.h"

#include "storage/ipc.h"
#include "storage/spin.h"
#include "storage/smgr.h"

#include "tcop/dest.h"
#include "commands/defrem.h"

#include "access/itup.h"
#include "access/funcindex.h"
#include "bootstrap/bootstrap.h"

#include "miscadmin.h"

#define DO_START { StartTransactionCommand();\
		 }

#define DO_END   { CommitTransactionCommand();\
		   if (!Quiet) { EMITPROMPT; }\
		   fflush(stdout); \
		 }

int num_tuples_read = 0;
static Oid objectid;


#line 138 "y.tab.c" /* yacc.c:339  */

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
extern int Int_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum Int_yytokentype
  {
    CONST = 258,
    ID = 259,
    OPEN = 260,
    XCLOSE = 261,
    XCREATE = 262,
    INSERT_TUPLE = 263,
    STRING = 264,
    XDEFINE = 265,
    XDECLARE = 266,
    INDEX = 267,
    ON = 268,
    USING = 269,
    XBUILD = 270,
    INDICES = 271,
    COMMA = 272,
    EQUALS = 273,
    LPAREN = 274,
    RPAREN = 275,
    OBJ_ID = 276,
    XBOOTSTRAP = 277,
    NULLVAL = 278,
    low = 279,
    high = 280
  };
#endif
/* Tokens.  */
#define CONST 258
#define ID 259
#define OPEN 260
#define XCLOSE 261
#define XCREATE 262
#define INSERT_TUPLE 263
#define STRING 264
#define XDEFINE 265
#define XDECLARE 266
#define INDEX 267
#define ON 268
#define USING 269
#define XBUILD 270
#define INDICES 271
#define COMMA 272
#define EQUALS 273
#define LPAREN 274
#define RPAREN 275
#define OBJ_ID 276
#define XBOOTSTRAP 277
#define NULLVAL 278
#define low 279
#define high 280

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 73 "bootparse.y" /* yacc.c:355  */

    List     *list;
    IndexElem	*ielem;
    char     *str;
    int		ival;

#line 235 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE Int_yylval;

int Int_yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 252 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 Int_yytype_uint8;
#else
typedef unsigned char Int_yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 Int_yytype_int8;
#else
typedef signed char Int_yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 Int_yytype_uint16;
#else
typedef unsigned short int Int_yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 Int_yytype_int16;
#else
typedef short int Int_yytype_int16;
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
/* Suppress an incorrect diagnostic about Int_yylval being uninitialized.  */
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


#if ! defined Int_yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined Int_yyoverflow || YYERROR_VERBOSE */


#if (! defined Int_yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union Int_yyalloc
{
  Int_yytype_int16 Int_yyss_alloc;
  YYSTYPE Int_yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union Int_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (Int_yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T Int_yynewbytes;                                            \
        YYCOPY (&Int_yyptr->Stack_alloc, Stack, Int_yysize);                    \
        Stack = &Int_yyptr->Stack_alloc;                                    \
        Int_yynewbytes = Int_yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        Int_yyptr += Int_yynewbytes / sizeof (*Int_yyptr);                          \
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
          YYSIZE_T Int_yyi;                         \
          for (Int_yyi = 0; Int_yyi < (Count); Int_yyi++)   \
            (Dst)[Int_yyi] = (Src)[Int_yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   66

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  26
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  41
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  70

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by Int_yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? Int_yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by Int_yylex, without out-of-bounds checking.  */
static const Int_yytype_uint8 Int_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const Int_yytype_uint16 Int_yyrline[] =
{
       0,    99,    99,   100,   104,   105,   109,   110,   111,   112,
     113,   114,   118,   127,   133,   143,   148,   142,   196,   195,
     224,   240,   243,   251,   257,   266,   270,   276,   277,   281,
     282,   286,   297,   298,   302,   303,   304,   308,   309,   310,
     315,   319
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const Int_yytname[] =
{
  "$end", "error", "$undefined", "CONST", "ID", "OPEN", "XCLOSE",
  "XCREATE", "INSERT_TUPLE", "STRING", "XDEFINE", "XDECLARE", "INDEX",
  "ON", "USING", "XBUILD", "INDICES", "COMMA", "EQUALS", "LPAREN",
  "RPAREN", "OBJ_ID", "XBOOTSTRAP", "NULLVAL", "low", "high", "$accept",
  "TopLevel", "Queries", "Query", "OpenStmt", "CloseStmt", "CreateStmt",
  "$@1", "$@2", "InsertStmt", "$@3", "DeclareIndexStmt", "BuildIndsStmt",
  "index_params", "index_on", "arg_list", "optbootstrap", "typelist",
  "typething", "optoideq", "tuplelist", "tuple", "const", "ident", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const Int_yytype_uint16 Int_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280
};
# endif

#define YYPACT_NINF -32

#define Int_yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-32)))

#define YYTABLE_NINF -1

#define Int_yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const Int_yytype_int8 Int_yypact[] =
{
       3,    -2,    -2,   -10,     5,     9,     8,    28,     3,   -32,
     -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,
      -2,    11,   -32,    -2,   -32,   -32,   -32,    12,    -2,    13,
      17,   -32,   -32,     2,    -2,    -2,   -32,   -32,     0,   -32,
     -32,   -32,    21,    19,   -32,    20,     2,   -32,   -32,    -2,
      -2,    22,    -2,   -32,    18,   -32,   -32,   -32,    -2,    23,
      -2,    25,   -32,   -32,    -2,    -4,   -32,    -2,   -32,   -32
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const Int_yytype_uint8 Int_yydefact[] =
{
       3,     0,    14,    28,    33,     0,     0,     0,     2,     4,
       6,     7,     8,     9,    10,    11,    41,    12,    13,    27,
       0,     0,    18,     0,    21,     1,     5,     0,     0,     0,
       0,    15,    32,     0,     0,     0,    40,    39,     0,    34,
      38,    37,     0,    16,    29,     0,     0,    19,    35,     0,
       0,     0,     0,    36,     0,    30,    17,    31,     0,     0,
       0,    23,    20,    22,     0,     0,    25,     0,    24,    26
};

  /* YYPGOTO[NTERM-NUM].  */
static const Int_yytype_int8 Int_yypgoto[] =
{
     -32,   -32,   -32,    31,   -32,   -32,   -32,   -32,   -32,   -32,
     -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,    -9,   -32,
     -32,   -31,   -32,    -1
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const Int_yytype_int8 Int_yydefgoto[] =
{
      -1,     7,     8,     9,    10,    11,    12,    35,    51,    13,
      29,    14,    15,    59,    60,    65,    20,    43,    44,    22,
      38,    39,    40,    41
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const Int_yytype_uint8 Int_yytable[] =
{
      17,    18,    16,    36,    16,    36,    16,    48,     1,     2,
       3,     4,    19,    67,     5,    53,    68,    46,     6,    27,
      47,    23,    30,    37,    24,    37,    21,    32,    25,    28,
      34,    31,    33,    42,    45,    49,    50,    58,    52,    26,
       0,    55,    56,    62,    64,     0,     0,     0,    54,    45,
       0,    57,     0,     0,     0,     0,     0,    61,     0,    63,
       0,     0,     0,    66,     0,     0,    69
};

static const Int_yytype_int8 Int_yycheck[] =
{
       1,     2,     4,     3,     4,     3,     4,    38,     5,     6,
       7,     8,    22,    17,    11,    46,    20,    17,    15,    20,
      20,    12,    23,    23,    16,    23,    21,    28,     0,    18,
      13,    19,    19,    34,    35,    14,    17,    19,    18,     8,
      -1,    50,    20,    20,    19,    -1,    -1,    -1,    49,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    58,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    67
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const Int_yytype_uint8 Int_yystos[] =
{
       0,     5,     6,     7,     8,    11,    15,    27,    28,    29,
      30,    31,    32,    35,    37,    38,     4,    49,    49,    22,
      42,    21,    45,    12,    16,     0,    29,    49,    18,    36,
      49,    19,    49,    19,    13,    33,     3,    23,    46,    47,
      48,    49,    49,    43,    44,    49,    17,    20,    47,    14,
      17,    34,    18,    47,    49,    44,    20,    49,    19,    39,
      40,    49,    20,    49,    19,    41,    49,    17,    20,    49
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const Int_yytype_uint8 Int_yyr1[] =
{
       0,    26,    27,    27,    28,    28,    29,    29,    29,    29,
      29,    29,    30,    31,    31,    33,    34,    32,    36,    35,
      37,    38,    39,    40,    40,    41,    41,    42,    42,    43,
      43,    44,    45,    45,    46,    46,    46,    47,    47,    47,
      48,    49
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const Int_yytype_uint8 Int_yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     0,     0,     8,     0,     6,
      10,     2,     2,     1,     4,     1,     3,     1,     0,     1,
       3,     3,     3,     0,     1,     2,     3,     1,     1,     1,
       1,     1
};


#define Int_yyerrok         (Int_yyerrstatus = 0)
#define Int_yyclearin       (Int_yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto Int_yyacceptlab
#define YYABORT         goto Int_yyabortlab
#define YYERROR         goto Int_yyerrorlab


#define YYRECOVERING()  (!!Int_yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (Int_yychar == YYEMPTY)                                        \
    {                                                           \
      Int_yychar = (Token);                                         \
      Int_yylval = (Value);                                         \
      YYPOPSTACK (Int_yylen);                                       \
      Int_yystate = *Int_yyssp;                                         \
      goto Int_yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      Int_yyerror (YY_("syntax error: cannot back up")); \
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
  if (Int_yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (Int_yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      Int_yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
Int_yy_symbol_value_print (FILE *Int_yyoutput, int Int_yytype, YYSTYPE const * const Int_yyvaluep)
{
  FILE *Int_yyo = Int_yyoutput;
  YYUSE (Int_yyo);
  if (!Int_yyvaluep)
    return;
# ifdef YYPRINT
  if (Int_yytype < YYNTOKENS)
    YYPRINT (Int_yyoutput, Int_yytoknum[Int_yytype], *Int_yyvaluep);
# endif
  YYUSE (Int_yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
Int_yy_symbol_print (FILE *Int_yyoutput, int Int_yytype, YYSTYPE const * const Int_yyvaluep)
{
  YYFPRINTF (Int_yyoutput, "%s %s (",
             Int_yytype < YYNTOKENS ? "token" : "nterm", Int_yytname[Int_yytype]);

  Int_yy_symbol_value_print (Int_yyoutput, Int_yytype, Int_yyvaluep);
  YYFPRINTF (Int_yyoutput, ")");
}

/*------------------------------------------------------------------.
| Int_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
Int_yy_stack_print (Int_yytype_int16 *Int_yybottom, Int_yytype_int16 *Int_yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; Int_yybottom <= Int_yytop; Int_yybottom++)
    {
      int Int_yybot = *Int_yybottom;
      YYFPRINTF (stderr, " %d", Int_yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (Int_yydebug)                                                  \
    Int_yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
Int_yy_reduce_print (Int_yytype_int16 *Int_yyssp, YYSTYPE *Int_yyvsp, int Int_yyrule)
{
  unsigned long int Int_yylno = Int_yyrline[Int_yyrule];
  int Int_yynrhs = Int_yyr2[Int_yyrule];
  int Int_yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             Int_yyrule - 1, Int_yylno);
  /* The symbols being reduced.  */
  for (Int_yyi = 0; Int_yyi < Int_yynrhs; Int_yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", Int_yyi + 1);
      Int_yy_symbol_print (stderr,
                       Int_yystos[Int_yyssp[Int_yyi + 1 - Int_yynrhs]],
                       &(Int_yyvsp[(Int_yyi + 1) - (Int_yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (Int_yydebug)                          \
    Int_yy_reduce_print (Int_yyssp, Int_yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int Int_yydebug;
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

# ifndef Int_yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define Int_yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
Int_yystrlen (const char *Int_yystr)
{
  YYSIZE_T Int_yylen;
  for (Int_yylen = 0; Int_yystr[Int_yylen]; Int_yylen++)
    continue;
  return Int_yylen;
}
#  endif
# endif

# ifndef Int_yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define Int_yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
Int_yystpcpy (char *Int_yydest, const char *Int_yysrc)
{
  char *Int_yyd = Int_yydest;
  const char *Int_yys = Int_yysrc;

  while ((*Int_yyd++ = *Int_yys++) != '\0')
    continue;

  return Int_yyd - 1;
}
#  endif
# endif

# ifndef Int_yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for Int_yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from Int_yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
Int_yytnamerr (char *Int_yyres, const char *Int_yystr)
{
  if (*Int_yystr == '"')
    {
      YYSIZE_T Int_yyn = 0;
      char const *Int_yyp = Int_yystr;

      for (;;)
        switch (*++Int_yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++Int_yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (Int_yyres)
              Int_yyres[Int_yyn] = *Int_yyp;
            Int_yyn++;
            break;

          case '"':
            if (Int_yyres)
              Int_yyres[Int_yyn] = '\0';
            return Int_yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! Int_yyres)
    return Int_yystrlen (Int_yystr);

  return Int_yystpcpy (Int_yyres, Int_yystr) - Int_yyres;
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
Int_yysyntax_error (YYSIZE_T *Int_yymsg_alloc, char **Int_yymsg,
                Int_yytype_int16 *Int_yyssp, int Int_yytoken)
{
  YYSIZE_T Int_yysize0 = Int_yytnamerr (YY_NULLPTR, Int_yytname[Int_yytoken]);
  YYSIZE_T Int_yysize = Int_yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *Int_yyformat = YY_NULLPTR;
  /* Arguments of Int_yyformat. */
  char const *Int_yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int Int_yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in Int_yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated Int_yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (Int_yytoken != YYEMPTY)
    {
      int Int_yyn = Int_yypact[*Int_yyssp];
      Int_yyarg[Int_yycount++] = Int_yytname[Int_yytoken];
      if (!Int_yypact_value_is_default (Int_yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int Int_yyxbegin = Int_yyn < 0 ? -Int_yyn : 0;
          /* Stay within bounds of both Int_yycheck and Int_yytname.  */
          int Int_yychecklim = YYLAST - Int_yyn + 1;
          int Int_yyxend = Int_yychecklim < YYNTOKENS ? Int_yychecklim : YYNTOKENS;
          int Int_yyx;

          for (Int_yyx = Int_yyxbegin; Int_yyx < Int_yyxend; ++Int_yyx)
            if (Int_yycheck[Int_yyx + Int_yyn] == Int_yyx && Int_yyx != YYTERROR
                && !Int_yytable_value_is_error (Int_yytable[Int_yyx + Int_yyn]))
              {
                if (Int_yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    Int_yycount = 1;
                    Int_yysize = Int_yysize0;
                    break;
                  }
                Int_yyarg[Int_yycount++] = Int_yytname[Int_yyx];
                {
                  YYSIZE_T Int_yysize1 = Int_yysize + Int_yytnamerr (YY_NULLPTR, Int_yytname[Int_yyx]);
                  if (! (Int_yysize <= Int_yysize1
                         && Int_yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  Int_yysize = Int_yysize1;
                }
              }
        }
    }

  switch (Int_yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        Int_yyformat = S;                       \
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
    YYSIZE_T Int_yysize1 = Int_yysize + Int_yystrlen (Int_yyformat);
    if (! (Int_yysize <= Int_yysize1 && Int_yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    Int_yysize = Int_yysize1;
  }

  if (*Int_yymsg_alloc < Int_yysize)
    {
      *Int_yymsg_alloc = 2 * Int_yysize;
      if (! (Int_yysize <= *Int_yymsg_alloc
             && *Int_yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *Int_yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *Int_yyp = *Int_yymsg;
    int Int_yyi = 0;
    while ((*Int_yyp = *Int_yyformat) != '\0')
      if (*Int_yyp == '%' && Int_yyformat[1] == 's' && Int_yyi < Int_yycount)
        {
          Int_yyp += Int_yytnamerr (Int_yyp, Int_yyarg[Int_yyi++]);
          Int_yyformat += 2;
        }
      else
        {
          Int_yyp++;
          Int_yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
Int_yydestruct (const char *Int_yymsg, int Int_yytype, YYSTYPE *Int_yyvaluep)
{
  YYUSE (Int_yyvaluep);
  if (!Int_yymsg)
    Int_yymsg = "Deleting";
  YY_SYMBOL_PRINT (Int_yymsg, Int_yytype, Int_yyvaluep, Int_yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (Int_yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int Int_yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE Int_yylval;
/* Number of syntax errors so far.  */
int Int_yynerrs;


/*----------.
| Int_yyparse.  |
`----------*/

int
Int_yyparse (void)
{
    int Int_yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int Int_yyerrstatus;

    /* The stacks and their tools:
       'Int_yyss': related to states.
       'Int_yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow Int_yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    Int_yytype_int16 Int_yyssa[YYINITDEPTH];
    Int_yytype_int16 *Int_yyss;
    Int_yytype_int16 *Int_yyssp;

    /* The semantic value stack.  */
    YYSTYPE Int_yyvsa[YYINITDEPTH];
    YYSTYPE *Int_yyvs;
    YYSTYPE *Int_yyvsp;

    YYSIZE_T Int_yystacksize;

  int Int_yyn;
  int Int_yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int Int_yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE Int_yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char Int_yymsgbuf[128];
  char *Int_yymsg = Int_yymsgbuf;
  YYSIZE_T Int_yymsg_alloc = sizeof Int_yymsgbuf;
#endif

#define YYPOPSTACK(N)   (Int_yyvsp -= (N), Int_yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int Int_yylen = 0;

  Int_yyssp = Int_yyss = Int_yyssa;
  Int_yyvsp = Int_yyvs = Int_yyvsa;
  Int_yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  Int_yystate = 0;
  Int_yyerrstatus = 0;
  Int_yynerrs = 0;
  Int_yychar = YYEMPTY; /* Cause a token to be read.  */
  goto Int_yysetstate;

/*------------------------------------------------------------.
| Int_yynewstate -- Push a new state, which is found in Int_yystate.  |
`------------------------------------------------------------*/
 Int_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  Int_yyssp++;

 Int_yysetstate:
  *Int_yyssp = Int_yystate;

  if (Int_yyss + Int_yystacksize - 1 <= Int_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T Int_yysize = Int_yyssp - Int_yyss + 1;

#ifdef Int_yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *Int_yyvs1 = Int_yyvs;
        Int_yytype_int16 *Int_yyss1 = Int_yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if Int_yyoverflow is a macro.  */
        Int_yyoverflow (YY_("memory exhausted"),
                    &Int_yyss1, Int_yysize * sizeof (*Int_yyssp),
                    &Int_yyvs1, Int_yysize * sizeof (*Int_yyvsp),
                    &Int_yystacksize);

        Int_yyss = Int_yyss1;
        Int_yyvs = Int_yyvs1;
      }
#else /* no Int_yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto Int_yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= Int_yystacksize)
        goto Int_yyexhaustedlab;
      Int_yystacksize *= 2;
      if (YYMAXDEPTH < Int_yystacksize)
        Int_yystacksize = YYMAXDEPTH;

      {
        Int_yytype_int16 *Int_yyss1 = Int_yyss;
        union Int_yyalloc *Int_yyptr =
          (union Int_yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (Int_yystacksize));
        if (! Int_yyptr)
          goto Int_yyexhaustedlab;
        YYSTACK_RELOCATE (Int_yyss_alloc, Int_yyss);
        YYSTACK_RELOCATE (Int_yyvs_alloc, Int_yyvs);
#  undef YYSTACK_RELOCATE
        if (Int_yyss1 != Int_yyssa)
          YYSTACK_FREE (Int_yyss1);
      }
# endif
#endif /* no Int_yyoverflow */

      Int_yyssp = Int_yyss + Int_yysize - 1;
      Int_yyvsp = Int_yyvs + Int_yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) Int_yystacksize));

      if (Int_yyss + Int_yystacksize - 1 <= Int_yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", Int_yystate));

  if (Int_yystate == YYFINAL)
    YYACCEPT;

  goto Int_yybackup;

/*-----------.
| Int_yybackup.  |
`-----------*/
Int_yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  Int_yyn = Int_yypact[Int_yystate];
  if (Int_yypact_value_is_default (Int_yyn))
    goto Int_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (Int_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      Int_yychar = Int_yylex ();
    }

  if (Int_yychar <= YYEOF)
    {
      Int_yychar = Int_yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      Int_yytoken = YYTRANSLATE (Int_yychar);
      YY_SYMBOL_PRINT ("Next token is", Int_yytoken, &Int_yylval, &Int_yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  Int_yyn += Int_yytoken;
  if (Int_yyn < 0 || YYLAST < Int_yyn || Int_yycheck[Int_yyn] != Int_yytoken)
    goto Int_yydefault;
  Int_yyn = Int_yytable[Int_yyn];
  if (Int_yyn <= 0)
    {
      if (Int_yytable_value_is_error (Int_yyn))
        goto Int_yyerrlab;
      Int_yyn = -Int_yyn;
      goto Int_yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (Int_yyerrstatus)
    Int_yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", Int_yytoken, &Int_yylval, &Int_yylloc);

  /* Discard the shifted token.  */
  Int_yychar = YYEMPTY;

  Int_yystate = Int_yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++Int_yyvsp = Int_yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto Int_yynewstate;


/*-----------------------------------------------------------.
| Int_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
Int_yydefault:
  Int_yyn = Int_yydefact[Int_yystate];
  if (Int_yyn == 0)
    goto Int_yyerrlab;
  goto Int_yyreduce;


/*-----------------------------.
| Int_yyreduce -- Do a reduction.  |
`-----------------------------*/
Int_yyreduce:
  /* Int_yyn is the number of a rule to reduce with.  */
  Int_yylen = Int_yyr2[Int_yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  Int_yyval = Int_yyvsp[1-Int_yylen];


  YY_REDUCE_PRINT (Int_yyn);
  switch (Int_yyn)
    {
        case 12:
#line 119 "bootparse.y" /* yacc.c:1646  */
    { 
		    DO_START;
		    boot_openrel(LexIDStr((Int_yyvsp[0].ival)));
		    DO_END; 
		}
#line 1378 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 128 "bootparse.y" /* yacc.c:1646  */
    {
		    DO_START;
		    closerel(LexIDStr((Int_yyvsp[0].ival)));
		    DO_END;
		}
#line 1388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 134 "bootparse.y" /* yacc.c:1646  */
    {
		    DO_START;
		    closerel(NULL);
		    DO_END;
		}
#line 1398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 143 "bootparse.y" /* yacc.c:1646  */
    { 
		    DO_START; 
		    numattr=(int)0;
		}
#line 1407 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 148 "bootparse.y" /* yacc.c:1646  */
    { 
		    if (!Quiet) putchar('\n');
		    DO_END;
		}
#line 1416 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 153 "bootparse.y" /* yacc.c:1646  */
    { 
		    DO_START; 

		    if ((Int_yyvsp[-6].ival)) {
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
			reldesc = heap_creatr(LexIDStr((Int_yyvsp[-5].ival)),
					      DEFAULT_SMGR,
					      tupdesc);
			if (DebugMode)
			    puts("bootstrap relation created ok");
		    } else {
			Oid id;
			TupleDesc tupdesc;
			/* extern Oid heap_create();*/

			tupdesc = CreateTupleDesc(numattr,attrtypes);
			id = heap_create(LexIDStr((Int_yyvsp[-5].ival)),
					 NULL,
					 'n',
					 DEFAULT_SMGR,
					 tupdesc);
			if (!Quiet)
			    printf("CREATED relation %s with OID %d\n",
				   LexIDStr((Int_yyvsp[-5].ival)), id);
		    }
		    DO_END;
		    if (DebugMode)
			puts("Commit End");
		}
#line 1460 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 196 "bootparse.y" /* yacc.c:1646  */
    { 
		    DO_START;
		    if (DebugMode)
		        printf("tuple %d<", (Int_yyvsp[0].ival));
		    num_tuples_read = 0;
		}
#line 1471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 203 "bootparse.y" /* yacc.c:1646  */
    {
		    if (num_tuples_read != numattr)
		        elog(WARN,"incorrect number of values for tuple");
		    if (reldesc == (Relation)NULL) {
		        elog(WARN,"must OPEN RELATION before INSERT\n");
		        err_out();
		    }
		    if (DebugMode)
			puts("Insert Begin");
		    objectid = (Int_yyvsp[-4].ival);
		    InsertOneTuple(objectid);
		    if (DebugMode)
			puts("Insert End");
		    if (!Quiet) { putchar('\n'); }
		    DO_END;
		    if (DebugMode)
			puts("Transaction End");
		}
#line 1494 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 225 "bootparse.y" /* yacc.c:1646  */
    {
		  List *params;

		  DO_START;

		  params = lappend(NIL, (List*)(Int_yyvsp[-1].ielem));
		  DefineIndex(LexIDStr((Int_yyvsp[-5].ival)), 
			      LexIDStr((Int_yyvsp[-7].ival)), 
			      LexIDStr((Int_yyvsp[-3].ival)),
			      params, NIL, 0, 0, NIL);
		  DO_END;
		}
#line 1511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 240 "bootparse.y" /* yacc.c:1646  */
    { build_indices(); }
#line 1517 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 244 "bootparse.y" /* yacc.c:1646  */
    {
		    IndexElem *n = (IndexElem*)(Int_yyvsp[-1].ielem);
		    n->class = LexIDStr((Int_yyvsp[0].ival));
		    (Int_yyval.ielem) = n;
		}
#line 1527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 252 "bootparse.y" /* yacc.c:1646  */
    {
		    IndexElem *n = makeNode(IndexElem);
		    n->name = LexIDStr((Int_yyvsp[0].ival));
		    (Int_yyval.ielem) = n;
		}
#line 1537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 258 "bootparse.y" /* yacc.c:1646  */
    {
		    IndexElem *n = makeNode(IndexElem);
		    n->name = LexIDStr((Int_yyvsp[-3].ival));
		    n->args = (List*)(Int_yyvsp[-1].list);
		    (Int_yyval.ielem) = n;
		}
#line 1548 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 267 "bootparse.y" /* yacc.c:1646  */
    {
		  (Int_yyval.list) = lappend(NIL, makeString(LexIDStr((Int_yyvsp[0].ival))));
		}
#line 1556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 271 "bootparse.y" /* yacc.c:1646  */
    {
		  (Int_yyval.list) = lappend((List*)(Int_yyvsp[-2].list), makeString(LexIDStr((Int_yyvsp[0].ival))));
		}
#line 1564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 276 "bootparse.y" /* yacc.c:1646  */
    { (Int_yyval.ival) = 1; }
#line 1570 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 277 "bootparse.y" /* yacc.c:1646  */
    { (Int_yyval.ival) = 0; }
#line 1576 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 287 "bootparse.y" /* yacc.c:1646  */
    { 
		   if(++numattr > MAXATTR)
			elog(FATAL,"Too many attributes\n");
		   DefineAttr(LexIDStr((Int_yyvsp[-2].ival)),LexIDStr((Int_yyvsp[0].ival)),numattr-1);
		   if (DebugMode)
		       printf("\n");
		}
#line 1588 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 297 "bootparse.y" /* yacc.c:1646  */
    { (Int_yyval.ival) = atol(LexIDStr((Int_yyvsp[0].ival))); 		}
#line 1594 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 298 "bootparse.y" /* yacc.c:1646  */
    { extern Oid newoid(); (Int_yyval.ival) = newoid();	}
#line 1600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 308 "bootparse.y" /* yacc.c:1646  */
    {InsertOneValue(objectid, LexIDStr((Int_yyvsp[0].ival)), num_tuples_read++); }
#line 1606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 309 "bootparse.y" /* yacc.c:1646  */
    {InsertOneValue(objectid, LexIDStr((Int_yyvsp[0].ival)), num_tuples_read++); }
#line 1612 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 311 "bootparse.y" /* yacc.c:1646  */
    { InsertOneNull(num_tuples_read++); }
#line 1618 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 315 "bootparse.y" /* yacc.c:1646  */
    { (Int_yyval.ival)=Int_yylval.ival; }
#line 1624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 319 "bootparse.y" /* yacc.c:1646  */
    { (Int_yyval.ival)=Int_yylval.ival; }
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1634 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter Int_yychar, and that requires
     that Int_yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of Int_yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering Int_yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", Int_yyr1[Int_yyn], &Int_yyval, &Int_yyloc);

  YYPOPSTACK (Int_yylen);
  Int_yylen = 0;
  YY_STACK_PRINT (Int_yyss, Int_yyssp);

  *++Int_yyvsp = Int_yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  Int_yyn = Int_yyr1[Int_yyn];

  Int_yystate = Int_yypgoto[Int_yyn - YYNTOKENS] + *Int_yyssp;
  if (0 <= Int_yystate && Int_yystate <= YYLAST && Int_yycheck[Int_yystate] == *Int_yyssp)
    Int_yystate = Int_yytable[Int_yystate];
  else
    Int_yystate = Int_yydefgoto[Int_yyn - YYNTOKENS];

  goto Int_yynewstate;


/*--------------------------------------.
| Int_yyerrlab -- here on detecting error.  |
`--------------------------------------*/
Int_yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  Int_yytoken = Int_yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (Int_yychar);

  /* If not already recovering from an error, report this error.  */
  if (!Int_yyerrstatus)
    {
      ++Int_yynerrs;
#if ! YYERROR_VERBOSE
      Int_yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR Int_yysyntax_error (&Int_yymsg_alloc, &Int_yymsg, \
                                        Int_yyssp, Int_yytoken)
      {
        char const *Int_yymsgp = YY_("syntax error");
        int Int_yysyntax_error_status;
        Int_yysyntax_error_status = YYSYNTAX_ERROR;
        if (Int_yysyntax_error_status == 0)
          Int_yymsgp = Int_yymsg;
        else if (Int_yysyntax_error_status == 1)
          {
            if (Int_yymsg != Int_yymsgbuf)
              YYSTACK_FREE (Int_yymsg);
            Int_yymsg = (char *) YYSTACK_ALLOC (Int_yymsg_alloc);
            if (!Int_yymsg)
              {
                Int_yymsg = Int_yymsgbuf;
                Int_yymsg_alloc = sizeof Int_yymsgbuf;
                Int_yysyntax_error_status = 2;
              }
            else
              {
                Int_yysyntax_error_status = YYSYNTAX_ERROR;
                Int_yymsgp = Int_yymsg;
              }
          }
        Int_yyerror (Int_yymsgp);
        if (Int_yysyntax_error_status == 2)
          goto Int_yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (Int_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (Int_yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (Int_yychar == YYEOF)
            YYABORT;
        }
      else
        {
          Int_yydestruct ("Error: discarding",
                      Int_yytoken, &Int_yylval);
          Int_yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto Int_yyerrlab1;


/*---------------------------------------------------.
| Int_yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
Int_yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label Int_yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto Int_yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (Int_yylen);
  Int_yylen = 0;
  YY_STACK_PRINT (Int_yyss, Int_yyssp);
  Int_yystate = *Int_yyssp;
  goto Int_yyerrlab1;


/*-------------------------------------------------------------.
| Int_yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
Int_yyerrlab1:
  Int_yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      Int_yyn = Int_yypact[Int_yystate];
      if (!Int_yypact_value_is_default (Int_yyn))
        {
          Int_yyn += YYTERROR;
          if (0 <= Int_yyn && Int_yyn <= YYLAST && Int_yycheck[Int_yyn] == YYTERROR)
            {
              Int_yyn = Int_yytable[Int_yyn];
              if (0 < Int_yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (Int_yyssp == Int_yyss)
        YYABORT;


      Int_yydestruct ("Error: popping",
                  Int_yystos[Int_yystate], Int_yyvsp);
      YYPOPSTACK (1);
      Int_yystate = *Int_yyssp;
      YY_STACK_PRINT (Int_yyss, Int_yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++Int_yyvsp = Int_yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", Int_yystos[Int_yyn], Int_yyvsp, Int_yylsp);

  Int_yystate = Int_yyn;
  goto Int_yynewstate;


/*-------------------------------------.
| Int_yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
Int_yyacceptlab:
  Int_yyresult = 0;
  goto Int_yyreturn;

/*-----------------------------------.
| Int_yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
Int_yyabortlab:
  Int_yyresult = 1;
  goto Int_yyreturn;

#if !defined Int_yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| Int_yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
Int_yyexhaustedlab:
  Int_yyerror (YY_("memory exhausted"));
  Int_yyresult = 2;
  /* Fall through.  */
#endif

Int_yyreturn:
  if (Int_yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      Int_yytoken = YYTRANSLATE (Int_yychar);
      Int_yydestruct ("Cleanup: discarding lookahead",
                  Int_yytoken, &Int_yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (Int_yylen);
  YY_STACK_PRINT (Int_yyss, Int_yyssp);
  while (Int_yyssp != Int_yyss)
    {
      Int_yydestruct ("Cleanup: popping",
                  Int_yystos[*Int_yyssp], Int_yyvsp);
      YYPOPSTACK (1);
    }
#ifndef Int_yyoverflow
  if (Int_yyss != Int_yyssa)
    YYSTACK_FREE (Int_yyss);
#endif
#if YYERROR_VERBOSE
  if (Int_yymsg != Int_yymsgbuf)
    YYSTACK_FREE (Int_yymsg);
#endif
  return Int_yyresult;
}
#line 321 "bootparse.y" /* yacc.c:1906  */



