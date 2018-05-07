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
extern YYSTYPE yylval;
