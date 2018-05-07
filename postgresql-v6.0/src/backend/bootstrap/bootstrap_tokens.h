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
#line 73 "bootparse.y" /* yacc.c:1909  */

    List     *list;
    IndexElem	*ielem;
    char     *str;
    int		ival;

#line 111 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
