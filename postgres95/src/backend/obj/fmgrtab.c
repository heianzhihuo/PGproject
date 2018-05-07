/*-------------------------------------------------------------------------
 *
 * fmgrtab.c--
 *    The function manager's table of internal functions.
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    : /usr/local/cvsroot/postgres95/src/backend/utils/Gen_fmgrtab.sh,v 1.3 1996/07/22 21:55:40 scrappy Exp $
 *
 * NOTES
 *
 *	******************************
 *	*** DO NOT EDIT THIS FILE! ***
 *	******************************
 *
 *	It has been GENERATED by /home/ubuntu/Postgresql/postgres95/src/backend/utils/Gen_fmgrtab.sh
 *	from /home/ubuntu/Postgresql/postgres95/src/backend/catalog/pg_proc.h
 *
 *	We lie here to cc about the return type and arguments of the
 *	builtin functions; all ld cares about is the fact that it
 *	will need to resolve an external function reference.
 *
 *-------------------------------------------------------------------------
 */
#ifdef WIN32
#include <limits.h>
#else
# if defined(PORTNAME_BSD44_derived) ||      defined(PORTNAME_bsdi) ||      defined(PORTNAME_bsdi_2_1)
# include <machine/limits.h>
# define MAXINT	INT_MAX
# else
# include <values.h>           /* for MAXINT */
# endif /* bsd descendents */
#endif /* WIN32 */

#include "utils/fmgrtab.h"

