#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "stater.h"

void addsecs(int p, int titleid, int secs)	{

}

int maxdepth;

static void subdump(int p, int lev)	{

//	printf("   lev=%2d, %2d: %lld l->(%d) r->(%d)\n", lev, p, xuids[p].xuid, xuids[p].l, xuids[p].r);

	if( xuids[p].l != -1 )
		subdump(xuids[p].l, lev + 1);
	if( xuids[p].r != -1 )
		subdump(xuids[p].r, lev + 1);
	if( lev > maxdepth )
		maxdepth = lev;

}

void dump_tree()	{

	maxdepth = 0;
	subdump(tree, 0);
	printf("---> maxdepth = %d <---\n", maxdepth);

}

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
	for(p = np = tree; p != -1; )	{
		if(xuid == xuids[p].xuid)
			break;
		np = p;
		if(xuid > xuids[p].xuid)
			p = xuids[p].r;
		else
			p = xuids[p].l;
	}

	if(p == -1)		{

		p = fp++;						// next element in tree
		xuids[p].xuid = xuid;
		xuids[p].l = xuids[p].r = -1;	// no children in new node

		if(np != -1)					// point parent to this node
			if(xuid > xuids[np].xuid)
				xuids[np].r = p;
			else
				xuids[np].l = p;
		else
			if(tree < 0)
				tree = p;

	} else
		printf("found: %d\n", p);


	addsecs(p, titleid, secs);

	if(fp == N)		{					// no more free nodes
		printf("Rebalancing:\n");
		rebalance();
	}

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

	while( line = readstr() )	{

		process_line(line->xuid, line->titleid, line->utime, line->secs);

	}

}
