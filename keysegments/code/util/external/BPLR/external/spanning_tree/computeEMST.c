/***********************************************************************



	File:	computeEMST.c

	A mex file for Matlab (c)

	The calling syntax is:

			

			  [Length, E_MST] = computeEMST(V_in, E_in, lamda)

	

	  V_in  = Dimension -by- Number of vertices		<Point coordinates of the vertices>

	  E_in  = 2 -by- Number of input edges			<Edges (referenced by start & end indices -Matlab index, i.e. starts at 0- of the points) to be included in the EMST>	

	  lamda = Double type scalar					<Exponential weight on Eucliden edges>



	  Length = Double type scalar					<Length of the EMST>

	  E_MST	 = 2  -by- (Number of vertices - 1)(?)	<Edges included in the EMST>



	

	Date:	November 2004



	Copyright (c) 2004 by Mert Rory Sabuncu





************************************************************************/



#include <math.h>

#include <stddef.h>

#include <stdio.h>

#include <stdlib.h>

#include "mex.h"



#define NOT	!

#define AND	&&

#define OR	||

#define EQ	==

#define NE	!=



#define	NEWA(n, type)	((type *) new ((size_t) ((n) * sizeof (type))))



struct edge{

	int		p1;		/* First endpoint of edge. */

    int		p2;		/* Second endpoint of edge. */

	double  len;		/* Length of edge. */

};



struct dsuf{

	int *	parent;

	int *	rank;

	int	    set_size;

};





/*

 * Local Routines

 */



static void	dsuf_create (struct dsuf *, int);

static void	dsuf_destroy (struct dsuf *);

static int	dsuf_find (struct dsuf *, int);

static void	dsuf_makeset (struct dsuf *, int);

static void	dsuf_unite (struct dsuf *, int, int);

static void	fatal (char *);

static int	mst_edge_list (int, int, struct edge *, struct edge *);

static void *	new (size_t);

static void	sort_edge_list (struct edge *, int);





/*

 * The main routine.  Read in graph, compute EMST, output it.

 * Input format: plhs[0]: N-by-1 

 */



void

mexFunction(

			int nlhs, 

			mxArray *plhs[], 

			int nrhs, 

			const mxArray *prhs[])

{



	/*Declarations*/

	const mxArray *		pointData = prhs[0];

	const mxArray *		edgeData = prhs[1];



	const int		dim = mxGetM(pointData);

	const int		npoints = mxGetN(pointData);

	const int		nedges = mxGetN(edgeData);

	const double*	edges = mxGetPr(edgeData);

	



	double *		pointValues = mxGetPr(pointData);

	

	double *		MST_edges;



	int				StartPointIndex, EndPointIndex;

	double			lamda;	

	double	*		pLamda;

	int				i,k;

	struct edge *	EdgeList, * pEdge, * solution, * e;

	double			* pi, * pj;

	double			distance, length, delta;

	double *		pLength;

	

	if (nrhs != 3)  mexErrMsgTxt("Must have three input arguments");



	if (dim == 1) mexErrMsgTxt("Dimension of input points should be larger than 1!");

	

	if (nedges < (npoints - 1))	mexErrMsgTxt("Not enough input edges");



	/*Get lamda - the exponential weight*/



	pLamda	=	mxGetPr(prhs[2]);

	lamda	=	pLamda[0];



	if (lamda == 0) mexErrMsgTxt("The exponential weight should be nonzero");



	EdgeList = NEWA(nedges, struct edge);

	pEdge = EdgeList;



	for (i = 0; i < nedges; i++){



		StartPointIndex = (int) edges[2*i] - 1;

		EndPointIndex = (int) edges[2*i + 1] - 1;



		pEdge->p1 = StartPointIndex;

		pEdge->p2 = EndPointIndex;



		pi = &pointValues[StartPointIndex * dim];

		pj = &pointValues[EndPointIndex * dim];



		distance = 0.0;

		

		for (k = 0; k < dim; k++){

			delta = pi[k] - pj[k];
			
			distance += (delta * delta);

		}

		pEdge->len = distance;

		pEdge++;

	}



	solution = NEWA (npoints - 1, struct edge);

	k = mst_edge_list (npoints, nedges, EdgeList, solution);

	e = solution;



	

	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);

	plhs[1] = mxCreateDoubleMatrix(2, (npoints - 1), mxREAL);

	

	MST_edges	=  mxGetPr(plhs[1]);



	length = 0.0;

	for (i = 0; i < k; i++) {

		length += pow(e -> len,lamda/2.0);

		MST_edges[i*2]	 =	(double) e->p1 + 1;

		MST_edges[i*2+1] =	(double) e->p2 + 1;

		e++;

	};

	pLength = mxGetPr(plhs[0]);

	pLength[0] = length;

	free((struct edge * )EdgeList);

	free((struct edge * )solution);

	return;

}



