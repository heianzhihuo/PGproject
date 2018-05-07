#include "postgres.h"

#include "miscadmin.h"		/* where the declarations go */

#include "storage/fd.h"		/* for O_ */

/*  from globals.c */
extern char *DatabaseName;
extern char *UserName;
extern char *DatabasePath;

/*
 * GetDatabaseName --
 *	Returns name of database.
 */

char*
GetDatabaseName()
{
    return DatabaseName;
}
