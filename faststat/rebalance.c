#include <stdlib.h>
#include <libpq-fe.h>

#include "stater.h"

int p;
int *sorted;

// extract to sorted from xuids tree
static void mksort(int i)	{

	if(xuids[i].l != -1)
		mksort(xuids[i].l);

	sorted[p++] = i;

	if(xuids[i].r != -1)
		mksort(xuids[i].r);

}

int bits(int i) {

	int b = 1;
	while(i /= 2)
		b++;
	return b;

}


// create rebalanced tree from sorted array
static int merge()	{

	int i, st, l, r;

	for(i=0; i < fp; i += 2)
		xuids[sorted[i]].l = xuids[sorted[i]].r = -1;

	for(st = 4; st/2 <= fp; st *= 2) {
		// printf("step=%d\n", st);
		for(i = st/2 - 1; i < fp; i += st)   {
			
			l = i - st/4;
			r = i + st/4;

			if(r >= fp)  {

				int d = (fp - st/2) % (st/4);
				r = (d) ? i + bits(d) : -1;

			}

			// printf("    (%2d)<->(%2d) ----> (%2d)     (%2d)<->(%2d) ----> (%2d)\n", l, r, i, sorted[l], sorted[r], sorted[i]);

			xuids[sorted[i]].l = sorted[l];
			xuids[sorted[i]].r = (r == -1) ? -1 : sorted[r];

		}

	}
	return sorted[st/4 - 1];

}

void rebalance()	{

	sorted = calloc(N*2+2, sizeof(int));

	p = 0;
	mksort(tree);
	printf("Before: ");
	dump_tree();

	tree = merge();
	printf("    N=%d, tree=%d\n", N, tree);
	printf("After:  ");
	dump_tree();
// 	exit(0);

	free(sorted);

	N = N * 2 + 1;

	xuids = reallocarray(xuids, N, sizeof(struct XUID));
	if(xuids == NULL)	{

		printf("Out of memory\n");
		exit(1);

	}

}

