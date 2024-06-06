#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

#include "stater.h"

static PGconn* conn = NULL;
static PGresult* res = NULL;

struct DICT countries[MAXCOUNTRIES];
struct DICT languages[MAXLANGUAGES];

int main()      {

	conn = PQconnectdb("dbname=global");
	
	if(PQstatus(conn) != CONNECTION_OK)     {

		printf("%s\n", PQerrorMessage(conn));
		return 1;

	}

	res = PQexec(conn, "select now()");
	if(PQresultStatus(res) != PGRES_TUPLES_OK)	{

		printf("%s\n", PQerrorMessage(conn));
		return 1;

	}

	int nrows = PQntuples(res);
	for(int i = 0; i < nrows; i++)  {

		char *w1  = PQgetvalue(res, i, 0);
		printf("%s\n", w1);

	}

	PQclear(res);
	PQfinish(conn);

	return 0;

}
