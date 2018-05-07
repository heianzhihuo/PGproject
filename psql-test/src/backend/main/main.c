#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "postgres.h"
#include "miscadmin.h"

#include "utils/rel.h"



int
main(int argc, char *argv[])
{
	Relation rdesc;
	char name[] = "Hello\0";
	int len;
	char *relname = name;
	int i;
	
	len = sizeof(RelationData);
	rdesc = (Relation) malloc(len);
	memset((char *)rdesc, 0,len);
	rdesc->rd_isnailed = false;
	RelationSetReferenceCount(rdesc, 1);
	rdesc->rd_rel = (Form_pg_class)malloc(sizeof *rdesc->rd_rel);
	memset((char *)rdesc->rd_rel, 0,sizeof *rdesc->rd_rel);
	memset(rdesc->rd_rel->relname,0,sizeof(NameData));
	strncpy(rdesc->rd_rel->relname.data,name, NAMEDATALEN);
	
		

	//namestrcpy(&(rdesc->rd_rel->relname),&name);
	printf("%d\n",rdesc->rd_rel);
	printf("%d\n",rdesc->rd_rel->relname);	
	printf("len:%d\n",len);
	
	printf("Hello World!\n");
}

