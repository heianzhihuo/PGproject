/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 69 "gram.y" /* yacc.c:1909  */

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

#line 330 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
