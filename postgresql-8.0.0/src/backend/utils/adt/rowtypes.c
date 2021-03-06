/*-------------------------------------------------------------------------
 *
 * rowtypes.c
 *	  I/O functions for generic composite types.
 *
 * Portions Copyright (c) 1996-2005, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/utils/adt/rowtypes.c,v 1.8 2004/12/31 22:01:22 pgsql Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <ctype.h>

#include "access/heapam.h"
#include "access/htup.h"
#include "catalog/pg_type.h"
#include "lib/stringinfo.h"
#include "libpq/pqformat.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/typcache.h"


/*
 * structure to cache metadata needed for record I/O
 */
typedef struct ColumnIOData
{
	Oid			column_type;
	Oid			typiofunc;
	Oid			typioparam;
	FmgrInfo	proc;
} ColumnIOData;

typedef struct RecordIOData
{
	Oid			record_type;
	int32		record_typmod;
	int			ncolumns;
	ColumnIOData columns[1];	/* VARIABLE LENGTH ARRAY */
} RecordIOData;


/*
 * record_in		- input routine for any composite type.
 */
Datum
record_in(PG_FUNCTION_ARGS)
{
	char	   *string = PG_GETARG_CSTRING(0);
	Oid			tupType = PG_GETARG_OID(1);
	int32		tupTypmod;
	TupleDesc	tupdesc;
	HeapTuple	tuple;
	RecordIOData *my_extra;
	bool		needComma = false;
	int			ncolumns;
	int			i;
	char	   *ptr;
	Datum	   *values;
	char	   *nulls;
	StringInfoData buf;

	/*
	 * Use the passed type unless it's RECORD; we can't support input of
	 * anonymous types, mainly because there's no good way to figure out
	 * which anonymous type is wanted.	Note that for RECORD, what we'll
	 * probably actually get is RECORD's typelem, ie, zero.
	 */
	if (tupType == InvalidOid || tupType == RECORDOID)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
		errmsg("input of anonymous composite types is not implemented")));
	tupTypmod = -1;				/* for all non-anonymous types */
	tupdesc = lookup_rowtype_tupdesc(tupType, tupTypmod);
	ncolumns = tupdesc->natts;

	/*
	 * We arrange to look up the needed I/O info just once per series of
	 * calls, assuming the record type doesn't change underneath us.
	 */
	my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
	if (my_extra == NULL ||
		my_extra->ncolumns != ncolumns)
	{
		fcinfo->flinfo->fn_extra =
			MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
							   sizeof(RecordIOData) - sizeof(ColumnIOData)
							   + ncolumns * sizeof(ColumnIOData));
		my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
		my_extra->record_type = InvalidOid;
		my_extra->record_typmod = 0;
	}

	if (my_extra->record_type != tupType ||
		my_extra->record_typmod != tupTypmod)
	{
		MemSet(my_extra, 0,
			   sizeof(RecordIOData) - sizeof(ColumnIOData)
			   + ncolumns * sizeof(ColumnIOData));
		my_extra->record_type = tupType;
		my_extra->record_typmod = tupTypmod;
		my_extra->ncolumns = ncolumns;
	}

	values = (Datum *) palloc(ncolumns * sizeof(Datum));
	nulls = (char *) palloc(ncolumns * sizeof(char));

	/*
	 * Scan the string.  We use "buf" to accumulate the de-quoted data for
	 * each column, which is then fed to the appropriate input converter.
	 */
	ptr = string;
	/* Allow leading whitespace */
	while (*ptr && isspace((unsigned char) *ptr))
		ptr++;
	if (*ptr++ != '(')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("malformed record literal: \"%s\"", string),
				 errdetail("Missing left parenthesis.")));

	initStringInfo(&buf);

	for (i = 0; i < ncolumns; i++)
	{
		ColumnIOData *column_info = &my_extra->columns[i];
		Oid			column_type = tupdesc->attrs[i]->atttypid;

		/* Ignore dropped columns in datatype, but fill with nulls */
		if (tupdesc->attrs[i]->attisdropped)
		{
			values[i] = (Datum) 0;
			nulls[i] = 'n';
			continue;
		}

		if (needComma)
		{
			/* Skip comma that separates prior field from this one */
			if (*ptr == ',')
				ptr++;
			else
				/* *ptr must be ')' */
				ereport(ERROR,
						(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					  errmsg("malformed record literal: \"%s\"", string),
						 errdetail("Too few columns.")));
		}

		/* Check for null: completely empty input means null */
		if (*ptr == ',' || *ptr == ')')
		{
			values[i] = (Datum) 0;
			nulls[i] = 'n';
		}
		else
		{
			/* Extract string for this column */
			bool		inquote = false;

			buf.len = 0;
			buf.data[0] = '\0';
			while (inquote || !(*ptr == ',' || *ptr == ')'))
			{
				char		ch = *ptr++;

				if (ch == '\0')
					ereport(ERROR,
							(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
							 errmsg("malformed record literal: \"%s\"",
									string),
							 errdetail("Unexpected end of input.")));
				if (ch == '\\')
				{
					if (*ptr == '\0')
						ereport(ERROR,
						   (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
							errmsg("malformed record literal: \"%s\"",
								   string),
							errdetail("Unexpected end of input.")));
					appendStringInfoChar(&buf, *ptr++);
				}
				else if (ch == '\"')
				{
					if (!inquote)
						inquote = true;
					else if (*ptr == '\"')
					{
						/* doubled quote within quote sequence */
						appendStringInfoChar(&buf, *ptr++);
					}
					else
						inquote = false;
				}
				else
					appendStringInfoChar(&buf, ch);
			}

			/*
			 * Convert the column value
			 */
			if (column_info->column_type != column_type)
			{
				getTypeInputInfo(column_type,
								 &column_info->typiofunc,
								 &column_info->typioparam);
				fmgr_info_cxt(column_info->typiofunc, &column_info->proc,
							  fcinfo->flinfo->fn_mcxt);
				column_info->column_type = column_type;
			}

			values[i] = FunctionCall3(&column_info->proc,
									  CStringGetDatum(buf.data),
							   ObjectIdGetDatum(column_info->typioparam),
							Int32GetDatum(tupdesc->attrs[i]->atttypmod));
			nulls[i] = ' ';
		}

		/*
		 * Prep for next column
		 */
		needComma = true;
	}

	if (*ptr++ != ')')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("malformed record literal: \"%s\"", string),
				 errdetail("Too many columns.")));
	/* Allow trailing whitespace */
	while (*ptr && isspace((unsigned char) *ptr))
		ptr++;
	if (*ptr)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("malformed record literal: \"%s\"", string),
				 errdetail("Junk after right parenthesis.")));

	tuple = heap_formtuple(tupdesc, values, nulls);

	pfree(buf.data);
	pfree(values);
	pfree(nulls);

	PG_RETURN_HEAPTUPLEHEADER(tuple->t_data);
}

