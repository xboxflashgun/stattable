#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "stater.h"

#define MAXTITLES (5)

typedef struct {
	ftree_el ftr;
	uint64 xuid;
	uint64 secs[MAXTITLES];
	uint32 titleid[MAXTITLES];
} XUID;

static int xuidcomp(XUID *a, XUID *b)	{

	return a->xuid - b->xuid;

}

typedef struct {
	ftree_el ftr;
	uint32 titleid;
} TITLEID;

static int titleidcomp(TITLEID *a, TITLEID *b)	{

	return a->titleid - b->titleid;

}

ftree *xuids;
ftree *titleids;

void process_line(uint64 xuid, int titleid, int utime, int secs)	{

	int ut = utime, lt = utime + secs;
	int p;
	XUID el;
	TITLEID t;

	if(ut >= u2 || lt < u1)
		return;
	if(ut < u1)
		ut = u1;
	if(lt > u2)
		lt = u2;
	secs = lt - ut;


	memset(&el, 0, sizeof(el));
	el.xuid = xuid;

	memset(&t, 0, sizeof(t));
	t.titleid = titleid;

	p = ftree_upsert(xuids, (ftree_el *)&el);
	ftree_upsert(titleids, (ftree_el *)&t);

}


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

	xuids = ftree_init(65535, sizeof(XUID), xuidcomp);
	titleids = ftree_init(8192, sizeof(TITLEID), titleidcomp);

	while( line = readstr() )	{

		process_line(line->xuid, line->titleid, line->utime, line->secs);

	}

	printf("Total xuids: %d, titleids: %d", xuids->fp, titleids->fp);

}
