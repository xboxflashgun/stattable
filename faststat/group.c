#include <stdlib.h>
#include <stddef.h>
#include <libpq-fe.h>

#include "stater.h"

void init_titdata( int p )
{

	TITLEID *t = ((TITLEID *)ftree_get(titleids, p));
	
	t->td = calloc(cls, sizeof(TITDATA));

	// setting up country/lang list
	for(int i = 0; i != cls; i++)
		(t->td)[i].cl = cl[i].cl;

	if(ftree_get(titleids, p)->l != -1)
		init_titdata(ftree_get(titleids, p)->l);
	if(ftree_get(titleids, p)->r != -1)
		init_titdata(ftree_get(titleids, p)->r);

}

void process_xuid(uint32 titleid, uint64 secs)	{

	int i;

	for(i = titleids->root; i != -1;)	{
		uint32 t = ((TITLEID *)ftree_get(titleids, i))->titleid;
		if(titleid > t)
			i = ftree_get(titleids, i)->r;
		else if(titleid < t)
			i = ftree_get(titleids, i)->l;
		else
			break;
	}

}

void scan_xuid(int p)	{

	int i;
	TITINFO *pl = (TITINFO *)(xuids->a + offsetof(XUID, pl) + (uint64)p * xuids->so);

	for(i = 0; i != MAXTITLES; i++)
		if(pl->titleid[i] == 1)		{
			pl = (TITINFO *)(pl->secs[i]);
			i = 0;
		} else
			process_xuid(pl->titleid[i], pl->secs[i]);

	if(ftree_get(xuids, p)->l != -1)
		scan_xuid(ftree_get(xuids, p)->l);
	if(ftree_get(xuids, p)->r != -1)
		scan_xuid(ftree_get(xuids, p)->r);

}

void grouptitles()	{

	printf("Grouping\n");
	
	// init TITLEID TITDATA
	init_titdata( titleids->root );

	printf("Titleid prepared, scanning xuids tree\n");

	scan_xuid( xuids->root );

	printf("Writing report\n");

}

