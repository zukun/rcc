#include <stdio.h>
#include "GraphMatching.h"


GraphMatching* LoadMatchingFile(char* filename, bool load_neib = true)
{
	FILE* fp;
	int N[2], A, E, _A = 0, _E = 0;
	char LINE[256];
	double cost;
	GraphMatching* m = NULL;

	fp = fopen(filename, "r");
	if (!fp) { printf("Can't open %s\n", filename); exit(1); }

	while (fgets(LINE, sizeof(LINE)-1, fp))
	{
		if (LINE[0] == 'p')
		{
			if (m || sscanf(&LINE[1], "%d %d %d %d\n", &N[0], &N[1], &A, &E) != 4) { printf("%s: wrong format1!\n", filename); exit(1); }
			m = new GraphMatching(N[0], N[1], A, E);
		}
		else if (LINE[0] == 'a')
		{
			int a, i0, i1;
			if (!m || sscanf(&LINE[1], "%d %d %d %Lf\n", &a, &i0, &i1, &cost) != 4 
			    || a!=_A++ || _A>A || i0<0 || i0>=N[0] || i1<0 || i1>=N[1]) { printf("%s: wrong format2!\n", filename); exit(1); }
			m->AddAssignment(i0, i1, cost);
		}
		else if (LINE[0] == 'e')
		{
			int a, b;
			if (!m || sscanf(&LINE[1], "%d %d %Lf\n", &a, &b, &cost) != 3
			    || (_E++)>=E || a<0 || a>=A || b<0 || b>=A || a==b) { printf("%s: wrong format3!\n", filename); exit(1); }
			m->AddEdge(a, b, cost);
		}
		else if (LINE[0] == 'n')
		{
			if (load_neib)
			{
				int r, i, j;
				if (!m || sscanf(&LINE[1], "%d %d %d\n", &r, &i, &j) != 3 
					|| r<0 || r>1 || i<0 || i>=N[r] || j<0 || j>=N[r] || i==j) { printf("%s: wrong format4!\n", filename); exit(1); }
				m->AddNeighbors(r, i, j);
			}
		}
	}
	fclose(fp);

	if (!m || _A!=A || _E!=E) { printf("%s: wrong format5!\n", filename); exit(1); }

	/////////////////////////////////////////////////////////////////////////

	printf("problem from %s loaded (N0=%d, N1=%d, A=%d, E=%d\n", filename, N[0], N[1], A, E);

	return m;
}

void main()
{
	GraphMatching* m = LoadMatchingFile("DATA.TXT");

	// you may need to experiment a bit with what subproblems to add. 

	//m->AddLinearSubproblem();
	//m->AddMaxflowSubproblem();
	m->AddLocalSubproblems(3);
	//m->AddTreeSubproblems();

	m->SolveDD(10000, 1e-5);

	delete m;
}
