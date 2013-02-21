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




#ifndef SHAPE_FITTING_H
#define SHAPE_FITTING_H

#include "TouchExpand.h"

//The main class-wrapper for shape-from-oriented points reconstruction

class ShapeFitting {
public:
  ShapeFitting(); //constructor
  ~ShapeFitting(); //destructor

  //main function, see FitShape for the example of usage
  //first three arguments contain paths to the input files. The last one should contain the path for the output.
  //Please, see readme.txt in the Stanford_Bunny folder distributed with this code for the description of the file formats
  void FitShape(const char *bbox_path, const char *param_path, const char *points_path, const char *result_path);
  
private:
  typedef enum {FREE, CLOSED, TABLE} TOPOLOGY_TYPE;
  
  int RESOLUTION;
  float FLUX_STRENGTH;
  int FLUX_DIFFUSE_RAD;
  int FLUX_DIFFUSE_ITER;
  float DENSITY_AMBIENT;
  float DENSITY_STRENGTH;
  int DENSITY_DIFFUSE_RAD;
  int DENSITY_DIFFUSE_ITER;
  TOPOLOGY_TYPE TOPOLOGY;
  NEIGHBORHOOD NEIGHB_SYSTEM;

  short *tlinks;
  short *nlinks;

  int dims[3];

  float bbox_min[3];
  float bbox_max[3];
  float step;

  void RasterizeSample(float *array, float p[], float w);
  void RasterizePoints(const char *points_path);

  template<class A, class B> void BoxFilter(A *array, int rad, bool rescale);
  void ReadParams(const char *param_path);

  void SaveResult(unsigned char *result, const char *result_path);
};

//rasterizes a point with coordinates p and weight w onto array 
inline void ShapeFitting::RasterizeSample(float *array, float p[], float w)
{
	float P[3];
	float d[3];
	int pt[3];

	int j, adr;

	for(j = 0; j < 3; j++)
	{
		P[j] = (p[j]-bbox_min[j])/step;
		pt[j] = int(P[j]);
		d[j] = P[j]-pt[j];
		if(pt[j] < 0 || pt[j] >= dims[j]-1)
			return;
	}

	adr = pt[2]*dims[1]*dims[0]+pt[1]*dims[0]+pt[0];
	array[adr] += w*(1.0f-d[0])*(1.0f-d[1])*(1.0f-d[2]);
	array[adr+1] += w*(d[0])*(1.0f-d[1])*(1.0f-d[2]);
	array[adr+dims[0]] += w*(1.0f-d[0])*(d[1])*(1.0f-d[2]);
	array[adr+dims[0]*dims[1]] += w*(1.0f-d[0])*(1.0f-d[1])*(d[2]);
	array[adr+1+dims[0]] += w*(d[0])*(d[1])*(1.0f-d[2]);
	array[adr+1+dims[0]*dims[1]] += w*(d[0])*(1.0f-d[1])*(d[2]);
	array[adr+dims[0]+dims[0]*dims[1]] += w*(1.0f-d[0])*(d[1])*(d[2]);
	array[adr+1+dims[0]+dims[0]*dims[1]] += w*(d[0])*(d[1])*(d[2]);
}



#endif