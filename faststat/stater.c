#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

#include "stater.h"

PGconn* conn = NULL;
PGresult* res = NULL;

struct DICT countries[MAXCOUNTRIES];
struct DICT languages[MAXLANGUAGES];

int N, u1, u2;
struct XUID *xuids;

int main(int argc, char *argv[])      {

	if(argc < 5)	{

		printf("usage: %s [type] [utime1] [utime2] [partition1] ...\n", argv[0]);
		return 0;

	}

	conn = PQconnectdb("dbname=global");
	
	if(PQstatus(conn) != CONNECTION_OK)     {

		printf("%s\n", PQerrorMessage(conn));
		return 1;

	}

	int type = atoi(argv[1]);
	u1 = atoi(argv[2]);
	u2 = atoi(argv[3]);

	for(int i = 4; i < argc; i++)	{

		printf("   Processing %s\n", argv[i]);
		process(type, u1, u2, argv[i]);

	}

	PQfinish(conn);

	return 0;

}
