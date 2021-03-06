/*-------------------------------------------------------------------------
 *
 * common.c
 *	  common routines between pg_dump and pg4_dump
 *
 * Since pg4_dump is long-dead code, there is no longer any useful distinction
 * between this file and pg_dump.c.
 *
 * Portions Copyright (c) 1996-2005, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/bin/pg_dump/common.c,v 1.85 2004/12/31 22:03:07 pgsql Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres_fe.h"
#include "pg_dump.h"
#include "pg_backup_archiver.h"

#include "postgres.h"
#include "catalog/pg_class.h"

#include <ctype.h>

#include "libpq-fe.h"
#ifndef HAVE_STRDUP
#include "strdup.h"
#endif


/*
 * Variables for mapping DumpId to DumpableObject
 */
static DumpableObject **dumpIdMap = NULL;
static int	allocedDumpIds = 0;
static DumpId lastDumpId = 0;

/*
 * Variables for mapping CatalogId to DumpableObject
 */
static bool catalogIdMapValid = false;
static DumpableObject **catalogIdMap = NULL;
static int	numCatalogIds = 0;

/*
 * These variables are static to avoid the notational cruft of having to pass
 * them into findTableByOid() and friends.
 */
static TableInfo *tblinfo;
static TypeInfo *typinfo;
static FuncInfo *funinfo;
static OprInfo *oprinfo;
static int	numTables;
static int	numTypes;
static int	numFuncs;
static int	numOperators;


static void flagInhTables(TableInfo *tbinfo, int numTables,
			  InhInfo *inhinfo, int numInherits);
static void flagInhAttrs(TableInfo *tbinfo, int numTables,
			 InhInfo *inhinfo, int numInherits);
static int	DOCatalogIdCompare(const void *p1, const void *p2);
static void findParentsByOid(TableInfo *self,
				 InhInfo *inhinfo, int numInherits);
static int	strInArray(const char *pattern, char **arr, int arr_size);


/*
 * getSchemaData
 *	  Collect information about all potentially dumpable objects
 */
TableInfo *
getSchemaData(int *numTablesPtr,
			  const bool schemaOnly,
			  const bool dataOnly)
{
	NamespaceInfo *nsinfo;
	AggInfo    *agginfo;
	InhInfo    *inhinfo;
	RuleInfo   *ruleinfo;
	ProcLangInfo *proclanginfo;
	CastInfo   *castinfo;
	OpclassInfo *opcinfo;
	ConvInfo   *convinfo;
	int			numNamespaces;
	int			numAggregates;
	int			numInherits;
	int			numRules;
	int			numProcLangs;
	int			numCasts;
	int			numOpclasses;
	int			numConversions;

	if (g_verbose)
		write_msg(NULL, "reading schemas\n");
	nsinfo = getNamespaces(&numNamespaces);

	if (g_verbose)
		write_msg(NULL, "reading user-defined functions\n");
	funinfo = getFuncs(&numFuncs);

	/* this must be after getFuncs */
	if (g_verbose)
		write_msg(NULL, "reading user-defined types\n");
	typinfo = getTypes(&numTypes);

	/* this must be after getFuncs, too */
	if (g_verbose)
		write_msg(NULL, "reading procedural languages\n");
	proclanginfo = getProcLangs(&numProcLangs);

	if (g_verbose)
		write_msg(NULL, "reading user-defined aggregate functions\n");
	agginfo = getAggregates(&numAggregates);

	if (g_verbose)
		write_msg(NULL, "reading user-defined operators\n");
	oprinfo = getOperators(&numOperators);

	if (g_verbose)
		write_msg(NULL, "reading user-defined operator classes\n");
	opcinfo = getOpclasses(&numOpclasses);

	if (g_verbose)
		write_msg(NULL, "reading user-defined conversions\n");
	convinfo = getConversions(&numConversions);

	if (g_verbose)
		write_msg(NULL, "reading user-defined tables\n");
	tblinfo = getTables(&numTables);

	if (g_verbose)
		write_msg(NULL, "reading table inheritance information\n");
	inhinfo = getInherits(&numInherits);

	if (g_verbose)
		write_msg(NULL, "reading rewrite rules\n");
	ruleinfo = getRules(&numRules);

	if (g_verbose)
		write_msg(NULL, "reading type casts\n");
	castinfo = getCasts(&numCasts);

	/* Link tables to parents, mark parents of target tables interesting */
	if (g_verbose)
		write_msg(NULL, "finding inheritance relationships\n");
	flagInhTables(tblinfo, numTables, inhinfo, numInherits);

	if (g_verbose)
		write_msg(NULL, "reading column info for interesting tables\n");
	getTableAttrs(tblinfo, numTables);

	if (g_verbose)
		write_msg(NULL, "flagging inherited columns in subtables\n");
	flagInhAttrs(tblinfo, numTables, inhinfo, numInherits);

	if (g_verbose)
		write_msg(NULL, "reading indexes\n");
	getIndexes(tblinfo, numTables);

	if (g_verbose)
		write_msg(NULL, "reading constraints\n");
	getConstraints(tblinfo, numTables);

	if (g_verbose)
		write_msg(NULL, "reading triggers\n");
	getTriggers(tblinfo, numTables);

	*numTablesPtr = numTables;
	return tblinfo;
}

