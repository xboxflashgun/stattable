#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "stater.h"

void read_init(struct DICT *dict, char *req)	{

	PGresult* res;

	res = PQexec(conn, req);
	if(PQresultStatus(res) != PGRES_TUPLES_OK)  {

		printf("%s\n", PQerrorMessage(conn));
		exit(1);

	}

	int nrows = PQntuples(res);
	for(int i = 0; i < nrows; i++)  {

		char *w1  = PQgetvalue(res, i, 0);
		char *w2  = PQgetvalue(res, i, 1);

		strncpy(dict[i].name, w1, 3);
		dict[i].id = atoi(w2);
//		printf("%s %s\n", w1, w2);

	}
	PQclear(res);

}


char get_countries[] = 
"select "
"		countryid, "
"		country, "
"		max(pl) as pl "
"from ( "
"	select "
"		countryid, "
"		country, "
"		max(players) as pl "
"	from stattab "
"	join countries using (countryid) "
"	where "
"		titleid is null "
"		and langid is null " 
"	group by 1,2 "
"	union "
"	select "
"		countryid, "
"		country, "
"		0 as pl "
"	from countries "
") as foo "
"group by 1,2 "
"order by 3 desc";

char get_languages[] =
"select "
"		langid, "
"		lang, "
"		max(pl) as pl "
"from ( "
"	select "
"		langid, "
"		lang, "
"		max(players) as pl "
"	from stattab "
"	join languages using (langid) "
"	where "
"		titleid is null "
"		and countryid is null "
"	group by 1,2 "
"	union "
"	select "
"		langid, "
"		lang, "
"		0 as pl "
"	from languages "
") as foo " 
"group by 1,2 "
"order by 3 desc";

