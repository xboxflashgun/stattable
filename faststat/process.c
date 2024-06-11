#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <libpq-fe.h>

#include "stater.h"

#define MAXTITLES (5)
#define TITPOOLSIZE (1024)

#pragma pack (push, 4)

typedef struct {
	uint64 secs[MAXTITLES];
	uint32 titleid[MAXTITLES];
} TITINFO;

typedef struct {
	ftree_el ftr;
	uint64 xuid;
	TITINFO pl;			// payload
} XUID;

typedef struct {
	ftree_el ftr;
	uint32 titleid;		// ftree payload
} TITLEID;

#pragma pack (pop)

static int xuidcomp(XUID *a, XUID *b)	{

	return a->xuid - b->xuid;

}

static int titleidcomp(TITLEID *a, TITLEID *b)	{

	return a->titleid - b->titleid;

}

ftree *xuids;
ftree *titleids;

static TITINFO *tpool;		// link to allocated pool
static int tpoolcnt;

void process_line(uint64 xuid, uint32 titleid, int utime, int secs)	{

	int ut = utime, lt = utime + secs;
	int i, p;
	XUID el;
	TITLEID t;

	if(ut >= u2 || lt < u1)
		return;
	if(ut < u1)
		ut = u1;
	if(lt > u2)
		lt = u2;
	secs = lt - ut;
	if(secs == 0)
		return;

	memset(&el, 0, sizeof(el));
	el.xuid = xuid;
	p = ftree_upsert(xuids, (ftree_el *)&el);

	// p -- index of xuid found
	TITINFO *pl = (TITINFO *)(xuids->a + offsetof(XUID, pl) + p * xuids->so);
	
	for(i = 0; i != MAXTITLES; i++)
		if(pl->titleid[i] == titleid)
			break;
		else if(pl->titleid[i] == 0)
			break;
		else if(pl->titleid[i] == 1)	{
			// pointer to new payload table
			printf("p=%d i=%d titleid=%d pl->titleid[i]=%d pl=%p secs=%llx\n", p, i, titleid, pl->titleid[i], pl, pl->secs[i]);
			pl = (TITINFO *)(pl->secs[i]);
			i = 0;
		}

	if(i == MAXTITLES)	{

		// do wee need to allocate new pool?
		if(tpoolcnt-- == 0)	{

			tpool = calloc(TITPOOLSIZE, sizeof(TITINFO));
			tpoolcnt = TITPOOLSIZE;
			printf("   allocated new pool\n");

		}

		// save prev info to new structue and set up t[MAXTITLES-1] to 1
		tpool->titleid[0] = pl->titleid[MAXTITLES-1];
		tpool->secs[0] = pl->secs[MAXTITLES-1];
		pl->titleid[MAXTITLES-1] = 1;
		pl->secs[MAXTITLES-1] = (uint64) tpool;

		i = 1;		// because we just wrote to [0]
		pl = tpool++;
	
	}

	pl->titleid[i] = titleid;
	pl->secs[i] += secs;

	// counting different titleids
	memset(&t, 0, sizeof(t));
	t.titleid = titleid;
	ftree_upsert(titleids, (ftree_el *)&t);


}


void process(int type, int u1, int u2, char *part)	{

	PGresult* res;

	char req[256];
	static char *p;
	static char **buffer = &p;
	LINE *line;

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
	titleids = ftree_init(8191, sizeof(TITLEID), titleidcomp);

	while( line = readstr() )	{

		process_line(line->xuid, line->titleid, line->utime, line->secs);

	}

	printf("Total xuids: %d, titleids: %d", xuids->fp, titleids->fp);

}
