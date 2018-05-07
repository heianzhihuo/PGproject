#define ABORT_TRANS 257
#define ACL 258
#define ADD 259
#define AFTER 260
#define AGGREGATE 261
#define ALL 262
#define ALTER 263
#define AND 264
#define APPEND 265
#define ARCHIVE 266
#define ARCH_STORE 267
#define AS 268
#define ASC 269
#define BACKWARD 270
#define BEFORE 271
#define BEGIN_TRANS 272
#define BINARY 273
#define BY 274
#define CAST 275
#define CHANGE 276
#define CLOSE 277
#define CLUSTER 278
#define COLUMN 279
#define COMMIT 280
#define COPY 281
#define CREATE 282
#define CURRENT 283
#define CURSOR 284
#define DATABASE 285
#define DECLARE 286
#define DELETE 287
#define DELIMITERS 288
#define DESC 289
#define DISTINCT 290
#define DO 291
#define DROP 292
#define END_TRANS 293
#define EXTEND 294
#define FETCH 295
#define FOR 296
#define FORWARD 297
#define FROM 298
#define FUNCTION 299
#define GRANT 300
#define GROUP 301
#define HAVING 302
#define HEAVY 303
#define IN 304
#define INDEX 305
#define INHERITS 306
#define INSERT 307
#define INSTEAD 308
#define INTO 309
#define ISNULL 310
#define LANGUAGE 311
#define LIGHT 312
#define LISTEN 313
#define LOAD 314
#define MERGE 315
#define MOVE 316
#define NEW 317
#define NONE 318
#define NOT 319
#define NOTHING 320
#define NOTIFY 321
#define NOTNULL 322
#define OIDS 323
#define ON 324
#define OPERATOR 325
#define OPTION 326
#define OR 327
#define ORDER 328
#define PNULL 329
#define PRIVILEGES 330
#define PUBLIC 331
#define PURGE 332
#define P_TYPE 333
#define RENAME 334
#define REPLACE 335
#define RETRIEVE 336
#define RETURNS 337
#define REVOKE 338
#define ROLLBACK 339
#define RULE 340
#define SELECT 341
#define SET 342
#define SETOF 343
#define STDIN 344
#define STDOUT 345
#define STORE 346
#define TABLE 347
#define TO 348
#define TRANSACTION 349
#define UPDATE 350
#define USING 351
#define VACUUM 352
#define VALUES 353
#define VERSION 354
#define VIEW 355
#define WHERE 356
#define WITH 357
#define WORK 358
#define EXECUTE 359
#define RECIPE 360
#define EXPLAIN 361
#define LIKE 362
#define IDENT 363
#define SCONST 364
#define Op 365
#define ICONST 366
#define PARAM 367
#define FCONST 368
#define OP 369
#define UMINUS 370
#define TYPECAST 371
#define REDUCE 372
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
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
    SortBy		*sortby;
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
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
