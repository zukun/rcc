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
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <time.h>

int densityCounter;


//////////////////////////////////////////////////////////////////////////////
ShapeFitting::ShapeFitting()
{
	tlinks = NULL;
	nlinks = NULL;
}

///////////////////////////////////////////////////////////////////////////////
ShapeFitting::~ShapeFitting()
{
	if(tlinks) delete tlinks;
	if(nlinks) delete nlinks;
}

////////////////////////////////////////////////////////////////////////////////

inline short SHORT(float x)
{
	if(x < -32767.0f) return -32767;
	if(x > 32767.0f) return 32767;
	return short(x);
}

void ShapeFitting::RasterizePoints(const char *points_path)
{
	int i, j;

	if(tlinks) delete tlinks;
	if(nlinks) delete nlinks;

	tlinks = NULL;
	nlinks = NULL;

	//starting with t-links

	int pos[3];
	i=0;

    tlinks = new short[dims[0]*dims[1]*dims[2]];
	memset(tlinks, 0, sizeof(short)*dims[0]*dims[1]*dims[2]);

	//next counting the number of points in the bounding box
	float nPoints = 0;

	FILE *in = fopen(points_path, "rb");
	if(!in) throw "Cannot open points file!";

	const int PORTION = 1000000;
	float *piece = new float[7*PORTION];
	int nread;

	while(nread = (int)fread(piece, sizeof(float), 7*PORTION, in ))
	{
		nread = nread/7;
		bool inside;
		for(i = 0; i < nread; i++)
		{
			inside = true;
			for(j = 0; j < 3; j++)
				if(piece[7*i+j] < bbox_min[j] || piece[7*i+j] > bbox_max[j])
					inside = false;

			if(inside)
				nPoints += piece[7*i+6];
		}
	}
	fclose(in);
	delete piece;

	float fluxStrength = FLUX_STRENGTH*(dims[0]*dims[1]+dims[1]*dims[2]+dims[2]*dims[0])*DENSITY_AMBIENT*int(NEIGHB_SYSTEM)/nPoints; //normalized strength of the data term (flux)

	//next processing flux term, one-by-one to save memory
	int offset[] = {1, dims[0], dims[0]*dims[1]};
	float *flux = new float[dims[0]*dims[1]*dims[2]];

	for(j = 0; j < 3; j++)
	{
		densityCounter = 0;
		memset(flux, 0, sizeof(float)*dims[0]*dims[1]*dims[2]);

		FILE *in = fopen(points_path, "rb");
		if(!in) throw "Cannot open in file!";

		const int PORTION = 1000000;
		float *piece = new float[7*PORTION];
		int nread;

		while(nread = (int)fread(piece, sizeof(float), 7*PORTION, in ))
		{
			nread = nread/7;
			for(i = 0; i < nread; i++)
				RasterizeSample(flux, piece+7*i, piece[7*i+3+j]*piece[7*i+6]*fluxStrength);
		}
		fclose(in);
		delete piece;

		for(int k = 0; k < FLUX_DIFFUSE_ITER; k++)
			BoxFilter<float,float>(flux, FLUX_DIFFUSE_RAD,true);

		i=0;
		int offset[] = {1, dims[0], dims[0]*dims[1]};

		for(pos[2] = 0; pos[2] < dims[2]; pos[2]++)
			for(pos[1] = 0; pos[1] < dims[1]; pos[1]++)
				for(pos[0] = 0; pos[0] < dims[0]; pos[0]++,i++)
				{
					if(pos[j] && pos[j] < dims[j]-1)  //adding a divergence term
						tlinks[i] = SHORT(flux[i+offset[j]]-flux[i-offset[j]]+float(tlinks[i]));
				}
	}
	delete flux;

		
	if(DENSITY_STRENGTH)
	{
		densityCounter = 0;
		//processing n-links
		nlinks = new short[dims[0]*dims[1]*dims[2]];

		float *density = new float[dims[0]*dims[1]*dims[2]];
		memset(density, 0, sizeof(float)*dims[0]*dims[1]*dims[2]);

		FILE *in = fopen(points_path, "rb");
		if(!in) throw "Cannot open in file!";

		const int PORTION = 1000000;
		float *piece = new float[7*PORTION];
		int nread;

		while(nread = (int)fread(piece, sizeof(float), 7*PORTION, in ))
		{
			nread = nread/7;
			for(i = 0; i < nread; i++)
				RasterizeSample(density, piece+7*i, piece[7*i+6]*DENSITY_STRENGTH);
		}
		fclose(in);
		delete piece;

		for(int k = 0; k < DENSITY_DIFFUSE_ITER; k++)
			BoxFilter<float, float>(density, DENSITY_DIFFUSE_RAD, true);

		i=0;
		for(pos[2] = 0; pos[2] < dims[2]; pos[2]++)
			for(pos[1] = 0; pos[1] < dims[1]; pos[1]++)
				for(pos[0] = 0; pos[0] < dims[0]; pos[0]++,i++)
					nlinks[i] = SHORT(density[i]+DENSITY_AMBIENT);
		delete density;
	}

	if(TOPOLOGY == FREE)
		return;

	i = 0;
	for(pos[2] = 0; pos[2] < dims[2]; pos[2]++)
		for(pos[1] = 0; pos[1] < dims[1]; pos[1]++)
			for(pos[0] = 0; pos[0] < dims[0]; pos[0]++,i++)
				for(j = 0; j < 3; j++)
					if((pos[j] == 0 && (TOPOLOGY == CLOSED || j != 1))  || pos[j] == dims[j]-1)
						tlinks[i] = -30000;
}