/* flagInhTables -
 *	 Fill in parent link fields of every target table, and mark
 *	 parents of target tables as interesting
 *
 * Note that only direct ancestors of targets are marked interesting.
 * This is sufficient; we don't much care whether they inherited their
 * attributes or not.
 *
 * modifies tblinfo
 */
static void
flagInhTables(TableInfo *tblinfo, int numTables,
			  InhInfo *inhinfo, int numInherits)
{
	int			i,
				j;
	int			numParents;
	TableInfo **parents;

	for (i = 0; i < numTables; i++)
	{
		/* Sequences and views never have parents */
		if (tblinfo[i].relkind == RELKIND_SEQUENCE ||
			tblinfo[i].relkind == RELKIND_VIEW)
			continue;

		/* Don't bother computing anything for non-target tables, either */
		if (!tblinfo[i].dump)
			continue;

		/* Find all the immediate parent tables */
		findParentsByOid(&tblinfo[i], inhinfo, numInherits);

		/* Mark the parents as interesting for getTableAttrs */
		numParents = tblinfo[i].numParents;
		parents = tblinfo[i].parents;
		for (j = 0; j < numParents; j++)
			parents[j]->interesting = true;
	}
}

/* flagInhAttrs -
 *	 for each dumpable table in tblinfo, flag its inherited attributes
 * so when we dump the table out, we don't dump out the inherited attributes
 *
 * modifies tblinfo
 */