/*

 * This routine computes the MST of a given list of edges.

 */



static int mst_edge_list

(int                n,	        /* IN - number of vertices */

 int			    nedges,		/* IN - number of edges */

 struct edge *		edge_list,	/* IN - list of edges */

 struct edge *		edges		/* OUT - MST edge list */

)

{

int		i;

int		mst_edge_count;

int		components;

int		max_vert;

struct edge *	ep;

struct edge *pp;

struct edge *	ep_endp;

int		root1;

int		root2;

struct dsuf	sets;

int 		ctr =0;

double		perc;



	sort_edge_list (edge_list, nedges);	

    max_vert = 1;		/* avoid zero-size union-find... */

	ep = edge_list;

	for (i = 0; i < nedges; i++, ep++) {

		if (ep -> p1 > max_vert) {

			max_vert = ep -> p1;

		}

		if (ep -> p2 > max_vert) {

			max_vert = ep -> p2;

		}



	}



	dsuf_create (&sets, max_vert + 1);



	/* Note that it is not a problem to "makeset" a vertex more	*/

	/* than once...							*/

	ep = edge_list;

	for (i = 0; i < nedges; i++, ep++) {

		dsuf_makeset (&sets, ep -> p1);

		dsuf_makeset (&sets, ep -> p2);

	}



	components = n;

	mst_edge_count = 0;

	ep = edge_list;

	ep_endp = (ep + nedges);



	while (components > 1) {

		ctr++;

		if (ep >= ep_endp) {

			/* Ran out of edges before MST complete! */

			printf("Ran our of edges before tree complete!");

			/*printf("\nHowever, I have managed to get you almost all edges");*/

			perc=(mst_edge_count*100)/(n-1);

			printf("\nthe number of edges is %d which is %g percent of all edges \n",mst_edge_count,perc);

			if (mst_edge_count > (n-(0.1*n)) )

				{

                                 pp=edge_list+ctr-1;

				 /*printf("\nctr is %d and msx dist is %g\n",ctr,pp->len);

                 */

								 dsuf_destroy (&sets);

			         return (mst_edge_count);

				}

			else{fatal ("mst_edge_list: Bug 1 Ran out of edges before tree complete Less than 90percent of all edges gleaned.");}

		}

		root1 = dsuf_find (&sets, ep -> p1);

		root2 = dsuf_find (&sets, ep -> p2);

		if (root1 NE root2) {

			dsuf_unite (&sets, root1, root2);

			*edges = *ep;

			++edges;

			++mst_edge_count;

			--components;

		};

		++ep;

	}

	pp=edge_list+ctr;


	dsuf_destroy (&sets);



	return (mst_edge_count);

}



 

	static

	void

sort_edge_list (



struct edge *		a,	/* IN/OUT - array of edges to be sorted. */

int			n	/* IN - number of elements in array. */

)

{

int		h;

struct edge	tmp;

double		keyLength;

struct edge *	p1;

struct edge *	p2;

struct edge *	p3;

struct edge *	p4;

struct edge *	endp;



	endp = &a [n];



	for (h = 1; h <= n; h = 3*h+1) {

	}



	do {

		h = h / 3;

		p4 = &a [h];

		p1 = p4;

		while (p1 < endp) {

			tmp = *p1;

			keyLength = tmp.len;

			p2 = p1;

			for (;;) {

				p3 = (p2 - h);

				if (p3 -> len < keyLength){

					break;

				}

				*p2 = *p3;

				p2 = p3;

				if (p2 < p4) break;

			}

			*p2 = tmp;

			++p1;

		}

	} while (h > 1);

}



/*

 * This routine creates a collection of N disjoint sets.  They are left

 * uninitialized so that a sparse collection can be accessed quickly.

 */



	static

	void

dsuf_create (



struct dsuf *	dsp,		/* IN/OUT - sets to create */

int		n		/* IN - number of disjoint sets */

)