/*
 * record_out		- output routine for any composite type.
 */
Datum
record_out(PG_FUNCTION_ARGS)
{
	HeapTupleHeader rec = PG_GETARG_HEAPTUPLEHEADER(0);
	Oid			tupType = PG_GETARG_OID(1);
	int32		tupTypmod;
	TupleDesc	tupdesc;
	HeapTupleData tuple;
	RecordIOData *my_extra;
	bool		needComma = false;
	int			ncolumns;
	int			i;
	Datum	   *values;
	char	   *nulls;
	StringInfoData buf;

	/*
	 * Use the passed type unless it's RECORD; in that case, we'd better
	 * get the type info out of the datum itself.  Note that for RECORD,
	 * what we'll probably actually get is RECORD's typelem, ie, zero.
	 */
	if (tupType == InvalidOid || tupType == RECORDOID)
	{
		tupType = HeapTupleHeaderGetTypeId(rec);
		tupTypmod = HeapTupleHeaderGetTypMod(rec);
	}
	else
		tupTypmod = -1;
	tupdesc = lookup_rowtype_tupdesc(tupType, tupTypmod);
	ncolumns = tupdesc->natts;

	/* Build a temporary HeapTuple control structure */
	tuple.t_len = HeapTupleHeaderGetDatumLength(rec);
	ItemPointerSetInvalid(&(tuple.t_self));
	tuple.t_tableOid = InvalidOid;
	tuple.t_data = rec;

	/*
	 * We arrange to look up the needed I/O info just once per series of
	 * calls, assuming the record type doesn't change underneath us.
	 */
	my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
	if (my_extra == NULL ||
		my_extra->ncolumns != ncolumns)
	{
		fcinfo->flinfo->fn_extra =
			MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
							   sizeof(RecordIOData) - sizeof(ColumnIOData)
							   + ncolumns * sizeof(ColumnIOData));
		my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
		my_extra->record_type = InvalidOid;
		my_extra->record_typmod = 0;
	}

	if (my_extra->record_type != tupType ||
		my_extra->record_typmod != tupTypmod)
	{
		MemSet(my_extra, 0,
			   sizeof(RecordIOData) - sizeof(ColumnIOData)
			   + ncolumns * sizeof(ColumnIOData));
		my_extra->record_type = tupType;
		my_extra->record_typmod = tupTypmod;
		my_extra->ncolumns = ncolumns;
	}

	values = (Datum *) palloc(ncolumns * sizeof(Datum));
	nulls = (char *) palloc(ncolumns * sizeof(char));

	/* Break down the tuple into fields */
	heap_deformtuple(&tuple, tupdesc, values, nulls);

	/* And build the result string */
	initStringInfo(&buf);

	appendStringInfoChar(&buf, '(');

	for (i = 0; i < ncolumns; i++)
	{
		ColumnIOData *column_info = &my_extra->columns[i];
		Oid			column_type = tupdesc->attrs[i]->atttypid;
		char	   *value;
		char	   *tmp;
		bool		nq;

		/* Ignore dropped columns in datatype */
		if (tupdesc->attrs[i]->attisdropped)
			continue;

		if (needComma)
			appendStringInfoChar(&buf, ',');
		needComma = true;

		if (nulls[i] == 'n')
		{
			/* emit nothing... */
			continue;
		}

		/*
		 * Convert the column value to text
		 */
		if (column_info->column_type != column_type)
		{
			bool		typIsVarlena;

			getTypeOutputInfo(column_type,
							  &column_info->typiofunc,
							  &column_info->typioparam,
							  &typIsVarlena);
			fmgr_info_cxt(column_info->typiofunc, &column_info->proc,
						  fcinfo->flinfo->fn_mcxt);
			column_info->column_type = column_type;
		}

		value = DatumGetCString(FunctionCall3(&column_info->proc,
											  values[i],
							   ObjectIdGetDatum(column_info->typioparam),
						   Int32GetDatum(tupdesc->attrs[i]->atttypmod)));

		/* Detect whether we need double quotes for this value */
		nq = (value[0] == '\0');	/* force quotes for empty string */
		for (tmp = value; *tmp; tmp++)
		{
			char		ch = *tmp;

			if (ch == '"' || ch == '\\' ||
				ch == '(' || ch == ')' || ch == ',' ||
				isspace((unsigned char) ch))
			{
				nq = true;
				break;
			}
		}

		/* And emit the string */
		if (nq)
			appendStringInfoChar(&buf, '"');
		for (tmp = value; *tmp; tmp++)
		{
			char		ch = *tmp;

			if (ch == '"' || ch == '\\')
				appendStringInfoChar(&buf, ch);
			appendStringInfoChar(&buf, ch);
		}
		if (nq)
			appendStringInfoChar(&buf, '"');
	}

	appendStringInfoChar(&buf, ')');

	pfree(values);
	pfree(nulls);

	PG_RETURN_CSTRING(buf.data);
}