static void
flagInhAttrs(TableInfo *tblinfo, int numTables,
			 InhInfo *inhinfo, int numInherits)
{
	int			i,
				j,
				k;

	for (i = 0; i < numTables; i++)
	{
		TableInfo  *tbinfo = &(tblinfo[i]);
		int			numParents;
		TableInfo **parents;
		TableInfo  *parent;

		/* Sequences and views never have parents */
		if (tbinfo->relkind == RELKIND_SEQUENCE ||
			tbinfo->relkind == RELKIND_VIEW)
			continue;

		/* Don't bother computing anything for non-target tables, either */
		if (!tbinfo->dump)
			continue;

		numParents = tbinfo->numParents;
		parents = tbinfo->parents;

		if (numParents == 0)
			continue;			/* nothing to see here, move along */

		/*----------------------------------------------------------------
		 * For each attr, check the parent info: if no parent has an attr
		 * with the same name, then it's not inherited. If there *is* an
		 * attr with the same name, then only dump it if:
		 *
		 * - it is NOT NULL and zero parents are NOT NULL
		 *	 OR
		 * - it has a default value AND the default value does not match
		 *	 all parent default values, or no parents specify a default.
		 *
		 * See discussion on -hackers around 2-Apr-2001.
		 *----------------------------------------------------------------
		 */
		for (j = 0; j < tbinfo->numatts; j++)
		{
			bool		foundAttr;		/* Attr was found in a parent */
			bool		foundNotNull;	/* Attr was NOT NULL in a parent */
			bool		defaultsMatch;	/* All non-empty defaults match */
			bool		defaultsFound;	/* Found a default in a parent */
			AttrDefInfo *attrDef;

			foundAttr = false;
			foundNotNull = false;
			defaultsMatch = true;
			defaultsFound = false;

			attrDef = tbinfo->attrdefs[j];

			for (k = 0; k < numParents; k++)
			{
				int			inhAttrInd;

				parent = parents[k];
				inhAttrInd = strInArray(tbinfo->attnames[j],
										parent->attnames,
										parent->numatts);

				if (inhAttrInd != -1)
				{
					foundAttr = true;
					foundNotNull |= parent->notnull[inhAttrInd];
					if (attrDef != NULL)		/* If we have a default,
												 * check parent */
					{
						AttrDefInfo *inhDef;

						inhDef = parent->attrdefs[inhAttrInd];
						if (inhDef != NULL)
						{
							defaultsFound = true;
							defaultsMatch &= (strcmp(attrDef->adef_expr,
												inhDef->adef_expr) == 0);
						}
					}
				}
			}

			/*
			 * Based on the scan of the parents, decide if we can rely on
			 * the inherited attr
			 */
			if (foundAttr)		/* Attr was inherited */
			{
				/* Set inherited flag by default */
				tbinfo->inhAttrs[j] = true;
				tbinfo->inhAttrDef[j] = true;
				tbinfo->inhNotNull[j] = true;

				/*
				 * Clear it if attr had a default, but parents did not, or
				 * mismatch
				 */
				if ((attrDef != NULL) && (!defaultsFound || !defaultsMatch))
				{
					tbinfo->inhAttrs[j] = false;
					tbinfo->inhAttrDef[j] = false;
				}

				/*
				 * Clear it if NOT NULL and none of the parents were NOT
				 * NULL
				 */
				if (tbinfo->notnull[j] && !foundNotNull)
				{
					tbinfo->inhAttrs[j] = false;
					tbinfo->inhNotNull[j] = false;
				}

				/* Clear it if attr has local definition */
				if (tbinfo->attislocal[j])
					tbinfo->inhAttrs[j] = false;
			}
		}

		/*
		 * Check for inherited CHECK constraints.  We assume a constraint
		 * is inherited if its expression matches the parent and the name
		 * is the same, *or* both names start with '$'.
		 */
		for (j = 0; j < tbinfo->ncheck; j++)
		{
			ConstraintInfo *constr;

			constr = &(tbinfo->checkexprs[j]);

			for (k = 0; k < numParents; k++)
			{
				int			l;

				parent = parents[k];
				for (l = 0; l < parent->ncheck; l++)
				{
					ConstraintInfo *pconstr;

					pconstr = &(parent->checkexprs[l]);
					if (strcmp(pconstr->condef, constr->condef) != 0)
						continue;
					if (strcmp(pconstr->dobj.name, constr->dobj.name) == 0 ||
						(pconstr->dobj.name[0] == '$' &&
						 constr->dobj.name[0] == '$'))
					{
						constr->coninherited = true;
						break;
					}
				}
				if (constr->coninherited)
					break;
			}
		}
	}
}

/*
 * AssignDumpId
 *		Given a newly-created dumpable object, assign a dump ID,
 *		and enter the object into the lookup table.
 *
 * The caller is expected to have filled in objType and catalogId,
 * but not any of the other standard fields of a DumpableObject.
 */
void
AssignDumpId(DumpableObject *dobj)
{
	dobj->dumpId = ++lastDumpId;
	dobj->name = NULL;			/* must be set later */
	dobj->namespace = NULL;		/* may be set later */
	dobj->dependencies = NULL;
	dobj->nDeps = 0;
	dobj->allocDeps = 0;

	while (dobj->dumpId >= allocedDumpIds)
	{
		int			newAlloc;

		if (allocedDumpIds <= 0)
		{
			newAlloc = 256;
			dumpIdMap = (DumpableObject **)
				malloc(newAlloc * sizeof(DumpableObject *));
		}
		else
		{
			newAlloc = allocedDumpIds * 2;
			dumpIdMap = (DumpableObject **)
				realloc(dumpIdMap, newAlloc * sizeof(DumpableObject *));
		}
		if (dumpIdMap == NULL)
			exit_horribly(NULL, NULL, "out of memory\n");
		memset(dumpIdMap + allocedDumpIds, 0,
			   (newAlloc - allocedDumpIds) * sizeof(DumpableObject *));
		allocedDumpIds = newAlloc;
	}
	dumpIdMap[dobj->dumpId] = dobj;

	/* mark catalogIdMap invalid, but don't rebuild it yet */
	catalogIdMapValid = false;
}

/*
 * Assign a DumpId that's not tied to a DumpableObject.
 *
 * This is used when creating a "fixed" ArchiveEntry that doesn't need to
 * participate in the sorting logic.
 */
DumpId
createDumpId(void)
{
	return ++lastDumpId;
}

/*
 * Return the largest DumpId so far assigned
 */
DumpId
getMaxDumpId(void)
{
	return lastDumpId;
}

/*
 * Find a DumpableObject by dump ID
 *
 * Returns NULL for invalid ID
 */