///////////////////////////////////////////////////////////////////////////
#define ADR_(x, y, z) ((x)+(y)*(dims[0]+1)+(z)*(dims[0]+1)*(dims[1]+1))

template<class A, class B> void ShapeFitting::BoxFilter(A *array, int rad, bool rescale)
{
	B *integralImage = new B[(dims[0]+1)*(dims[1]+1)*(dims[2]+1)];
	memset(integralImage, 0, sizeof(B)*(dims[0]+1)*(dims[1]+1)*(dims[2]+1));

	int dx = 1, dy = dims[0]+1, dz = (dims[0]+1)*(dims[1]+1);

	int pos[3];
	for(pos[2] = 1; pos[2] < dims[2]+1; pos[2]++)
		for(pos[1] = 1; pos[1] < dims[1]+1; pos[1]++)
			for(pos[0] = 1; pos[0] < dims[0]+1; pos[0]++)
			{
				int adr = pos[2]*(dims[0]+1)*(dims[1]+1)+pos[1]*(dims[0]+1)+pos[0];
				integralImage[adr] = array[(pos[2]-1)*dims[0]*dims[1]+(pos[1]-1)*dims[0]+pos[0]-1]+
					integralImage[adr-dx]+integralImage[adr-dy]+integralImage[adr-dz]
					-integralImage[adr-dx-dy]-integralImage[adr-dx-dz]-integralImage[adr-dy-dz]+
						integralImage[adr-dx-dy-dz];
			}

	int i = 0;
	int delta = rad;
	
	int minx, maxx, miny, maxy, minz, maxz;
	for(pos[2] = 0; pos[2] < dims[2]; pos[2]++)
		for(pos[1] = 0; pos[1] < dims[1]; pos[1]++)
			for(pos[0] = 0; pos[0] < dims[0]; pos[0]++,i++)
			{
				minx = pos[0]+1-delta-1;
				maxx = pos[0]+1+delta;
				miny = pos[1]+1-delta-1;
				maxy = pos[1]+1+delta;
				minz = pos[2]+1-delta-1;
				maxz = pos[2]+1+delta;

				if(minx < 0) { minx = 0; } 
				if(miny < 0) { miny = 0; }
				if(minz < 0) { minz = 0; }
				if(maxx > dims[0]) { maxx = dims[0]; } 
				if(maxy > dims[1]) { maxy = dims[1]; } 
				if(maxz > dims[2]) { maxz = dims[2]; } 

				B factor = rescale? B(1.0/((maxx-minx)*(maxy-miny)*(maxz-minz)+0.00001)) : B(1.0);
					
				array[i] = A((integralImage[ADR_(maxx,maxy,maxz)]-
					integralImage[ADR_(minx,maxy,maxz)]-
					integralImage[ADR_(maxx,miny,maxz)]-
					integralImage[ADR_(maxx,maxy,minz)]+
					integralImage[ADR_(minx,miny,maxz)]+
					integralImage[ADR_(maxx,miny,minz)]+
					integralImage[ADR_(minx,maxy,minz)]-
					integralImage[ADR_(minx,miny,minz)])*factor);
	
			}
	delete integralImage;
}



//////////////////////////////////////////////////////////
void ShapeFitting::ReadParams(const char *param_path)
{
	FILE *ini = fopen(param_path, "r");
	if(!ini) throw "Cannot read ini file!";

	if(fscanf(ini, "RESOLUTION = %d\n\
					FLUX_STRENGTH = %f\n\
					FLUX_DIFFUSE_RAD = %d\n\
					FLUX_DIFFUSE_ITER = %d\n\
					DENSITY_AMBIENT = %f\n\
					DENSITY_STRENGTH = %f\n\
					DENSITY_DIFFUSE_RAD = %d\n\
					DENSITY_DIFFUSE_ITER = %d\n\
					TOPOLOGY = %d\n\
					NEIGHB_SYSTEM = %d",
					&RESOLUTION, &FLUX_STRENGTH, &FLUX_DIFFUSE_RAD, &FLUX_DIFFUSE_ITER,
					&DENSITY_AMBIENT, &DENSITY_STRENGTH, &DENSITY_DIFFUSE_RAD,
					&DENSITY_DIFFUSE_ITER, &TOPOLOGY, &NEIGHB_SYSTEM ) != 10)
		throw "Cannot parse ini file!";
	fclose(ini);
}


/////////////////////////////////////////////////////////////////




