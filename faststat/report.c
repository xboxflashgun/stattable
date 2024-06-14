#include <stdio.h>
#include <libpq-fe.h>

#include "stater.h"

void scan_titleids(int p)	{

	uint32 titleid = ((TITLEID *)ftree_get(titleids, p))->titleid;
	TITDATA *t = (TITDATA *)(((TITLEID *)ftree_get(titleids, p))->td);  // titleid data

	while(t->cl)	{
		if(t->players)
			printf("%u,%d,%d,%d,%lld\n", titleid, t->cl & 0xff, t->cl >> 8, t->players, t->secs);
		t++;
	}

	if(ftree_get(titleids, p)->l != -1)
		scan_titleids(ftree_get(titleids, p)->l);
	if(ftree_get(titleids, p)->r != -1)
		scan_titleids(ftree_get(titleids, p)->r);

}

void report()	{

	printf("Writing report\n");

	scan_titleids( titleids->root );

	printf("End of report\n");

}