{

	if (n <= 0) {

		fatal ("dsuf_create: Bug 1.");

	}



	dsp -> set_size		= n;

	dsp -> parent		= NEWA (n, int);

	dsp -> rank		= NEWA (n, int);

}





/*

 * Destroy the given collection of disjoint sets.

 */



	static

	void

dsuf_destroy (



struct dsuf *	dsp		/* IN - sets to destroy */

)

{

	free ((char *) (dsp -> rank));

	free ((char *) (dsp -> parent));



	dsp -> set_size	= 0;

	dsp -> parent	= NULL;

	dsp -> rank	= NULL;

}



/*

 * This routine makes a single disjoint set for item "i".

 */



	static

	void

dsuf_makeset (



struct dsuf *	dsp,		/* IN - collection of sets */

int		i		/* IN - item to make into a disjoint set */

)

{

	if ((i < 0) OR (i >= dsp -> set_size)) {

		/* Item out of bounds. */

		fatal ("dsuf_makeset: Bug 1.");

	}

	dsp -> parent [i]	= i;

	dsp -> rank [i]		= 0;

}



/*

 * This routine "unites" two sets that were previously disjoint.  I and J

 * must be the "canonical" member of each disjoint set (i.e. they must

 * each be the output of a "find" operation), and must be distinct.

 *

 * We perform the "union by rank" heuristic here.

 */



	static

	void

dsuf_unite (



struct dsuf *	dsp,		/* IN - collection of sets */

int		i,		/* IN - first set to unite */

int		j		/* IN - second set to unite */

)

{

int		ri;

int		rj;



	if ((i < 0) OR (i >= dsp -> set_size)) {

		/* Item I is out of range. */

		fatal ("dsuf_unite: Bug 1.");

	}

	if ((j < 0) OR (j >= dsp -> set_size)) {

		/* Item J is out of range. */

		fatal ("dsuf_unite: Bug 2.");

	}

	if (i EQ j) {

		/* Attempt to unite I with I. */

		fatal ("dsuf_unite: Bug 3.");

	}



	ri = dsp -> rank [i];

	rj = dsp -> rank [j];



	if (ri EQ rj) {

		/* Both subtrees have the same maximum depth.  We	*/

		/* arbitrarily choose I to be underneath J.  The rank	*/

		/* of J must then increase.				*/

		dsp -> parent [i] = j;

		dsp -> rank [j]   = rj + 1;

	}

	else if (ri > rj) {

		/* Tree I is (probably) deeper.  Putting J underneath	*/

		/* will not increase I's rank.				*/

		dsp -> parent [j] = i;

	}

	else {

		/* Tree J is (probably) deeper... */

		dsp -> parent [i] = j;

	}

}

/*

 * This routine, given a member I of one of the disjoint sets A, will

 * choose a cannonical member J of set A and return it.  Until set A gets

 * united with some other set, find (I) will always return the same J.

 *

 * This routine performs the "path compression" heuristic.

 */



static	int

dsuf_find (



struct dsuf *	dsp,		/* IN - collection of sets */

int		i		/* IN - item to find cannonical item for */

)

{

int		j;

int		k;



	/* Yes, I know this routine is very elegent when coded	*/

	/* recursively...  Here's the iterative version.	*/



	j = dsp -> parent [i];

	if (i EQ j) {

		/* A cannonical element has itself as parent. */

		return (i);

	}



	/* We must search up the tree -- and compress when done... */

	for (;;) {

		k = dsp -> parent [j];

		if (j EQ k) break;

		j = k;

	}



	/* Now compress the path (make all items in chain point directly */

	/* at the root K) -- we never have to do this search again!	 */

	while (i NE k) {

		j = dsp -> parent [i];

		dsp -> parent [i] = k;

		i = j;

	}



	return (k);

}



/*

 * This routine displays a fatal message and then dies!

 */



	static

	void

fatal (



char *		msg		/* IN - message to display. */

)

{

	(void) fprintf (stderr, "%s\n", msg);

	(void) fflush (stderr);

	abort ();

}

/*

 * This routine performs all dynamic memory allocation for the program.

 * We test for out of memory condition here.

 */



	static

	void *

new (



size_t		size		/* IN - size of chunk in bytes. */

)

{

void *		p;



	if (size EQ 0) {

		/* Avoid implementation-defined bahavior of malloc! */

		size = 1;

	}



	p = malloc (size);

	if (p EQ NULL) {

		(void) fprintf (stderr, "Out of memory!\n");

		exit (1);

	}



	return (p);

}

