#define uint64 unsigned long long
#define uint32 unsigned long
#define uchar  unsigned char

#pragma pack (push, 4)

extern void process(int type, int u1, int u2, char *part);

extern PGconn* conn;
extern PGresult* res;

// ftree: definitions
typedef struct {
	int l, r;
	// payload of unknown size
} ftree_el;

typedef struct {
	uchar *a;		// pointer to array of elements ftree_el
					// uchar because we don't know actual size of ftree_el
	int N;			// max number of elements (size of array)
	int fp;			// free index pointer, rebalance if fp == N
	int root;		// index of root element
	int so;			// sizeof(ftree_el + payload)
	int maxdepth;	// maximum depth
	int (*comp)(ftree_el *a, ftree_el *b);	// compare function
} ftree;

extern ftree *ftree_init(int N, int so, int (*comp)());
// extern ftree_find(ftree *f, void *el);
extern int ftree_upsert(ftree *f, ftree_el *el);
extern void ftree_dump(ftree *f);
extern void ftree_free(ftree *f);
// 
#define ftree_get(f,p) ((ftree_el *) (f->a + p * f->so))


extern int u1;
extern int u2;

union intchar {
	int i;
	char c[4];
};

// copy.c
struct LINE {
	uint64 xuid;
	int titleid;
	int utime;
	int secs;
};

void dump_tree();

struct LINE * readstr();


#pragma pack (pop)
