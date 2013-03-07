#include <math.h>
#include "contrastCut.h"


// 0.0030    0.0133    0.0219    0.0133    0.0030
// 0.0133    0.0596    0.0983    0.0596    0.0133
// 0.0219    0.0983    0.1621    0.0983    0.0219
// 0.0133    0.0596    0.0983    0.0596    0.0133
// 0.0030    0.0133    0.0219    0.0133    0.0030


ContrastCut::ContrastCut(ImageData* id, FilterType ft) : AbstractDataObj(id), _type(ft) {
}


// s not used in this case
float ContrastCut::getDataCost (Coord p, ushort d) {
  if (p.x <2 || p.x > _id->w()-3 ||
      p.y <2 || p.y > _id->h()-3)
    return 1.;  // don't care about border pixels, for now

  float mean=0, lum, contrast=0;
  const unsigned char* I;
  int y,x, i;
  for (y=p.y-2, i=0; y<=p.y+2; ++y) {
    for (x=p.x-2; x<=p.x+2; ++x, ++i) {
      I = _id->_imptr(d,Coord(x,y));
      I = _id->images((int)d)->data() + 3*(y * _id->images((int)d)->_size.x + x);
      lum = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];
      mean += lum*_gaussianK5[i];
    } // x
  } // y

  mean /= .9997f;

  
  for (y=p.y-1, i=0; y<=p.y+1; ++y) {
    for (x=p.x-1; x<=p.x+1; ++x, ++i) {
      I = _id->_imptr(d,Coord(x,y));
      lum = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];
      contrast += fabsf(lum-mean) * _gaussianK3[i];
    }
  }
  contrast /= .9997f;
  
  float res = 255.f - contrast;    
  res *= .05f; // to equalize with inter = 1.;
  assert(res>=0);
  if (_type == Max)
	  return res;
  else if (_type == Min)
	  return (contrast*.05f);
  else {
	  assert (0);
	  return 0.;
  }
}


void ContrastCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {

  assert(ordered.empty());

  float score; //, lum;
  //unsigned char* I;
  int x, y;

  for (unsigned int i=0; i<_id->size(); ++i) {
    score = 0;
    //ImageAbs* im = _images[i];
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s) {
		  score += 12.75 - getDataCost(NULL, Coord(x+s, y), i);
      } // loop over span pixels
    } // loop over spans

    ordered.push(Ranker(i,score));
  } // loop over images

}

// sum .9997
float ContrastCut::_gaussianK5[25] = {0.0030f, 0.0133f, 0.0219f, 0.0133f, 0.0030f, 0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f, 0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f, 0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f, 0.0030f, 0.0133f, 0.0219f, 0.0133f, 0.0030f};


float ContrastCut::_gaussianK3[9] = {.0113f, .0838f, .0113f, .0838f, .6193f, .0838f, .0113f, .0838f, .0113f};


//#pragma managed
