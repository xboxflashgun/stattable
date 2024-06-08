#define int4 unsigned long
#define int8 unsigned long long

#define MAXCOUNTRIES (256)
#define MAXLANGUAGES (200)

struct DICT {
	int id;
	char name[4];
};

extern void read_init(struct DICT *dict, char *req);
extern void process(int type, int u1, int u2, char *part);

extern char get_countries[];
extern char get_languages[];

extern PGconn* conn;
extern PGresult* res;

struct TITLEIDLONG	{
	int t;
	int secs;
};

struct XUID {
	int8 xuid;		// 6 bytes xuid + li + ci
	int l,r;		// index of left and right elements
	union {
		int t1,t2;
		 struct TITLEIDLONG *tp;
	} t;
	int t3, t4;
};

extern void rebalance();

extern struct XUID *xuids;
extern int N;	// number of xuids so far
extern int u1;
extern int u2;

union intchar {
	int i;
	char c[4];
};

// copy.c
#pragma pack (push, 4)
struct LINE {
	int8 xuid;
	int titleid;
	int utime;
	int secs;
};

#pragma pack (pop)

struct LINE * readstr();