extern char *boolin();
extern char *boolout();
extern char *byteain();
extern char *byteaout();
extern char *charin();
extern char *charout();
extern char *namein();
extern char *nameout();
extern char *char16in();
extern char *char16out();
extern char *int2in();
extern char *int2out();
extern char *int28in();
extern char *int28out();
extern char *int4in();
extern char *int4out();
extern char *regprocin();
extern char *regprocout();
extern char *textin();
extern char *textout();
extern char *tidin();
extern char *tidout();
extern char *xidin();
extern char *xidout();
extern char *cidin();
extern char *cidout();
extern char *oid8in();
extern char *oid8out();
extern char *booleq();
extern char *chareq();
extern char *nameeq();
extern char *int2eq();
extern char *int2lt();
extern char *int4eq();
extern char *int4lt();
extern char *texteq();
extern char *xideq();
extern char *cideq();
extern char *charne();
extern char *charlt();
extern char *charle();
extern char *chargt();
extern char *charge();
extern char *charpl();
extern char *charmi();
extern char *charmul();
extern char *chardiv();
extern char *nameregexeq();
extern char *nameregexne();
extern char *textregexeq();
extern char *textregexne();
extern char *textcat();
extern char *boolne();
extern char *rtsel();
extern char *rtnpage();
extern char *btreesel();
extern char *btreenpage();
extern char *eqsel();
extern char *neqsel();
extern char *intltsel();
extern char *intgtsel();
extern char *eqjoinsel();
extern char *neqjoinsel();
extern char *intltjoinsel();
extern char *intgtjoinsel();
extern char *point_in();
extern char *point_out();
extern char *lseg_in();
extern char *lseg_out();
extern char *path_in();
extern char *path_out();
extern char *box_in();
extern char *box_out();
extern char *box_overlap();
extern char *box_ge();
extern char *box_gt();
extern char *box_eq();
extern char *box_lt();
extern char *box_le();
extern char *point_above();
extern char *point_left();
extern char *point_right();
extern char *point_below();
extern char *point_eq();
extern char *on_pb();
extern char *on_ppath();
extern char *box_center();
extern char *areasel();
extern char *areajoinsel();
extern char *int4mul();
extern char *int4fac();
extern char *pointdist();
extern char *int4ne();
extern char *int2ne();
extern char *int2gt();
extern char *int4gt();
extern char *int2le();
extern char *int4le();
extern char *int4ge();
extern char *int2ge();
extern char *int2mul();
extern char *int2div();
extern char *int4div();
extern char *int2mod();
extern char *int4mod();
extern char *textne();
extern char *int24eq();
extern char *int42eq();
extern char *int24lt();
extern char *int42lt();
extern char *int24gt();
extern char *int42gt();
extern char *int24ne();
extern char *int42ne();
extern char *int24le();
extern char *int42le();
extern char *int24ge();
extern char *int42ge();
extern char *int24mul();
extern char *int42mul();
extern char *int24div();
extern char *int42div();
extern char *int24mod();
extern char *int42mod();
extern char *int2pl();
extern char *int4pl();
extern char *int24pl();
extern char *int42pl();
extern char *int2mi();
extern char *int4mi();
extern char *int24mi();
extern char *int42mi();
extern char *oideq();
extern char *oidne();
extern char *box_same();
extern char *box_contain();
extern char *box_left();
extern char *box_overleft();
extern char *box_overright();
extern char *box_right();
extern char *box_contained();
extern char *rt_box_union();
extern char *rt_box_inter();
extern char *rt_box_size();
extern char *rt_bigbox_size();
extern char *rt_poly_union();
extern char *rt_poly_inter();
extern char *rt_poly_size();
extern char *float4in();
extern char *float4out();
extern char *float4mul();
extern char *float4div();
extern char *float4pl();
extern char *float4mi();
extern char *float4um();
extern char *float4abs();
extern char *float4inc();
extern char *float4larger();
extern char *float4smaller();
extern char *int4um();
extern char *int2um();
extern char *float8in();
extern char *float8out();
extern char *float8mul();
extern char *float8div();
extern char *float8pl();
extern char *float8mi();
extern char *float8um();
extern char *float8abs();
extern char *float8inc();
extern char *float8larger();
extern char *float8smaller();
extern char *dround();
extern char *dtrunc();
extern char *dsqrt();
extern char *dcbrt();
extern char *dpow();
extern char *dexp();
extern char *dlog1();
extern char *nabstimein();
extern char *nabstimeout();
extern char *reltimein();
extern char *reltimeout();
extern char *timepl();
extern char *timemi();
extern char *tintervalin();
extern char *tintervalout();
extern char *ininterval();
extern char *intervalrel();
extern char *timenow();
extern char *abstimeeq();
extern char *abstimene();
extern char *abstimelt();
extern char *abstimegt();
extern char *abstimele();
extern char *abstimege();
extern char *reltimeeq();
extern char *reltimene();
extern char *reltimelt();
extern char *reltimegt();
extern char *reltimele();
extern char *reltimege();
extern char *intervaleq();
extern char *intervalct();
extern char *intervalov();
extern char *intervalleneq();
extern char *intervallenne();
extern char *intervallenlt();
extern char *intervallengt();
extern char *intervallenle();
extern char *intervallenge();
extern char *intervalstart();
extern char *intervalend();
extern char *timeofday();
extern char *int2fac();
extern char *float48mul();
extern char *float48div();
extern char *float48pl();
extern char *float48mi();
extern char *float84mul();
extern char *float84div();
extern char *float84pl();
extern char *float84mi();
extern char *float4eq();
extern char *float4ne();
extern char *float4lt();
extern char *float4le();
extern char *float4gt();
extern char *float4ge();
extern char *float8eq();
extern char *float8ne();
extern char *float8lt();
extern char *float8le();
extern char *float8gt();
extern char *float8ge();
extern char *float48eq();
extern char *float48ne();
extern char *float48lt();
extern char *float48le();
extern char *float48gt();
extern char *float48ge();
extern char *float84eq();
extern char *float84ne();
extern char *float84lt();
extern char *float84le();
extern char *float84gt();
extern char *float84ge();
extern char *ftod();
extern char *dtof();
extern char *i2toi4();
extern char *i4toi2();
extern char *keyfirsteq();
extern char *rtinsert();
extern char *rtdelete();
extern char *rtgettuple();
extern char *rtbuild();
extern char *rtbeginscan();
extern char *rtendscan();
extern char *rtmarkpos();
extern char *rtrestrpos();
extern char *rtrescan();
extern char *btgettuple();
extern char *btinsert();
extern char *btdelete();
extern char *btbeginscan();
extern char *btrescan();
extern char *btendscan();
extern char *btmarkpos();
extern char *btrestrpos();
extern char *btbuild();
extern char *poly_same();
extern char *poly_contain();
extern char *poly_left();
extern char *poly_overleft();
extern char *poly_overright();
extern char *poly_right();
extern char *poly_contained();
extern char *poly_overlap();
extern char *poly_in();
extern char *poly_out();
extern char *btint2cmp();
extern char *btint4cmp();
extern char *btint42cmp();
extern char *btint24cmp();
extern char *btfloat4cmp();
extern char *btfloat8cmp();
extern char *btoidcmp();
extern char *btabstimecmp();
extern char *btcharcmp();
extern char *btnamecmp();
extern char *bttextcmp();
extern char *lseg_distance();
extern char *lseg_interpt();
extern char *dist_ps();
extern char *dist_pb();
extern char *dist_sb();
extern char *close_ps();
extern char *close_pb();
extern char *close_sb();
extern char *on_ps();
extern char *path_distance();
extern char *dist_ppth();
extern char *on_sb();
extern char *inter_sb();
extern char *btchar16cmp();
extern char *hashsel();
extern char *hashnpage();
extern char *hashgettuple();
extern char *hashinsert();
extern char *hashdelete();
extern char *hashbeginscan();
extern char *hashrescan();
extern char *hashendscan();
extern char *hashmarkpos();
extern char *hashrestrpos();
extern char *hashbuild();
extern char *hashint2();
extern char *hashint4();
extern char *hashfloat4();
extern char *hashfloat8();
extern char *hashoid();
extern char *hashchar();
extern char *hashname();
extern char *hashtext();
extern char *char2in();
extern char *char4in();
extern char *char8in();
extern char *char2out();
extern char *char4out();
extern char *char8out();
extern char *char2eq();
extern char *char4eq();
extern char *char8eq();
extern char *char2lt();
extern char *char4lt();
extern char *char8lt();
extern char *char2le();
extern char *char4le();
extern char *char8le();
extern char *char2gt();
extern char *char4gt();
extern char *char8gt();
extern char *char2ge();
extern char *char16eq();
extern char *char16lt();
extern char *char16le();
extern char *char16gt();
extern char *char16ge();
extern char *char16ne();
extern char *hashchar16();
extern char *int4notin();
extern char *oidnotin();
extern char *int44in();
extern char *int44out();
extern char *namelt();
extern char *namele();
extern char *namegt();
extern char *namege();
extern char *namene();
extern char *mktinterval();
extern char *oid8eq();
extern char *char4ge();
extern char *char8ge();
extern char *char2ne();
extern char *char4ne();
extern char *char8ne();
extern char *btchar2cmp();
extern char *btchar4cmp();
extern char *btchar8cmp();
extern char *hashchar2();
extern char *hashchar4();
extern char *hashchar8();
extern char *char8regexeq();
extern char *char8regexne();
extern char *char2regexeq();
extern char *char16regexeq();
extern char *char16regexne();
extern char *GetPgUserName();
extern char *userfntest();
extern char *oidrand();
extern char *oidsrand();
extern char *oideqint4();
extern char *int4eqoid();
extern char *byteaGetSize();
extern char *byteaGetByte();
extern char *byteaSetByte();
extern char *byteaGetBit();
extern char *byteaSetBit();
extern char *pqtest();
extern char *text_lt();
extern char *text_le();
extern char *text_gt();
extern char *text_ge();
extern char *array_eq();
extern char *array_assgn();
extern char *array_clip();
extern char *array_dims();
extern char *array_set();
extern char *array_ref();
extern char *array_in();
extern char *array_out();
extern char *filename_in();
extern char *filename_out();
extern char *smgrin();
extern char *smgrout();
extern char *smgreq();
extern char *smgrne();
extern char *lo_import();
extern char *lo_export();
extern char *int4inc();
extern char *int2inc();
extern char *int4larger();
extern char *int4smaller();
extern char *int2larger();
extern char *int2smaller();
extern char *oidint2in();
extern char *oidint2out();
extern char *oidint2lt();
extern char *oidint2le();
extern char *oidint2eq();
extern char *oidint2ge();
extern char *oidint2gt();
extern char *oidint2ne();
extern char *oidint2cmp();
extern char *mkoidint2();
extern char *char4regexeq();
extern char *char2regexne();
extern char *char4regexne();
extern char *textlike();
extern char *textnlike();
extern char *char2like();
extern char *char2nlike();
extern char *char4like();
extern char *char4nlike();
extern char *char8like();
extern char *char8nlike();
extern char *namelike();
extern char *namenlike();
extern char *char16like();
extern char *char16nlike();
extern char *oidint4in();
extern char *oidint4out();
extern char *oidint4lt();
extern char *oidint4le();
extern char *oidint4eq();
extern char *oidint4ge();
extern char *oidint4gt();
extern char *oidint4ne();
extern char *oidint4cmp();
extern char *mkoidint4();
extern char *oidnamein();
extern char *oidnameout();
extern char *oidnamelt();
extern char *oidnamele();
extern char *oidnameeq();
extern char *oidnamege();
extern char *oidnamegt();
extern char *oidnamene();
extern char *oidnamecmp();
extern char *mkoidname();
extern char *lo_open();
extern char *lo_close();
extern char *LOread();
extern char *LOwrite();
extern char *lo_lseek();
extern char *lo_creat();
extern char *lo_tell();
extern char *lo_unlink();
extern char *RegprocToOid();
extern char *path_inter();
extern char *box_copy();
extern char *box_area();
extern char *box_length();
extern char *box_height();
extern char *box_distance();
extern char *box_intersect();
extern char *box_diagonal();
extern char *path_n_lt();
extern char *path_n_gt();
extern char *path_n_eq();
extern char *path_n_le();
extern char *path_n_ge();
extern char *path_length();
extern char *point_copy();
extern char *point_vert();
extern char *point_horiz();
extern char *point_distance();
extern char *point_slope();
extern char *lseg_construct();
extern char *lseg_intersect();
extern char *lseg_parallel();
extern char *lseg_perp();
extern char *lseg_vertical();
extern char *lseg_horizontal();
extern char *lseg_eq();
extern char *NullValue();
extern char *NonNullValue();
extern char *aclitemin();
extern char *aclitemout();
extern char *aclinsert();
extern char *aclremove();
extern char *aclcontains();
extern char *seteval();
extern char *bpcharin();
extern char *bpcharout();
extern char *varcharin();
extern char *varcharout();
extern char *bpchareq();
extern char *bpcharlt();
extern char *bpcharle();
extern char *bpchargt();
extern char *bpcharge();
extern char *bpcharne();
extern char *varchareq();
extern char *varcharlt();
extern char *varcharle();
extern char *varchargt();
extern char *varcharge();
extern char *varcharne();
extern char *bpcharcmp();
extern char *varcharcmp();
extern char *hashbpchar();
extern char *hashvarchar();
extern char *date_in();
extern char *date_out();
extern char *date_eq();
extern char *date_lt();
extern char *date_le();
extern char *date_gt();
extern char *date_ge();
extern char *date_ne();
extern char *date_cmp();
extern char *time_in();
extern char *time_out();
extern char *time_eq();
extern char *time_lt();
extern char *time_le();
extern char *time_gt();
extern char *time_ge();
extern char *time_ne();
extern char *time_cmp();
extern char *int42reltime();
extern char *char2icregexeq();
extern char *char2icregexne();
extern char *char4icregexeq();
extern char *char4icregexne();
extern char *char8icregexeq();
extern char *char8icregexne();
extern char *char16icregexeq();
extern char *char16icregexne();
extern char *texticregexeq();
extern char *texticregexne();
extern char *nameicregexeq();
extern char *nameicregexne();

