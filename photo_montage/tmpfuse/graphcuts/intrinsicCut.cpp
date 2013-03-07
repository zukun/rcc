#include "intrinsicCut.h"
#include <algorithm>

IntrinsicCut::IntrinsicCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result) :
  PortraitCut(oWidth, oHeight, images, result, stdout) {
  setCuttype(C_GRAD); 
  //memset(_labels, 0, _w*_h*sizeof(ushort));
  //memset(_required, (short)-1, _w*_h*sizeof(short));
  //allocateComputeAllGradMags();
  //allocateComputeAllCGrads();
  computeMedians();
}

IntrinsicCut::~IntrinsicCut() {
  delete[] _medians;
  delete[] _medianColors;
}


void IntrinsicCut::computeMedians() {
  int med = _n/2;

  _medians = new CGrad[_w*_h];
  double* workspace = new double[_n];

  _medianColors = new Vec3i[_w*_h];
  int* cworkspace = new int[_n];

  int c,i,j,l, index;
  for (c=0; c<6; ++c) {

      for (j=0,index=0; j<_h; ++j)
	for (i=0; i<_w; ++i, ++index) {

	  for (l=0; l<_n; ++l) { 
	    workspace[l] = _cgrads[l][index].val(c);
	  } // end iterate over images

	  nth_element(workspace, workspace+med, workspace+_n);
	  _medians[index].setVal(c,workspace[med]);

	} // end iterate over pixels
  } // iterate over channels, directions

  for (c=0; c<3; ++c) {
    for (j=0,index=0; j<_h; ++j)
      for (i=0; i<_w; ++i, ++index) {
	
	for (l=0; l<_n; ++l) {
	  Coord p(i,j);
	  const unsigned char* col = _imptr(l, p);
	  cworkspace[l] = col[c];
	}
	nth_element(cworkspace, cworkspace+med, cworkspace+_n);
	_medianColors[index].data()[c] = cworkspace[med];
      }
  }

  printf("Finished computing medians\n");
  delete[] workspace;
  delete[] cworkspace;
}


float IntrinsicCut::BVZ_data_penalty(Coord p, ushort d) {
  //return 0;
  
  int index = p.y*_w + p.x;
  //return 0.005f * (float) (_medians[index].normFrom(_cgrads[d][index]));
  
  const unsigned char* col = _imptr(d,p);
  Vec3i c(col[0], col[1], col[2]);
  
  float cost =  0.003f * (float) sqrt(_medianColors[index].distanceTo2(c));
  //fprintf(stdout, "cost is %lf\n", cost);
  return cost;
  /*
  //return  (.001f * float(sqrt(_medianColors[index].distanceTo2(c))) +
  //   .005f * float(_medians[index].normFrom(_cgrads[d][index])));
  */
}


float IntrinsicCut::BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl) {
  //return 0; 
  if (l==nl) return 0;

  return 0.1 + 1.f*PortraitCut::BVZ_interaction_penalty(p,np,l,nl);
  //return 0.1 + PortraitCut::BVZ_interaction_penalty(p,np,l,nl);
}
