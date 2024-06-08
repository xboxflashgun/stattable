// #include <arpa/inet.h>
#include <stdint.h>
#include <endian.h>
#include <libpq-fe.h>

#include "stater.h"

static struct LINE line;

struct LINE * readstr()	{

	static char *p;
	static char **buffer = &p;

	int8 xuid;
	unsigned int titleid;
	int utime;
	int secs;

	printf("str1\n");

	int res;
	res = PQgetCopyData(conn, buffer, 0);
	if(res < 0)
		return NULL;

	printf("res=%d\n", res);
	
	for(int i=0; i!=res; i++)	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ", *(unsigned char *)((*buffer)+i));
	}
	printf("\n");

	printf(" > %016llx\n", *(int8 *)(*buffer));
	printf(" > %016llx\n", *(int8 *)((*buffer)+8));
	printf(" > %016llx\n", *(int8 *)(*buffer+16));
	printf(" > %016llx\n", *(int8 *)(*buffer+24));

	xuid = *(int8 *)((*buffer) + 8);
	titleid = *(unsigned int *)((*buffer) + 24);
	utime = *(int *)((*buffer) + 32);
	secs = *(int *)((*buffer) + 40);

	printf("%llx %x %x %x\n", xuid,titleid,utime,secs);

	// line.xuid = be64toh(xuid);
	line.xuid = xuid;
	line.titleid = be32toh(titleid);
	line.utime = be32toh(utime);
	line.secs = be32toh(secs);

	PQfreemem(*buffer);

	return &line;

}

