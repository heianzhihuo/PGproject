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
#define IS 310
#define ISNULL 311
#define LANGUAGE 312
#define LIGHT 313
#define LISTEN 314
#define LOAD 315
#define MERGE 316
#define MOVE 317
#define NEW 318
#define NONE 319
#define NOT 320
#define NOTHING 321
#define NOTIFY 322
#define NOTNULL 323
#define OIDS 324
#define ON 325
#define OPERATOR 326
#define OPTION 327
#define OR 328
#define ORDER 329
#define PNULL 330
#define PRIVILEGES 331
#define PUBLIC 332
#define PURGE 333
#define P_TYPE 334
#define RENAME 335
#define REPLACE 336
#define RETRIEVE 337
#define RETURNS 338
#define REVOKE 339
#define ROLLBACK 340
#define RULE 341
#define SELECT 342
#define SET 343
#define SETOF 344
#define STDIN 345
#define STDOUT 346
#define STORE 347
#define TABLE 348
#define TO 349
#define TRANSACTION 350
#define UPDATE 351
#define USING 352
#define VACUUM 353
#define VALUES 354
#define VERSION 355
#define VIEW 356
#define WHERE 357
#define WITH 358
#define WORK 359
#define EXECUTE 360
#define RECIPE 361
#define EXPLAIN 362
#define LIKE 363
#define IDENT 364
#define SCONST 365
#define Op 366
#define ICONST 367
#define PARAM 368
#define FCONST 369
#define OP 370
#define UMINUS 371
#define TYPECAST 372
#define REDUCE 373
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