static FmgrCall fmgr_builtins[] = {
  {28 , 1 , boolin, "boolin" },
  {29 , 1 , boolout, "boolout" },
  {30 , 1 , byteain, "byteain" },
  {31 , 1 , byteaout, "byteaout" },
  {32 , 1 , charin, "charin" },
  {33 , 1 , charout, "charout" },
  {34 , 1 , namein, "namein" },
  {35 , 1 , nameout, "nameout" },
  {36 , 1 , char16in, "char16in" },
  {37 , 1 , char16out, "char16out" },
  {38 , 1 , int2in, "int2in" },
  {39 , 1 , int2out, "int2out" },
  {40 , 1 , int28in, "int28in" },
  {41 , 1 , int28out, "int28out" },
  {42 , 1 , int4in, "int4in" },
  {43 , 1 , int4out, "int4out" },
  {44 , 1 , regprocin, "regprocin" },
  {45 , 1 , regprocout, "regprocout" },
  {46 , 1 , textin, "textin" },
  {47 , 1 , textout, "textout" },
  {48 , 1 , tidin, "tidin" },
  {49 , 1 , tidout, "tidout" },
  {50 , 1 , xidin, "xidin" },
  {51 , 1 , xidout, "xidout" },
  {52 , 1 , cidin, "cidin" },
  {53 , 1 , cidout, "cidout" },
  {54 , 1 , oid8in, "oid8in" },
  {55 , 1 , oid8out, "oid8out" },
  {60 , 2 , booleq, "booleq" },
  {61 , 2 , chareq, "chareq" },
  {62 , 2 , nameeq, "nameeq" },
  {63 , 2 , int2eq, "int2eq" },
  {64 , 2 , int2lt, "int2lt" },
  {65 , 2 , int4eq, "int4eq" },
  {66 , 2 , int4lt, "int4lt" },
  {67 , 2 , texteq, "texteq" },
  {68 , 2 , xideq, "xideq" },
  {69 , 2 , cideq, "cideq" },
  {70 , 2 , charne, "charne" },
  {71 , 2 , charlt, "charlt" },
  {72 , 2 , charle, "charle" },
  {73 , 2 , chargt, "chargt" },
  {74 , 2 , charge, "charge" },
  {75 , 2 , charpl, "charpl" },
  {76 , 2 , charmi, "charmi" },
  {77 , 2 , charmul, "charmul" },
  {78 , 2 , chardiv, "chardiv" },
  {79 , 2 , nameregexeq, "nameregexeq" },
  {80 , 2 , nameregexne, "nameregexne" },
  {81 , 2 , textregexeq, "textregexeq" },
  {82 , 2 , textregexne, "textregexne" },
  {83 , 2 , textcat, "textcat" },
  {84 , 2 , boolne, "boolne" },
  {97 , 7 , rtsel, "rtsel" },
  {98 , 7 , rtnpage, "rtnpage" },
  {99 , 7 , btreesel, "btreesel" },
  {100 , 7 , btreenpage, "btreenpage" },
  {101 , 5 , eqsel, "eqsel" },
  {102 , 5 , neqsel, "neqsel" },
  {103 , 5 , intltsel, "intltsel" },
  {104 , 5 , intgtsel, "intgtsel" },
  {105 , 5 , eqjoinsel, "eqjoinsel" },
  {106 , 5 , neqjoinsel, "neqjoinsel" },
  {107 , 5 , intltjoinsel, "intltjoinsel" },
  {108 , 5 , intgtjoinsel, "intgtjoinsel" },
  {117 , 1 , point_in, "point_in" },
  {118 , 1 , point_out, "point_out" },
  {119 , 1 , lseg_in, "lseg_in" },
  {120 , 1 , lseg_out, "lseg_out" },
  {121 , 1 , path_in, "path_in" },
  {122 , 1 , path_out, "path_out" },
  {123 , 1 , box_in, "box_in" },
  {124 , 1 , box_out, "box_out" },
  {125 , 2 , box_overlap, "box_overlap" },
  {126 , 2 , box_ge, "box_ge" },
  {127 , 2 , box_gt, "box_gt" },
  {128 , 2 , box_eq, "box_eq" },
  {129 , 2 , box_lt, "box_lt" },
  {130 , 2 , box_le, "box_le" },
  {131 , 2 , point_above, "point_above" },
  {132 , 2 , point_left, "point_left" },
  {133 , 2 , point_right, "point_right" },
  {134 , 2 , point_below, "point_below" },
  {135 , 2 , point_eq, "point_eq" },
  {136 , 2 , on_pb, "on_pb" },
  {137 , 2 , on_ppath, "on_ppath" },
  {138 , 1 , box_center, "box_center" },
  {139 , 5 , areasel, "areasel" },
  {140 , 5 , areajoinsel, "areajoinsel" },
  {141 , 2 , int4mul, "int4mul" },
  {142 , 1 , int4fac, "int4fac" },
  {143 , 2 , pointdist, "pointdist" },
  {144 , 2 , int4ne, "int4ne" },
  {145 , 2 , int2ne, "int2ne" },
  {146 , 2 , int2gt, "int2gt" },
  {147 , 2 , int4gt, "int4gt" },
  {148 , 2 , int2le, "int2le" },
  {149 , 2 , int4le, "int4le" },
  {150 , 2 , int4ge, "int4ge" },
  {151 , 2 , int2ge, "int2ge" },
  {152 , 2 , int2mul, "int2mul" },
  {153 , 2 , int2div, "int2div" },
  {154 , 2 , int4div, "int4div" },
  {155 , 2 , int2mod, "int2mod" },
  {156 , 2 , int4mod, "int4mod" },
  {157 , 2 , textne, "textne" },
  {158 , 2 , int24eq, "int24eq" },
  {159 , 2 , int42eq, "int42eq" },
  {160 , 2 , int24lt, "int24lt" },
  {161 , 2 , int42lt, "int42lt" },
  {162 , 2 , int24gt, "int24gt" },
  {163 , 2 , int42gt, "int42gt" },
  {164 , 2 , int24ne, "int24ne" },
  {165 , 2 , int42ne, "int42ne" },
  {166 , 2 , int24le, "int24le" },
  {167 , 2 , int42le, "int42le" },
  {168 , 2 , int24ge, "int24ge" },
  {169 , 2 , int42ge, "int42ge" },
  {170 , 2 , int24mul, "int24mul" },
  {171 , 2 , int42mul, "int42mul" },
  {172 , 2 , int24div, "int24div" },
  {173 , 2 , int42div, "int42div" },
  {174 , 2 , int24mod, "int24mod" },
  {175 , 2 , int42mod, "int42mod" },
  {176 , 2 , int2pl, "int2pl" },
  {177 , 2 , int4pl, "int4pl" },
  {178 , 2 , int24pl, "int24pl" },
  {179 , 2 , int42pl, "int42pl" },
  {180 , 2 , int2mi, "int2mi" },
  {181 , 2 , int4mi, "int4mi" },
  {182 , 2 , int24mi, "int24mi" },
  {183 , 2 , int42mi, "int42mi" },
  {184 , 2 , oideq, "oideq" },
  {185 , 2 , oidne, "oidne" },
  {186 , 2 , box_same, "box_same" },
  {187 , 2 , box_contain, "box_contain" },
  {188 , 2 , box_left, "box_left" },
  {189 , 2 , box_overleft, "box_overleft" },
  {190 , 2 , box_overright, "box_overright" },
  {191 , 2 , box_right, "box_right" },
  {192 , 2 , box_contained, "box_contained" },
  {193 , 2 , rt_box_union, "rt_box_union" },
  {194 , 2 , rt_box_inter, "rt_box_inter" },
  {195 , 2 , rt_box_size, "rt_box_size" },
  {196 , 2 , rt_bigbox_size, "rt_bigbox_size" },
  {197 , 2 , rt_poly_union, "rt_poly_union" },
  {198 , 2 , rt_poly_inter, "rt_poly_inter" },
  {199 , 2 , rt_poly_size, "rt_poly_size" },
  {200 , 1 , float4in, "float4in" },
  {201 , 1 , float4out, "float4out" },
  {202 , 2 , float4mul, "float4mul" },
  {203 , 2 , float4div, "float4div" },
  {204 , 2 , float4pl, "float4pl" },
  {205 , 2 , float4mi, "float4mi" },
  {206 , 1 , float4um, "float4um" },
  {207 , 1 , float4abs, "float4abs" },
  {208 , 1 , float4inc, "float4inc" },
  {209 , 2 , float4larger, "float4larger" },
  {211 , 2 , float4smaller, "float4smaller" },
  {212 , 1 , int4um, "int4um" },
  {213 , 1 , int2um, "int2um" },
  {214 , 1 , float8in, "float8in" },
  {215 , 1 , float8out, "float8out" },
  {216 , 2 , float8mul, "float8mul" },
  {217 , 2 , float8div, "float8div" },
  {218 , 2 , float8pl, "float8pl" },
  {219 , 2 , float8mi, "float8mi" },
  {220 , 1 , float8um, "float8um" },
  {221 , 1 , float8abs, "float8abs" },
  {222 , 1 , float8inc, "float8inc" },
  {223 , 2 , float8larger, "float8larger" },
  {224 , 2 , float8smaller, "float8smaller" },
  {228 , 1 , dround, "dround" },
  {229 , 1 , dtrunc, "dtrunc" },
  {230 , 1 , dsqrt, "dsqrt" },
  {231 , 1 , dcbrt, "dcbrt" },
  {232 , 2 , dpow, "dpow" },
  {233 , 1 , dexp, "dexp" },
  {234 , 1 , dlog1, "dlog1" },
  {240 , 1 , nabstimein, "nabstimein" },
  {241 , 1 , nabstimeout, "nabstimeout" },
  {242 , 1 , reltimein, "reltimein" },
  {243 , 1 , reltimeout, "reltimeout" },
  {244 , 2 , timepl, "timepl" },
  {245 , 2 , timemi, "timemi" },
  {246 , 1 , tintervalin, "tintervalin" },
  {247 , 1 , tintervalout, "tintervalout" },
  {248 , 2 , ininterval, "ininterval" },
  {249 , 1 , intervalrel, "intervalrel" },
  {250 , 0 , timenow, "timenow" },
  {251 , 2 , abstimeeq, "abstimeeq" },
  {252 , 2 , abstimene, "abstimene" },
  {253 , 2 , abstimelt, "abstimelt" },
  {254 , 2 , abstimegt, "abstimegt" },
  {255 , 2 , abstimele, "abstimele" },
  {256 , 2 , abstimege, "abstimege" },
  {257 , 2 , reltimeeq, "reltimeeq" },
  {258 , 2 , reltimene, "reltimene" },
  {259 , 2 , reltimelt, "reltimelt" },
  {260 , 2 , reltimegt, "reltimegt" },
  {261 , 2 , reltimele, "reltimele" },
  {262 , 2 , reltimege, "reltimege" },
  {263 , 2 , intervaleq, "intervaleq" },
  {264 , 2 , intervalct, "intervalct" },
  {265 , 2 , intervalov, "intervalov" },
  {266 , 2 , intervalleneq, "intervalleneq" },
  {267 , 2 , intervallenne, "intervallenne" },
  {268 , 2 , intervallenlt, "intervallenlt" },
  {269 , 2 , intervallengt, "intervallengt" },
  {270 , 2 , intervallenle, "intervallenle" },
  {271 , 2 , intervallenge, "intervallenge" },
  {272 , 1 , intervalstart, "intervalstart" },
  {273 , 1 , intervalend, "intervalend" },
  {274 , 0 , timeofday, "timeofday" },
  {276 , 1 , int2fac, "int2fac" },
  {279 , 2 , float48mul, "float48mul" },
  {280 , 2 , float48div, "float48div" },
  {281 , 2 , float48pl, "float48pl" },
  {282 , 2 , float48mi, "float48mi" },
  {283 , 2 , float84mul, "float84mul" },
  {284 , 2 , float84div, "float84div" },
  {285 , 2 , float84pl, "float84pl" },
  {286 , 2 , float84mi, "float84mi" },
  {287 , 2 , float4eq, "float4eq" },
  {288 , 2 , float4ne, "float4ne" },
  {289 , 2 , float4lt, "float4lt" },
  {290 , 2 , float4le, "float4le" },
  {291 , 2 , float4gt, "float4gt" },
  {292 , 2 , float4ge, "float4ge" },
  {293 , 2 , float8eq, "float8eq" },
  {294 , 2 , float8ne, "float8ne" },
  {295 , 2 , float8lt, "float8lt" },
  {296 , 2 , float8le, "float8le" },
  {297 , 2 , float8gt, "float8gt" },
  {298 , 2 , float8ge, "float8ge" },
  {299 , 2 , float48eq, "float48eq" },
  {300 , 2 , float48ne, "float48ne" },
  {301 , 2 , float48lt, "float48lt" },
  {302 , 2 , float48le, "float48le" },
  {303 , 2 , float48gt, "float48gt" },
  {304 , 2 , float48ge, "float48ge" },
  {305 , 2 , float84eq, "float84eq" },
  {306 , 2 , float84ne, "float84ne" },
  {307 , 2 , float84lt, "float84lt" },
  {308 , 2 , float84le, "float84le" },
  {309 , 2 , float84gt, "float84gt" },
  {310 , 2 , float84ge, "float84ge" },
  {311 , 2 , ftod, "ftod" },
  {312 , 2 , dtof, "dtof" },
  {313 , 2 , i2toi4, "i2toi4" },
  {314 , 2 , i4toi2, "i4toi2" },
  {315 , 2 , keyfirsteq, "keyfirsteq" },
  {320 , 2 , rtinsert, "rtinsert" },
  {321 , 2 , rtdelete, "rtdelete" },
  {322 , 2 , rtgettuple, "rtgettuple" },
  {323 , 9 , rtbuild, "rtbuild" },
  {324 , 4 , rtbeginscan, "rtbeginscan" },
  {325 , 1 , rtendscan, "rtendscan" },
  {326 , 1 , rtmarkpos, "rtmarkpos" },
  {327 , 1 , rtrestrpos, "rtrestrpos" },
  {328 , 3 , rtrescan, "rtrescan" },
  {330 , 2 , btgettuple, "btgettuple" },
  {331 , 2 , btinsert, "btinsert" },
  {332 , 2 , btdelete, "btdelete" },
  {333 , 4 , btbeginscan, "btbeginscan" },
  {334 , 3 , btrescan, "btrescan" },
  {335 , 1 , btendscan, "btendscan" },
  {336 , 1 , btmarkpos, "btmarkpos" },
  {337 , 1 , btrestrpos, "btrestrpos" },
  {338 , 9 , btbuild, "btbuild" },
  {339 , 2 , poly_same, "poly_same" },
  {340 , 2 , poly_contain, "poly_contain" },
  {341 , 2 , poly_left, "poly_left" },
  {342 , 2 , poly_overleft, "poly_overleft" },
  {343 , 2 , poly_overright, "poly_overright" },
  {344 , 2 , poly_right, "poly_right" },
  {345 , 2 , poly_contained, "poly_contained" },
  {346 , 2 , poly_overlap, "poly_overlap" },
  {347 , 1 , poly_in, "poly_in" },
  {348 , 1 , poly_out, "poly_out" },
  {350 , 2 , btint2cmp, "btint2cmp" },
  {351 , 2 , btint4cmp, "btint4cmp" },
  {352 , 2 , btint42cmp, "btint42cmp" },
  {353 , 2 , btint24cmp, "btint24cmp" },
  {354 , 2 , btfloat4cmp, "btfloat4cmp" },
  {355 , 2 , btfloat8cmp, "btfloat8cmp" },
  {356 , 2 , btoidcmp, "btoidcmp" },
  {357 , 2 , btabstimecmp, "btabstimecmp" },
  {358 , 2 , btcharcmp, "btcharcmp" },
  {359 , 2 , btnamecmp, "btnamecmp" },
  {360 , 2 , bttextcmp, "bttextcmp" },
  {361 , 2 , lseg_distance, "lseg_distance" },
  {362 , 2 , lseg_interpt, "lseg_interpt" },
  {363 , 2 , dist_ps, "dist_ps" },
  {364 , 2 , dist_pb, "dist_pb" },
  {365 , 2 , dist_sb, "dist_sb" },
  {366 , 2 , close_ps, "close_ps" },
  {367 , 2 , close_pb, "close_pb" },
  {368 , 2 , close_sb, "close_sb" },
  {369 , 2 , on_ps, "on_ps" },
  {370 , 2 , path_distance, "path_distance" },
  {371 , 2 , dist_ppth, "dist_ppth" },
  {372 , 2 , on_sb, "on_sb" },
  {373 , 2 , inter_sb, "inter_sb" },
  {374 , 2 , btchar16cmp, "btchar16cmp" },
  {438 , 7 , hashsel, "hashsel" },
  {439 , 7 , hashnpage, "hashnpage" },
  {440 , 2 , hashgettuple, "hashgettuple" },
  {441 , 2 , hashinsert, "hashinsert" },
  {442 , 2 , hashdelete, "hashdelete" },
  {443 , 4 , hashbeginscan, "hashbeginscan" },
  {444 , 3 , hashrescan, "hashrescan" },
  {445 , 1 , hashendscan, "hashendscan" },
  {446 , 1 , hashmarkpos, "hashmarkpos" },
  {447 , 1 , hashrestrpos, "hashrestrpos" },
  {448 , 9 , hashbuild, "hashbuild" },
  {449 , 2 , hashint2, "hashint2" },
  {450 , 2 , hashint4, "hashint4" },
  {451 , 2 , hashfloat4, "hashfloat4" },
  {452 , 2 , hashfloat8, "hashfloat8" },
  {453 , 2 , hashoid, "hashoid" },
  {454 , 2 , hashchar, "hashchar" },
  {455 , 2 , hashname, "hashname" },
  {456 , 2 , hashtext, "hashtext" },
  {466 , 1 , char2in, "char2in" },
  {467 , 1 , char4in, "char4in" },
  {468 , 1 , char8in, "char8in" },
  {469 , 1 , char2out, "char2out" },
  {470 , 1 , char4out, "char4out" },
  {471 , 1 , char8out, "char8out" },
  {472 , 2 , char2eq, "char2eq" },
  {473 , 2 , char4eq, "char4eq" },
  {474 , 2 , char8eq, "char8eq" },
  {475 , 2 , char2lt, "char2lt" },
  {476 , 2 , char4lt, "char4lt" },
  {477 , 2 , char8lt, "char8lt" },
  {478 , 2 , char2le, "char2le" },
  {479 , 2 , char4le, "char4le" },
  {480 , 2 , char8le, "char8le" },
  {481 , 2 , char2gt, "char2gt" },
  {482 , 2 , char4gt, "char4gt" },
  {483 , 2 , char8gt, "char8gt" },
  {484 , 2 , char2ge, "char2ge" },
  {490 , 2 , char16eq, "char16eq" },
  {492 , 2 , char16lt, "char16lt" },
  {493 , 2 , char16le, "char16le" },
  {494 , 2 , char16gt, "char16gt" },
  {495 , 2 , char16ge, "char16ge" },
  {496 , 2 , char16ne, "char16ne" },
  {499 , 2 , hashchar16, "hashchar16" },
  {650 , 2 , int4notin, "int4notin" },
  {651 , 2 , oidnotin, "oidnotin" },
  {652 , 1 , int44in, "int44in" },
  {653 , 1 , int44out, "int44out" },
  {655 , 2 , namelt, "namelt" },
  {656 , 2 , namele, "namele" },
  {657 , 2 , namegt, "namegt" },
  {658 , 2 , namege, "namege" },
  {659 , 2 , namene, "namene" },
  {682 , 2 , mktinterval, "mktinterval" },
  {683 , 2 , oid8eq, "oid8eq" },
  {684 , 2 , char4ge, "char4ge" },
  {685 , 2 , char8ge, "char8ge" },
  {686 , 2 , char2ne, "char2ne" },
  {687 , 2 , char4ne, "char4ne" },
  {688 , 2 , char8ne, "char8ne" },
  {689 , 2 , btchar2cmp, "btchar2cmp" },
  {690 , 2 , btchar4cmp, "btchar4cmp" },
  {691 , 2 , btchar8cmp, "btchar8cmp" },
  {692 , 2 , hashchar2, "hashchar2" },
  {693 , 2 , hashchar4, "hashchar4" },
  {694 , 2 , hashchar8, "hashchar8" },
  {695 , 2 , char8regexeq, "char8regexeq" },
  {696 , 2 , char8regexne, "char8regexne" },
  {699 , 2 , char2regexeq, "char2regexeq" },
  {700 , 2 , char16regexeq, "char16regexeq" },
  {701 , 2 , char16regexne, "char16regexne" },
  {710 , 0 , GetPgUserName, "GetPgUserName" },
  {711 , 1 , userfntest, "userfntest" },
  {713 , 2 , oidrand, "oidrand" },
  {715 , 1 , oidsrand, "oidsrand" },
  {716 , 2 , oideqint4, "oideqint4" },
  {717 , 2 , int4eqoid, "int4eqoid" },
  {720 , 1 , byteaGetSize, "byteaGetSize" },
  {721 , 2 , byteaGetByte, "byteaGetByte" },
  {722 , 3 , byteaSetByte, "byteaSetByte" },
  {723 , 2 , byteaGetBit, "byteaGetBit" },
  {724 , 3 , byteaSetBit, "byteaSetBit" },
  {730 , 1 , pqtest, "pqtest" },
  {740 , 2 , text_lt, "text_lt" },
  {741 , 2 , text_le, "text_le" },
  {742 , 2 , text_gt, "text_gt" },
  {743 , 2 , text_ge, "text_ge" },
  {744 , 2 , array_eq, "array_eq" },
  {745 , 8 , array_assgn, "array_assgn" },
  {746 , 7 , array_clip, "array_clip" },
  {747 , 1 , array_dims, "array_dims" },
  {748 , 8 , array_set, "array_set" },
  {749 , 7 , array_ref, "array_ref" },
  {750 , 2 , array_in, "array_in" },
  {751 , 2 , array_out, "array_out" },
  {752 , 2 , filename_in, "filename_in" },
  {753 , 2 , filename_out, "filename_out" },
  {760 , 1 , smgrin, "smgrin" },
  {761 , 1 , smgrout, "smgrout" },
  {762 , 2 , smgreq, "smgreq" },
  {763 , 2 , smgrne, "smgrne" },
  {764 , 1 , lo_import, "lo_import" },
  {765 , 2 , lo_export, "lo_export" },
  {766 , 1 , int4inc, "int4inc" },
  {767 , 1 , int2inc, "int2inc" },
  {768 , 2 , int4larger, "int4larger" },
  {769 , 2 , int4smaller, "int4smaller" },
  {770 , 2 , int2larger, "int2larger" },
  {771 , 2 , int2smaller, "int2smaller" },
  {820 , 1 , oidint2in, "oidint2in" },
  {821 , 1 , oidint2out, "oidint2out" },
  {822 , 2 , oidint2lt, "oidint2lt" },
  {823 , 2 , oidint2le, "oidint2le" },
  {824 , 2 , oidint2eq, "oidint2eq" },
  {825 , 2 , oidint2ge, "oidint2ge" },
  {826 , 2 , oidint2gt, "oidint2gt" },
  {827 , 2 , oidint2ne, "oidint2ne" },
  {828 , 2 , oidint2cmp, "oidint2cmp" },
  {829 , 2 , mkoidint2, "mkoidint2" },
  {836 , 2 , char4regexeq, "char4regexeq" },
  {837 , 2 , char2regexne, "char2regexne" },
  {838 , 2 , char4regexne, "char4regexne" },
  {850 , 2 , textlike, "textlike" },
  {851 , 2 , textnlike, "textnlike" },
  {852 , 2 , char2like, "char2like" },
  {853 , 2 , char2nlike, "char2nlike" },
  {854 , 2 , char4like, "char4like" },
  {855 , 2 , char4nlike, "char4nlike" },
  {856 , 2 , char8like, "char8like" },
  {857 , 2 , char8nlike, "char8nlike" },
  {858 , 2 , namelike, "namelike" },
  {859 , 2 , namenlike, "namenlike" },
  {860 , 2 , char16like, "char16like" },
  {861 , 2 , char16nlike, "char16nlike" },
  {920 , 1 , oidint4in, "oidint4in" },
  {921 , 1 , oidint4out, "oidint4out" },
  {922 , 2 , oidint4lt, "oidint4lt" },
  {923 , 2 , oidint4le, "oidint4le" },
  {924 , 2 , oidint4eq, "oidint4eq" },
  {925 , 2 , oidint4ge, "oidint4ge" },
  {926 , 2 , oidint4gt, "oidint4gt" },
  {927 , 2 , oidint4ne, "oidint4ne" },
  {928 , 2 , oidint4cmp, "oidint4cmp" },
  {929 , 2 , mkoidint4, "mkoidint4" },
  {940 , 1 , oidnamein, "oidnamein" },
  {941 , 1 , oidnameout, "oidnameout" },
  {942 , 2 , oidnamelt, "oidnamelt" },
  {943 , 2 , oidnamele, "oidnamele" },
  {944 , 2 , oidnameeq, "oidnameeq" },
  {945 , 2 , oidnamege, "oidnamege" },
  {946 , 2 , oidnamegt, "oidnamegt" },
  {947 , 2 , oidnamene, "oidnamene" },
  {948 , 2 , oidnamecmp, "oidnamecmp" },
  {949 , 2 , mkoidname, "mkoidname" },
  {952 , 2 , lo_open, "lo_open" },
  {953 , 1 , lo_close, "lo_close" },
  {954 , 2 , LOread, "LOread" },
  {955 , 2 , LOwrite, "LOwrite" },
  {956 , 3 , lo_lseek, "lo_lseek" },
  {957 , 1 , lo_creat, "lo_creat" },
  {958 , 1 , lo_tell, "lo_tell" },
  {964 , 1 , lo_unlink, "lo_unlink" },
  {972 , 1 , RegprocToOid, "RegprocToOid" },
  {973 , 2 , path_inter, "path_inter" },
  {974 , 1 , box_copy, "box_copy" },
  {975 , 1 , box_area, "box_area" },
  {976 , 1 , box_length, "box_length" },
  {977 , 1 , box_height, "box_height" },
  {978 , 2 , box_distance, "box_distance" },
  {980 , 2 , box_intersect, "box_intersect" },
  {981 , 1 , box_diagonal, "box_diagonal" },
  {982 , 2 , path_n_lt, "path_n_lt" },
  {983 , 2 , path_n_gt, "path_n_gt" },
  {984 , 2 , path_n_eq, "path_n_eq" },
  {985 , 2 , path_n_le, "path_n_le" },
  {986 , 2 , path_n_ge, "path_n_ge" },
  {987 , 1 , path_length, "path_length" },
  {988 , 1 , point_copy, "point_copy" },
  {989 , 2 , point_vert, "point_vert" },
  {990 , 2 , point_horiz, "point_horiz" },
  {991 , 2 , point_distance, "point_distance" },
  {992 , 2 , point_slope, "point_slope" },
  {993 , 2 , lseg_construct, "lseg_construct" },
  {994 , 2 , lseg_intersect, "lseg_intersect" },
  {995 , 2 , lseg_parallel, "lseg_parallel" },
  {996 , 2 , lseg_perp, "lseg_perp" },
  {997 , 1 , lseg_vertical, "lseg_vertical" },
  {998 , 1 , lseg_horizontal, "lseg_horizontal" },
  {999 , 2 , lseg_eq, "lseg_eq" },
  {1029 , 1 , NullValue, "NullValue" },
  {1030 , 1 , NonNullValue, "NonNullValue" },
  {1031 , 1 , aclitemin, "aclitemin" },
  {1032 , 1 , aclitemout, "aclitemout" },
  {1035 , 2 , aclinsert, "aclinsert" },
  {1036 , 2 , aclremove, "aclremove" },
  {1037 , 2 , aclcontains, "aclcontains" },
  {1038 , 1 , seteval, "seteval" },
  {1044 , 3 , bpcharin, "bpcharin" },
  {1045 , 1 , bpcharout, "bpcharout" },
  {1046 , 3 , varcharin, "varcharin" },
  {1047 , 1 , varcharout, "varcharout" },
  {1048 , 2 , bpchareq, "bpchareq" },
  {1049 , 2 , bpcharlt, "bpcharlt" },
  {1050 , 2 , bpcharle, "bpcharle" },
  {1051 , 2 , bpchargt, "bpchargt" },
  {1052 , 2 , bpcharge, "bpcharge" },
  {1053 , 2 , bpcharne, "bpcharne" },
  {1070 , 2 , varchareq, "varchareq" },
  {1071 , 2 , varcharlt, "varcharlt" },
  {1072 , 2 , varcharle, "varcharle" },
  {1073 , 2 , varchargt, "varchargt" },
  {1074 , 2 , varcharge, "varcharge" },
  {1075 , 2 , varcharne, "varcharne" },
  {1078 , 2 , bpcharcmp, "bpcharcmp" },
  {1079 , 2 , varcharcmp, "varcharcmp" },
  {1080 , 1 , hashbpchar, "hashbpchar" },
  {1081 , 1 , hashvarchar, "hashvarchar" },
  {1084 , 1 , date_in, "date_in" },
  {1085 , 1 , date_out, "date_out" },
  {1086 , 2 , date_eq, "date_eq" },
  {1087 , 2 , date_lt, "date_lt" },
  {1088 , 2 , date_le, "date_le" },
  {1089 , 2 , date_gt, "date_gt" },
  {1090 , 2 , date_ge, "date_ge" },
  {1091 , 2 , date_ne, "date_ne" },
  {1092 , 2 , date_cmp, "date_cmp" },
  {1099 , 1 , time_in, "time_in" },
  {1100 , 1 , time_out, "time_out" },
  {1101 , 2 , time_eq, "time_eq" },
  {1102 , 2 , time_lt, "time_lt" },
  {1103 , 2 , time_le, "time_le" },
  {1104 , 2 , time_gt, "time_gt" },
  {1105 , 2 , time_ge, "time_ge" },
  {1106 , 2 , time_ne, "time_ne" },
  {1107 , 2 , time_cmp, "time_cmp" },
  {1200 , 1 , int42reltime, "int42reltime" },
  {1230 , 2 , char2icregexeq, "char2icregexeq" },
  {1231 , 2 , char2icregexne, "char2icregexne" },
  {1232 , 2 , char4icregexeq, "char4icregexeq" },
  {1233 , 2 , char4icregexne, "char4icregexne" },
  {1234 , 2 , char8icregexeq, "char8icregexeq" },
  {1235 , 2 , char8icregexne, "char8icregexne" },
  {1236 , 2 , char16icregexeq, "char16icregexeq" },
  {1237 , 2 , char16icregexne, "char16icregexne" },
  {1238 , 2 , texticregexeq, "texticregexeq" },
  {1239 , 2 , texticregexne, "texticregexne" },
  {1240 , 2 , nameicregexeq, "nameicregexeq" },
  {1241 , 2 , nameicregexne, "nameicregexne" },
	/* guardian value */
#ifndef WIN32
      { MAXINT, 0, (func_ptr) NULL }
#else
      { INT_MAX, 0, (func_ptr) NULL }
#endif /* WIN32 */
};

static int fmgr_nbuiltins = (sizeof(fmgr_builtins) / sizeof(FmgrCall)) - 1;

FmgrCall *fmgr_isbuiltin(Oid id)
{
    register int i;
    int	low = 0;
    int	high = fmgr_nbuiltins;

    low = 0;
    high = fmgr_nbuiltins;
    while (low <= high) {
	i = low + (high - low) / 2;
	if (id == fmgr_builtins[i].proid)
	    break;
	else if (id > fmgr_builtins[i].proid)
	    low = i + 1;
	else
	    high = i - 1;
    }
    if (id == fmgr_builtins[i].proid)
	return(&fmgr_builtins[i]);
    return((FmgrCall *) NULL);
}

func_ptr fmgr_lookupByName(char *name) 
{
    int i;
    for (i=0;i<fmgr_nbuiltins;i++) {
	if (strcmp(name,fmgr_builtins[i].funcName) == 0)
		return(fmgr_builtins[i].func);
    }
    return((func_ptr) NULL);
}

