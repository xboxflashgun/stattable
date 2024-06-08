// #include <arpa/inet.h>
#include <stdint.h>
#include <endian.h>
#include <libpq-fe.h>

#include "stater.h"

static union {
	struct LINE l;
	char c[sizeof(struct LINE)];
} line;

struct LINE * readstr()	{

	static char *p;
	static char **buffer = &p;

	int res;
	res = PQgetCopyData(conn, buffer, 0);
	if(res < 0)
		return NULL;

	/*
	// dump buffer
	for(int i=0; i!=res; i++)	{
		if(i % 10 == 0)
			printf("\n%04d ", i);
		printf("%02x ", *(unsigned char *)((*buffer)+i));
	}
	printf("\n\n");		*/

	union intchar num;
	num.i = 0;

	num.c[0] = (*buffer)[1];
	num.c[1] = (*buffer)[0];

	if(num.i == 0xffff)
		return NULL;

	// xuid
	line.c[0] = (*buffer)[13];
	line.c[1] = (*buffer)[12];
	line.c[2] = (*buffer)[11];
	line.c[3] = (*buffer)[10];
	line.c[4] = (*buffer)[9];
	line.c[5] = (*buffer)[8];
	line.c[6] = (*buffer)[7];
	line.c[7] = (*buffer)[6];

	// titleid
	line.c[8]  = (*buffer)[25];
	line.c[9]  = (*buffer)[24];
	line.c[10] = (*buffer)[23];
	line.c[11] = (*buffer)[22];

	// utime
	line.c[12] = (*buffer)[33];
	line.c[13] = (*buffer)[32];
	line.c[14] = (*buffer)[31];
	line.c[15] = (*buffer)[30];

	// secs
	
	num.i = 0;	// secs length
	num.c[0] = (*buffer)[37];
	num.c[1] = (*buffer)[36];
	num.c[2] = (*buffer)[35];
	num.c[3] = (*buffer)[34];

	if(num.i < 0)
		line.l.secs = u2-u1;
	else {

		line.c[16] = (*buffer)[41];
		line.c[17] = (*buffer)[40];
		line.c[18] = (*buffer)[39];
		line.c[19] = (*buffer)[38];
	}

	PQfreemem(*buffer);

	return &line.l;

}

