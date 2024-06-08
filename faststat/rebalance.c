#include <stdlib.h>
#include <libpq-fe.h>

#include "stater.h"

int p;
int *sorted;

// extract to sorted from xuids tree
static void mksort(int i)	{

	if(xuids[i].l)
		mksort(xuids[i].l);

	sorted[p++] = i;

	if(xuids[i].r)
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

	sorted = calloc(N, sizeof(int));

	p = 0;
	mksort(0);

	p = 0;
	merge(0);

	free(sorted);

}

