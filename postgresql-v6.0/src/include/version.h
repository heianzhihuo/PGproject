/*-------------------------------------------------------------------------
 *
 * version.h--
 *    this file contains the interface to version.c.
 *    Also some parameters.
 *
 * $Id: version.h,v 1.1 1996/11/11 14:24:38 bryanh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef VERSION_H
#define VERSION_H

void
ValidatePgVersion(const char *path, char **reason_p);

void
SetPgVersion(const char *path, char **reason_p);

#define	PG_RELEASE	6
#define PG_VERSION	0
#define	PG_VERFILE	"PG_VERSION"

#endif
