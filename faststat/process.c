#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "stater.h"

void process(int type, int u1, int u2, char *part)	{

	PGresult* res;

	char req[256];
	static char *p;
	static char **buffer = &p;

	sprintf(req, "copy %s to STDOUT", part);
	printf(" req: %s\n", req);
	
	res = PQexec(conn, req);
	if(PQresultStatus(res) != PGRES_COPY_OUT)  {

		printf("error: %s\n", PQerrorMessage(conn));
		exit(1);

	}

	printf(" go to %p\n", buffer);

	while( PQgetCopyData( conn, buffer, 0 ) > 0 )	{

		if(*buffer)
			printf(" -> %s", *buffer);

		if(*buffer)
			PQfreemem(*buffer);

	}

}