DumpableObject *
findObjectByDumpId(DumpId dumpId)
{
	if (dumpId <= 0 || dumpId >= allocedDumpIds)
		return NULL;			/* out of range? */
	return dumpIdMap[dumpId];
}

/*
 * Find a DumpableObject by catalog ID
 *
 * Returns NULL for unknown ID
 *
 * We use binary search in a sorted list that is built on first call.
 * If AssignDumpId() and findObjectByCatalogId() calls were intermixed,
 * the code would work, but possibly be very slow.	In the current usage
 * pattern that does not happen, indeed we only need to build the list once.
 */
DumpableObject *
findObjectByCatalogId(CatalogId catalogId)
{
	DumpableObject **low;
	DumpableObject **high;

	if (!catalogIdMapValid)
	{
		if (catalogIdMap)
			free(catalogIdMap);
		getDumpableObjects(&catalogIdMap, &numCatalogIds);
		if (numCatalogIds > 1)
			qsort((void *) catalogIdMap, numCatalogIds,
				  sizeof(DumpableObject *), DOCatalogIdCompare);
		catalogIdMapValid = true;
	}

	/*
	 * We could use bsearch() here, but the notational cruft of calling
	 * bsearch is nearly as bad as doing it ourselves; and the generalized
	 * bsearch function is noticeably slower as well.
	 */
	if (numCatalogIds <= 0)
		return NULL;
	low = catalogIdMap;
	high = catalogIdMap + (numCatalogIds - 1);
	while (low <= high)
	{
		DumpableObject **middle;
		int			difference;

		middle = low + (high - low) / 2;
		/* comparison must match DOCatalogIdCompare, below */
		difference = oidcmp((*middle)->catId.oid, catalogId.oid);
		if (difference == 0)
			difference = oidcmp((*middle)->catId.tableoid, catalogId.tableoid);
		if (difference == 0)
			return *middle;
		else if (difference < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}
	return NULL;
}

static int
DOCatalogIdCompare(const void *p1, const void *p2)
{
	DumpableObject *obj1 = *(DumpableObject **) p1;
	DumpableObject *obj2 = *(DumpableObject **) p2;
	int			cmpval;

	/*
	 * Compare OID first since it's usually unique, whereas there will
	 * only be a few distinct values of tableoid.
	 */
	cmpval = oidcmp(obj1->catId.oid, obj2->catId.oid);
	if (cmpval == 0)
		cmpval = oidcmp(obj1->catId.tableoid, obj2->catId.tableoid);
	return cmpval;
}

/*
 * Build an array of pointers to all known dumpable objects
 *
 * This simply creates a modifiable copy of the internal map.
 */
void
getDumpableObjects(DumpableObject ***objs, int *numObjs)
{
	int			i,
				j;

	*objs = (DumpableObject **)
		malloc(allocedDumpIds * sizeof(DumpableObject *));
	if (*objs == NULL)
		exit_horribly(NULL, NULL, "out of memory\n");
	j = 0;
	for (i = 1; i < allocedDumpIds; i++)
	{
		if (dumpIdMap[i])
			(*objs)[j++] = dumpIdMap[i];
	}
	*numObjs = j;
}

/*
 * Add a dependency link to a DumpableObject
 *
 * Note: duplicate dependencies are currently not eliminated
 */
void
addObjectDependency(DumpableObject *dobj, DumpId refId)
{
	if (dobj->nDeps >= dobj->allocDeps)
	{
		if (dobj->allocDeps <= 0)
		{
			dobj->allocDeps = 16;
			dobj->dependencies = (DumpId *)
				malloc(dobj->allocDeps * sizeof(DumpId));
		}
		else
		{
			dobj->allocDeps *= 2;
			dobj->dependencies = (DumpId *)
				realloc(dobj->dependencies,
						dobj->allocDeps * sizeof(DumpId));
		}
		if (dobj->dependencies == NULL)
			exit_horribly(NULL, NULL, "out of memory\n");
	}
	dobj->dependencies[dobj->nDeps++] = refId;
}

/*
 * Remove a dependency link from a DumpableObject
 *
 * If there are multiple links, all are removed
 */
void
removeObjectDependency(DumpableObject *dobj, DumpId refId)
{
	int			i;
	int			j = 0;

	for (i = 0; i < dobj->nDeps; i++)
	{
		if (dobj->dependencies[i] != refId)
			dobj->dependencies[j++] = dobj->dependencies[i];
	}
	dobj->nDeps = j;
}


/*
 * findTableByOid
 *	  finds the entry (in tblinfo) of the table with the given oid
 *	  returns NULL if not found
 *
 * NOTE:  should hash this, but just do linear search for now
 */
TableInfo *
findTableByOid(Oid oid)
{
	int			i;

	for (i = 0; i < numTables; i++)
	{
		if (tblinfo[i].dobj.catId.oid == oid)
			return &tblinfo[i];
	}
	return NULL;
}

/*
 * findTypeByOid
 *	  finds the entry (in typinfo) of the type with the given oid
 *	  returns NULL if not found
 *
 * NOTE:  should hash this, but just do linear search for now
 */
TypeInfo *
findTypeByOid(Oid oid)
{
	int			i;

	for (i = 0; i < numTypes; i++)
	{
		if (typinfo[i].dobj.catId.oid == oid)
			return &typinfo[i];
	}
	return NULL;
}

/*
 * findFuncByOid
 *	  finds the entry (in funinfo) of the function with the given oid
 *	  returns NULL if not found
 *
 * NOTE:  should hash this, but just do linear search for now
 */
FuncInfo *
findFuncByOid(Oid oid)
{
	int			i;

	for (i = 0; i < numFuncs; i++)
	{
		if (funinfo[i].dobj.catId.oid == oid)
			return &funinfo[i];
	}
	return NULL;
}

/*
 * findOprByOid
 *	  finds the entry (in oprinfo) of the operator with the given oid
 *	  returns NULL if not found
 *
 * NOTE:  should hash this, but just do linear search for now
 */
OprInfo *
findOprByOid(Oid oid)
{
	int			i;

	for (i = 0; i < numOperators; i++)
	{
		if (oprinfo[i].dobj.catId.oid == oid)
			return &oprinfo[i];
	}
	return NULL;
}


/*
 * findParentsByOid
 *	  find a table's parents in tblinfo[]
 */
static void
findParentsByOid(TableInfo *self,
				 InhInfo *inhinfo, int numInherits)
{
	Oid			oid = self->dobj.catId.oid;
	int			i,
				j;
	int			numParents;

	numParents = 0;
	for (i = 0; i < numInherits; i++)
	{
		if (inhinfo[i].inhrelid == oid)
			numParents++;
	}

	self->numParents = numParents;

	if (numParents > 0)
	{
		self->parents = (TableInfo **) malloc(sizeof(TableInfo *) * numParents);
		j = 0;
		for (i = 0; i < numInherits; i++)
		{
			if (inhinfo[i].inhrelid == oid)
			{
				TableInfo  *parent;

				parent = findTableByOid(inhinfo[i].inhparent);
				if (parent == NULL)
				{
					write_msg(NULL, "failed sanity check, parent OID %u of table \"%s\" (OID %u) not found\n",
							  inhinfo[i].inhparent,
							  self->dobj.name,
							  oid);
					exit_nicely();
				}
				self->parents[j++] = parent;
			}
		}
	}
	else
		self->parents = NULL;
}

/*
 * parseOidArray
 *	  parse a string of numbers delimited by spaces into a character array
 *
 * Note: actually this is used for both Oids and potentially-signed
 * attribute numbers.  This should cause no trouble, but we could split
 * the function into two functions with different argument types if it does.
 */

void
parseOidArray(const char *str, Oid *array, int arraysize)
{
	int			j,
				argNum;
	char		temp[100];
	char		s;

	argNum = 0;
	j = 0;
	for (;;)
	{
		s = *str++;
		if (s == ' ' || s == '\0')
		{
			if (j > 0)
			{
				if (argNum >= arraysize)
				{
					write_msg(NULL, "could not parse numeric array: too many numbers\n");
					exit_nicely();
				}
				temp[j] = '\0';
				array[argNum++] = atooid(temp);
				j = 0;
			}
			if (s == '\0')
				break;
		}
		else
		{
			if (!(isdigit((unsigned char) s) || s == '-') ||
				j >= sizeof(temp) - 1)
			{
				write_msg(NULL, "could not parse numeric array: invalid character in number\n");
				exit_nicely();
			}
			temp[j++] = s;
		}
	}

	while (argNum < arraysize)
		array[argNum++] = InvalidOid;
}


/*
 * strInArray:
 *	  takes in a string and a string array and the number of elements in the
 * string array.
 *	  returns the index if the string is somewhere in the array, -1 otherwise
 */

static int
strInArray(const char *pattern, char **arr, int arr_size)
{
	int			i;

	for (i = 0; i < arr_size; i++)
	{
		if (strcmp(pattern, arr[i]) == 0)
			return i;
	}
	return -1;
}
