#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

#include "stater.h"

PGconn* conn = NULL;
PGresult* res = NULL;

struct DICT countries[MAXCOUNTRIES];
struct DICT languages[MAXLANGUAGES];

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

	read_init(countries, get_countries);
	read_init(languages, get_languages);

	int type = atoi(argv[1]);
	int u1 = atoi(argv[2]);
	int u2 = atoi(argv[3]);

	for(int i = 4; i < argc; i++)	{

		printf("   Processing %s\n", argv[i]);

	}

	PQfinish(conn);

	return 0;

}
