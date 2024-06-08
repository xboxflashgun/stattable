#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "stater.h"

void process_line(int8 xuid, int titleid, int utime, int secs)	{

	int ut = utime, lt = utime + secs;
	if(ut >= u2 || lt < u1)
		return;
	if(ut < u1)
		ut = u1;
	if(lt > u2)
		lt = u2;
	secs = lt - ut;

	int p, np;
	for(p = np = fp; p != -1;p = np)	{
		if(xuid == xuids[p].xuid)
			break;
		if(xuid > xuids[p].xuid)
			np = xuids[p].r;
		else
			np = xuids[p].l;
	}

	if(p == -1)
		p = fp++;		// next element in tree
	if(np != -1)


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
		process_line(line->xuid, line->titleid, line->utime, line->secs);

	}

}
