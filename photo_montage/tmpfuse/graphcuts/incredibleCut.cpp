#include <math.h>
#include <algorithm>
#include <float.h>
#include "incredibleCut.h"
// usually 220
#define HIGHLIGHT_THRES 100
#define CUTOFF_ZONE .35f
// usually .15

IncredibleCut::IncredibleCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result) :
  PortraitCut(oWidth, oHeight, images, result, stdout) {
  setCuttype(C_GRAD2); 
  //memset(_labels, 0, _w*_h*sizeof(ushort));
  //memset(_required, (short)-1, _w*_h*sizeof(short));
  //allocateComputeAllCGrads();
//  allocateComputeAllGradMags(); 

  setupTolerances();

  /*
  GrayImage gray = (GrayImage) imNew(IMAGE_GRAY, _w, _h);
  char name[100];
  for (uint i=0; i<_images.size(); ++i) {
    for (int y=0; y<_h; ++y)
      for (int x=0; x<_w; ++x) {
	imRef(gray, x, y) = (unsigned char) (3.f * BVZ_data_penalty(Coord(x,y), i));
      }

    sprintf(name, "cont%.3d.pgm", i);
    imSave(gray,name);
    
  }
  imFree(gray);
  std::exit(0);*/
  
}

IncredibleCut::~IncredibleCut() {
  delete[] _low; delete[] _high;
}

void IncredibleCut::setupTolerances() {

  _low = new float[_w*_h];
  _high = new float[_w*_h];

/*  
  int lown =  int(CUTOFF_ZONE*float(_n));
  int highn = int(ceil((1.f-CUTOFF_ZONE) * float(_n)));
  printf("low high %d %d\n",lown, highn);
  assert(lown>0 && lown<_n);
  assert(highn>0 && highn<_n);
  assert(lown < highn);

  int i,j,l,index;
  float* lum = new float[_n];
  for (j=0,index=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {
      
      Coord p(i,j);
      for (l=0; l<_n; ++l) { 
	unsigned char* I = _imptr(l,p);
	lum[l] = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];
      }

      nth_element(lum, lum+lown, lum+_n);
      _low[index] =  lum[lown];
      nth_element(lum, lum+highn, lum+_n);
      _high[index] = lum[highn];
    }
  
  delete[] lum;
  
*/

  int i,j,l,index;
  float max, min, lum, off;
  for (j=0,index=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {
      
      Coord p(i,j);
      max = FLT_MIN, min = FLT_MAX;
      for (l=0; l<_n; ++l) { 
	unsigned char* I = _imptr(l,p);
	lum = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];
	if (lum>max)
	  max = lum;
	if (lum<min)
	  min = lum;
      }
      off = CUTOFF_ZONE * (max-min);
      _low[index] =  min + off;
      _high[index] = max - off; 
      assert(_low[index] < _high[index]);
    }     
  
}

#define I_PENALTY 1.5f
float IncredibleCut::BVZ_data_penalty(Coord p, ushort d) {
  int index = p.y*_w + p.x;
  unsigned char* I = _imptr(d,p);
  float lum = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];
  if (lum <= _low[index]) return I_PENALTY;
  if (lum >= HIGHLIGHT_THRES && lum >= _high[index]) return I_PENALTY;
  return 0;
}



float IncredibleCut::BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl) {
  if (l==nl) return 0;

  return (.1f +  1.f * (PortraitCut::BVZ_interaction_penalty(p,np,l,nl))); 
}