/*
 * record_recv		- binary input routine for any composite type.
 */
Datum
record_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Oid			tupType = PG_GETARG_OID(1);
	int32		tupTypmod;
	TupleDesc	tupdesc;
	HeapTuple	tuple;
	RecordIOData *my_extra;
	int			ncolumns;
	int			usercols;
	int			validcols;
	int			i;
	Datum	   *values;
	char	   *nulls;

	/*
	 * Use the passed type unless it's RECORD; we can't support input of
	 * anonymous types, mainly because there's no good way to figure out
	 * which anonymous type is wanted.	Note that for RECORD, what we'll
	 * probably actually get is RECORD's typelem, ie, zero.
	 */
	if (tupType == InvalidOid || tupType == RECORDOID)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
		errmsg("input of anonymous composite types is not implemented")));
	tupTypmod = -1;				/* for all non-anonymous types */
	tupdesc = lookup_rowtype_tupdesc(tupType, tupTypmod);
	ncolumns = tupdesc->natts;

	/*
	 * We arrange to look up the needed I/O info just once per series of
	 * calls, assuming the record type doesn't change underneath us.
	 */
	my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
	if (my_extra == NULL ||
		my_extra->ncolumns != ncolumns)
	{
		fcinfo->flinfo->fn_extra =
			MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
							   sizeof(RecordIOData) - sizeof(ColumnIOData)
							   + ncolumns * sizeof(ColumnIOData));
		my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
		my_extra->record_type = InvalidOid;
		my_extra->record_typmod = 0;
	}

	if (my_extra->record_type != tupType ||
		my_extra->record_typmod != tupTypmod)
	{
		MemSet(my_extra, 0,
			   sizeof(RecordIOData) - sizeof(ColumnIOData)
			   + ncolumns * sizeof(ColumnIOData));
		my_extra->record_type = tupType;
		my_extra->record_typmod = tupTypmod;
		my_extra->ncolumns = ncolumns;
	}

	values = (Datum *) palloc(ncolumns * sizeof(Datum));
	nulls = (char *) palloc(ncolumns * sizeof(char));

	/* Fetch number of columns user thinks it has */
	usercols = pq_getmsgint(buf, 4);

	/* Need to scan to count nondeleted columns */
	validcols = 0;
	for (i = 0; i < ncolumns; i++)
	{
		if (!tupdesc->attrs[i]->attisdropped)
			validcols++;
	}
	if (usercols != validcols)
		ereport(ERROR,
				(errcode(ERRCODE_DATATYPE_MISMATCH),
				 errmsg("wrong number of columns: %d, expected %d",
						usercols, validcols)));

	/* Process each column */
	for (i = 0; i < ncolumns; i++)
	{
		ColumnIOData *column_info = &my_extra->columns[i];
		Oid			column_type = tupdesc->attrs[i]->atttypid;
		Oid			coltypoid;
		int			itemlen;

		/* Ignore dropped columns in datatype, but fill with nulls */
		if (tupdesc->attrs[i]->attisdropped)
		{
			values[i] = (Datum) 0;
			nulls[i] = 'n';
			continue;
		}

		/* Verify column datatype */
		coltypoid = pq_getmsgint(buf, sizeof(Oid));
		if (coltypoid != column_type)
			ereport(ERROR,
					(errcode(ERRCODE_DATATYPE_MISMATCH),
					 errmsg("wrong data type: %u, expected %u",
							coltypoid, column_type)));

		/* Get and check the item length */
		itemlen = pq_getmsgint(buf, 4);
		if (itemlen < -1 || itemlen > (buf->len - buf->cursor))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
					 errmsg("insufficient data left in message")));

		if (itemlen == -1)
		{
			/* -1 length means NULL */
			values[i] = (Datum) 0;
			nulls[i] = 'n';
		}
		else
		{
			/*
			 * Rather than copying data around, we just set up a phony
			 * StringInfo pointing to the correct portion of the input
			 * buffer. We assume we can scribble on the input buffer so as
			 * to maintain the convention that StringInfos have a trailing
			 * null.
			 */
			StringInfoData item_buf;
			char		csave;

			item_buf.data = &buf->data[buf->cursor];
			item_buf.maxlen = itemlen + 1;
			item_buf.len = itemlen;
			item_buf.cursor = 0;

			buf->cursor += itemlen;

			csave = buf->data[buf->cursor];
			buf->data[buf->cursor] = '\0';

			/* Now call the column's receiveproc */
			if (column_info->column_type != column_type)
			{
				getTypeBinaryInputInfo(column_type,
									   &column_info->typiofunc,
									   &column_info->typioparam);
				fmgr_info_cxt(column_info->typiofunc, &column_info->proc,
							  fcinfo->flinfo->fn_mcxt);
				column_info->column_type = column_type;
			}

			values[i] = FunctionCall2(&column_info->proc,
									  PointerGetDatum(&item_buf),
							  ObjectIdGetDatum(column_info->typioparam));

			nulls[i] = ' ';

			/* Trouble if it didn't eat the whole buffer */
			if (item_buf.cursor != itemlen)
				ereport(ERROR,
						(errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
					 errmsg("improper binary format in record column %d",
							i + 1)));

			buf->data[buf->cursor] = csave;
		}
	}

	tuple = heap_formtuple(tupdesc, values, nulls);

	pfree(values);
	pfree(nulls);

	PG_RETURN_HEAPTUPLEHEADER(tuple->t_data);
}

