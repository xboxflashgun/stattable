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
	struct LINE *line;

	sprintf(req, "copy %s to STDOUT (format BINARY)", part);
	
	res = PQexec(conn, req);
	if(PQresultStatus(res) != PGRES_COPY_OUT)  {

		printf("error: %s\n", PQerrorMessage(conn));
		exit(1);

	}

	int r;
	r = PQgetCopyData( conn, buffer, 0 );	//  skip header
	printf("res-header=%d\n", r);
	PQfreemem(*buffer);

	while( line = readstr() )	{

		printf("%lld %u %d %d\n", line->xuid, line->titleid, line->utime, line->secs);
		exit(0);

	}

}
