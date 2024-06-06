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
