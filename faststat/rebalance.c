#include <stdlib.h>
#include <libpq-fe.h>

#include "stater.h"

int p;
int *sorted;

// extract to sorted from xuids tree
static void mksort(int i)	{

	if(xuids[i].l != -1)
		mksort(xuids[i].l);

	printf("pos %2d, el %2d (xuid=%lld)\n", p, i, xuids[i].xuid);
	sorted[p++] = i;

	if(xuids[i].r != -1)
		mksort(xuids[i].r);

}

// create rebalanced tree from sorted array
static int merge(int lev)	{

	int l,m,r;		// left, middle, right
	if(lev == 0)	{

		if(p < N)	{
			l = sorted[p++];
			if(p < N)	{
				m = sorted[p++];
				xuids[m].l = l;
				if(p < N)	{
					r = sorted[p++];
					xuids[m].r = r;
				}
			} else
				return l;
		}

	}

	l = m;
	if(p < N)	{
		m = sorted[p++];
		xuids[m].l = l;
		if(p < N)	{
			r = merge(lev + 1);
			xuids[m].r = r;
		}
	} else
		return l;

	return m;

}

void rebalance()	{

	sorted = calloc(N+NINCREMENT, sizeof(int));

	printf("Sorting\n");
	p = 0;
	mksort(tree);
	exit(0);

	printf("Merging\n");
	p = 0;
	tree = merge(tree);

	free(sorted);

	N += NINCREMENT;

}

