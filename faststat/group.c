#include <libpq-fe.h>

#include "stater.h"

void init_titdata( int p )
{

}

void grouptitles()	{

	printf("grouping\n");
	
	// init TITLEID TITDATA
	init_titdata( titleids->root );

}

