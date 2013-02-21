/*
Copyright 2008 
	Victor Lempitsky (victorlempitsky@gmail.com), 
	Yuri Boykov (yuri@csd.uwo.ca), 
	Vladimir Kolmogorov (vnk@adastral.ucl.ac.uk),
	The University of Western Ontario

 This software implements the touch-expand algorithm and demonstrates how it can be applied for shape-from-oriented points reconstruction.
The algorithm and its application were introduced in:

		Victor Lempitsky and Yuri Boykov.
		Global Optimization for Shape Fitting.
		In proceedings of IEEE COnference on Computer Vision and Pattern Recognition (CVPR),
		2007

This software also contains mincut/maxflow code by Vladimir Kolmogorov and Yuri Boykov (version 2.2), which follows
		
		An Experimental Comparison of Min-Cut/Max-Flow Algorithms
		for Energy Minimization in Vision.
		Yuri Boykov and Vladimir Kolmogorov.
		In IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI), 
		September 2004

If you use this software in your research, you should cite
the two aforementioned papers in any resulting publication.

For the licence, please see the README.TXT
The maxflow part of this software is covered by a special copyright and a special license described in the MAXFLOW\README.txt.
*/




#include "ShapeFitting.h"
#include <time.h>
#include <conio.h>


void ShapeFitting::FitShape(const char *bbox_path, const char *param_path, const char *points_path, const char *result_path)
{
	FILE *echo = stdout;

	unsigned char *result;

	//reading bounding box
	FILE *inbbox = fopen(bbox_path, "r");
	if(fscanf(inbbox, "%f %f %f %f %f %f", bbox_min, bbox_min+1, bbox_min+2,
		bbox_max, bbox_max+1, bbox_max+2) != 6)
			throw "Cannot read bbox!";
	fclose(inbbox);

	//reading algorithm parameters
	ReadParams(param_path);

	//computing the voxel size
	step = (bbox_max[0]-bbox_min[0])/RESOLUTION;
	
	int j;

	//padding with additional layers to enforce topology
	for(j = 0; j < 3; j++)
	{
		if(TOPOLOGY == CLOSED || TOPOLOGY == TABLE)
		{
			if(TOPOLOGY != TABLE || j != 1)
				bbox_min[j] -= step;
			bbox_max[j] += step;
		}
		dims[j] = int((bbox_max[j]-bbox_min[j])/step);
	}

	fprintf(echo, "Grid dimensions: %d %d %d\n", dims[0], dims[1], dims[2]);

	fputs("Rasterizing oriented points...\n",echo);
	RasterizePoints(points_path);

	//////////////////EXAMPLE OF TOUCH-EXPAND USE/////////////////////////////////////////
	//using the touch-expand algorithm to compute the minimal cut
	TouchExpand *touchExpand = new TouchExpand(dims, NEIGHB_SYSTEM, DENSITY_AMBIENT, tlinks, DENSITY_STRENGTH? nlinks : NULL);

	fputs("Performing coarse-to-fine initialization...\n",echo);
	clock_t startMaxflow = clock();
	touchExpand->InitializeCoarseToFine(2, true);

	fputs("----- Starting final maxflow ------\n",echo);
	touchExpand->MaxFlow(1, echo);
	fputs("Maxflow computed.\n",echo);
	fprintf(echo, "Time elapsed for minimization %lf sec.\n", double(clock()-startMaxflow)/CLOCKS_PER_SEC);
	
   	result = new unsigned char[dims[0]*dims[1]*dims[2]];
	touchExpand->GetResult(result);
	delete touchExpand;
	///////////////////////////////////////////////////////////////////////////////////////

	//saving result
	SaveResult(result, result_path); 

	//cleaning up
	delete result;
	if(tlinks)	delete tlinks;
	if(nlinks) delete nlinks; 
	tlinks = NULL;
	nlinks = NULL;
}


//////////////////////////////////////////////////////////////////////////////////

void ShapeFitting::SaveResult(unsigned char *result, const char *result_path)
{
	FILE *out = fopen(result_path, "wb");
	fwrite(dims, sizeof(int), 3, out); //writing dimensions of the grid (3 x int32)
	fwrite(result, sizeof(unsigned char), dims[0]*dims[1]*dims[2], out); //writing the cut (nx*ny*nz x uint8)
	fwrite(bbox_min, sizeof(float), 3, out); //writing the topleftfront corner of bbox (3 x float32)
	fwrite(&step, sizeof(float), 1, out); //writing voxel size (1 x float32)
	fclose(out);
}
//////////////////////////////////////////////////////////////////////////////


int main()
{
	try{
		ShapeFitting sf;
		sf.FitShape("Stanford_Bunny\\bbox.dat", "Stanford_Bunny\\params.ini", "Stanford_Bunny\\points.dat", "Stanford_Bunny\\result.dat");
	}
	catch(const char* err)
	{
		fputs(err, stderr);
		getch();
		exit(1);
	}
	return 0;
}
