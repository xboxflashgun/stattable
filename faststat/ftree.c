#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "stater.h"

static void rebalance(ftree *f);

static void subdump(ftree *f, int p)	{

	ftree_el *el = (ftree_el *) (f->a + p * f->so);
	if( el->l != -1 )
		subdump(f, el->l);
	if( el->r != -1 )
		subdump(f, el->r);

}

void ftree_dump(ftree *f)	{

	subdump(f, f->root);

}

ftree *ftree_init(int N, int so, int (*comp)())	{

	ftree *f = calloc(1, sizeof(ftree));
	f->N = N;
	f->so = so;
	f->comp = comp;
	f->root = -1;			// no elements yet
	f->a = calloc(N, so);
	return f;

}

int ftree_upsert(ftree *f, ftree_el *el)	{

	if(f->root == -1)	{

		f->root = 0;			// this is the first element
		f->fp++;
		memcpy(f->a, el, f->so);
		f->maxdepth = 1;

		el->l = el->r = -1;		// no children yet
		
		return 0;

	}

	int p, np;
	for(p = np = f->root; p != -1; )	{
		if( f->comp(ftree_get(f, p),el) == 0)
			break;
		np = p;
		if( f->comp(el, ftree_get(f, p)) > 0)
			p = ftree_get(f, p)->r;
		else
			p = ftree_get(f, p)->l;
	}

	if(p == -1)		{

		p = f->fp++;						// next element in tree
		ftree_el *pp = ftree_get(f,p);		// this element
		ftree_el *npp = ftree_get(f, np);	// link to parent el

		memcpy(pp, el, f->so);			// copy new node to array
		pp->l = pp->r = -1;					// no children in new node

		if( f->comp(el, npp) > 0)
			npp->r = p;
		else
			npp->l = p;

	}

	if(f->fp == f->N)	{

		rebalance(f);
	
	}

	return p;

}

static int *sorted;
static int sp;			// sorted pointer

// extract to sorted from xuids tree
static void mksort(ftree *f, int i)	{

	if(ftree_get(f, i)->l != -1)
		mksort(f, ftree_get(f, i)->l);

	sorted[sp++] = i;

	if(ftree_get(f, i)->r != -1)
		mksort(f, ftree_get(f, i)->r);

}


// create rebalanced tree from sorted array
static int merge(ftree *f)	{

	int i, st, l, r;

	for(i=0; i < f->fp; i += 2)
		ftree_get(f, i)->l = ftree_get(f, i)->r = -1;

	for(st = 4; st/2 <= f->fp; st *= 2) {
		// printf("step=%d\n", st);
		for(i = st/2 - 1; i < f->fp; i += st)   {
			
			l = i - st/4;
			r = i + st/4;

			// printf("    (%2d)<->(%2d) ----> (%2d)     (%2d)<->(%2d) ----> (%2d)\n", l, r, i, sorted[l], sorted[r], sorted[i]);

			ftree_get(f, sorted[i])->l = sorted[l];
			ftree_get(f, sorted[i])->r = (r == -1) ? -1 : sorted[r];

		}

	}
	return sorted[st/4 - 1];

}

static void rebalance(ftree *f)	{

	sorted = calloc(f->N, sizeof(int));

	sp = 0;
	mksort(f, f->root);

	f->root = merge(f);

	printf("    N=%d, tree=%d\n", f->N, f->root);
	printf("After:  ");
	ftree_dump(f);

	free(sorted);

	f->N = f->N * 2 + 1;

	f->a = reallocarray(f->a, f->N, f->so);
	if(f->a == NULL)	{

		printf("Out of memory\n");
		exit(1);

	}

}