/*
 * record_send		- binary output routine for any composite type.
 */
Datum
record_send(PG_FUNCTION_ARGS)
{
	HeapTupleHeader rec = PG_GETARG_HEAPTUPLEHEADER(0);
	Oid			tupType = PG_GETARG_OID(1);
	int32		tupTypmod;
	TupleDesc	tupdesc;
	HeapTupleData tuple;
	RecordIOData *my_extra;
	int			ncolumns;
	int			validcols;
	int			i;
	Datum	   *values;
	char	   *nulls;
	StringInfoData buf;

	/*
	 * Use the passed type unless it's RECORD; in that case, we'd better
	 * get the type info out of the datum itself.  Note that for RECORD,
	 * what we'll probably actually get is RECORD's typelem, ie, zero.
	 */
	if (tupType == InvalidOid || tupType == RECORDOID)
	{
		tupType = HeapTupleHeaderGetTypeId(rec);
		tupTypmod = HeapTupleHeaderGetTypMod(rec);
	}
	else
		tupTypmod = -1;
	tupdesc = lookup_rowtype_tupdesc(tupType, tupTypmod);
	ncolumns = tupdesc->natts;

	/* Build a temporary HeapTuple control structure */
	tuple.t_len = HeapTupleHeaderGetDatumLength(rec);
	ItemPointerSetInvalid(&(tuple.t_self));
	tuple.t_tableOid = InvalidOid;
	tuple.t_data = rec;

	/*
	 * We arrange to look up the needed I/O info just once per series of
	 * calls, assuming the record type doesn't change underneath us.
	 */
	my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
	if (my_extra == NULL ||
		my_extra->ncolumns != ncolumns)
	{
		fcinfo->flinfo->fn_extra =
			MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
							   sizeof(RecordIOData) - sizeof(ColumnIOData)
							   + ncolumns * sizeof(ColumnIOData));
		my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
		my_extra->record_type = InvalidOid;
		my_extra->record_typmod = 0;
	}

	if (my_extra->record_type != tupType ||
		my_extra->record_typmod != tupTypmod)
	{
		MemSet(my_extra, 0,
			   sizeof(RecordIOData) - sizeof(ColumnIOData)
			   + ncolumns * sizeof(ColumnIOData));
		my_extra->record_type = tupType;
		my_extra->record_typmod = tupTypmod;
		my_extra->ncolumns = ncolumns;
	}

	values = (Datum *) palloc(ncolumns * sizeof(Datum));
	nulls = (char *) palloc(ncolumns * sizeof(char));

	/* Break down the tuple into fields */
	heap_deformtuple(&tuple, tupdesc, values, nulls);

	/* And build the result string */
	pq_begintypsend(&buf);

	/* Need to scan to count nondeleted columns */
	validcols = 0;
	for (i = 0; i < ncolumns; i++)
	{
		if (!tupdesc->attrs[i]->attisdropped)
			validcols++;
	}
	pq_sendint(&buf, validcols, 4);

	for (i = 0; i < ncolumns; i++)
	{
		ColumnIOData *column_info = &my_extra->columns[i];
		Oid			column_type = tupdesc->attrs[i]->atttypid;
		bytea	   *outputbytes;

		/* Ignore dropped columns in datatype */
		if (tupdesc->attrs[i]->attisdropped)
			continue;

		pq_sendint(&buf, column_type, sizeof(Oid));

		if (nulls[i] == 'n')
		{
			/* emit -1 data length to signify a NULL */
			pq_sendint(&buf, -1, 4);
			continue;
		}

		/*
		 * Convert the column value to binary
		 */
		if (column_info->column_type != column_type)
		{
			bool		typIsVarlena;

			getTypeBinaryOutputInfo(column_type,
									&column_info->typiofunc,
									&column_info->typioparam,
									&typIsVarlena);
			fmgr_info_cxt(column_info->typiofunc, &column_info->proc,
						  fcinfo->flinfo->fn_mcxt);
			column_info->column_type = column_type;
		}

		outputbytes = DatumGetByteaP(FunctionCall2(&column_info->proc,
												   values[i],
							 ObjectIdGetDatum(column_info->typioparam)));

		/* We assume the result will not have been toasted */
		pq_sendint(&buf, VARSIZE(outputbytes) - VARHDRSZ, 4);
		pq_sendbytes(&buf, VARDATA(outputbytes),
					 VARSIZE(outputbytes) - VARHDRSZ);
		pfree(outputbytes);
	}

	pfree(values);
	pfree(nulls);

	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}
