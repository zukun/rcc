#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "hb.h"

#define pow2(n) ( 1 << (n))
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#define WCLAMP(a) (MAX(MIN(a,_w-1),0))
#define HCLAMP(a) (MAX(MIN(a,_h-1),0))
//#define I(a,b) (HCLAMP(b)*_w + WCLAMP(a))
#define I(a,b) ((b)*_w + (a))

int numbits(int a) {
  int res = 0;
  if (a==0) return 0;
  do {
    a >>= 1; ++res;
  } while (a!=0);
  return res;
}


HB_Precond::HB_Precond(const int w, const int h) :
  _w(w), _h(h) {

  _xlevels = numbits(_w); _ylevels = numbits(_h); 
  _maxLevels = MIN(_xlevels, _ylevels);
  _maxLevels = MIN(5,_maxLevels);
  //printf("xelvels -3 = %d\n",_xlevels);
}


void HB_Precond::sweepUp(Areal* r) const {
  //int maxLevels = MIN(_xlevels, _ylevels);

  int l;
  int start, stride, x, y;
  Areal me;
  for (l=0; l<_maxLevels-1; ++l) {
    start = pow2(l);
    stride = start << 1;


    for (y=start; y<=_h-1; y+=stride) {
      for (x=start; x<=_w-1; x+=stride) {
	if (x+start >=_w || y+start >=_h)
	  continue;
	if (!(x==_w-1 && y==_h-1)) {
	  me = .25 * r[I(x,y)];
	  r[I(x,y-start)] += me;
	  r[I(x,y+start)] += me;
	  r[I(x-start,y)] += me;
	  r[I(x+start,y)] += me;
	}
      } // y
    } // x

    
    for (y=start; y<=_h-1; y+=stride) {
      for (x=start; x<=_w-1; x+=stride) {
	//printf("%d %d\n",x,y);
	if (x+start >=_w || y+start >=_h)
	  continue;
	if (x==start && y!=_h-1){
	  //debugGrid[I(0,y)] += 20+l;
	  me = .5 * r[I(0,y)];
	  r[I(0,y-start)] += me;
	  r[I(0,y+start)] += me;
	}
	if (y==start && x!=_w-1) {
	  //debugGrid[I(x,0)] += 20+l;
	  me = .5 * r[I(x,0)];
	  r[I(x-start,0)] += me;
	  r[I(x+start,0)] += me;
	}
	if (x!=_w-1 && y!=_h-1) {
	  me = .5 * r[I(x+start,y)];
	  r[I(x+start, y-start)] += me;
	  r[I(x+start, y+start)] += me;
	  me = .5 * r[I(x,y+start)];
	  r[I(x-start, y+start)] += me;
	  r[I(x+start, y+start)] += me;
	  //debugGrid[I(x+start,y)] += 20+l;
	  //debugGrid[I(x,y+start)] += 20+l;
	}
      } // y
    } // x
    

  } // levels

}


void HB_Precond::sweepDown(Areal* r) const {
  //int maxLevels = MIN(_xlevels, _ylevels);

  int l;
  int start, stride, x, y;
  for (l=_maxLevels-2; l>=0; --l) {
    start = pow2(l);
    stride = start << 1;

    for (y=start; y<=_h-1; y+=stride) {
      for (x=start; x<=_w-1; x+=stride) {
	if (x+start >=_w || y+start >=_h)
	  continue;
	if (x==start && y!=_h-1){
	  //debugGrid[I(0,y)] += 20+l;
	  r[I(0,y)] += .5*r[I(0,y-start)] + .5*r[I(0,y+start)];
	}
	if (y==start && x!=_w-1) {
	  //debugGrid[I(x,0)] += 20+l;
	  r[I(x,0)] += .5*r[I(x-start,0)] + .5*r[I(x+start,0)];
	}
	if (x!=_w-1 && y!=_h-1) {
	  r[I(x+start,y)] += .5*r[I(x+start, y-start)] + .5*r[I(x+start, y+start)];
	  r[I(x,y+start)] += .5*r[I(x-start, y+start)] + .5*r[I(x+start, y+start)];
	}
      } // y
    } // x
    

    for (y=start; y<=_h-1; y+=stride) {
      for (x=start; x<=_w-1; x+=stride) {
	if (x+start >=_w || y+start >=_h)
	  continue;
	if (!(x==_w-1 && y==_h-1)) {
	  r[I(x,y)] += .25*r[I(x,y-start)] + .25*r[I(x,y+start)] +
	    .25*r[I(x-start,y)] + .25*r[I(x+start,y)];
	}
      } // y
    } // x
    

  } // levels

}


Areal* HB_Precond::extractMatrix(int which) const {
  int totalVar = _w*_h;
  // 0 for S, 1 for S'
  Areal* out = new Areal[totalVar*totalVar];
  Areal* bV = new Areal[totalVar]; // basis vector
  memset(out,0,totalVar*totalVar*sizeof(Areal));
  int i,j,c;
  assert(which==0 || which==1);

  for (i=0; i<totalVar; ++i) {
    memset(bV,0,totalVar*sizeof(Areal));
    bV[i]=1.;
    if (which==0) 
      sweepDown(bV);
    else
      sweepUp(bV);
    
    for (j=0,c=0; j<totalVar; ++j, c+=totalVar)
      out[c+i] = bV[j];
  }


  delete[] bV;
  return out;
}

void HB_Precond::printMatrix(Areal* m, FILE* fp) const {
  int i,j,index=0;
  int totalVar = _w*_h;
  for (j=0; j<totalVar; ++j) {
    for (i=0;i<totalVar; ++i,++index) {
      fprintf(fp,"%.2f ",m[index]);
    }
    fprintf(fp,"\n");
  }
}
