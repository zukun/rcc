#include "panoCut.h"

PanoCut::PanoCut(ImageData* id) : AbstractDataObj(id) {

  //allocateComputeAllCGrads();
}

  
#define P_BLACK(I) (I[0]==0 && I[1]==0 && I[2]==0)

float PanoCut::getDataCost (Coord p, ushort d) {
  const unsigned char* I = _id->_imptr(d,p);
  if (P_BLACK(I))
    return 500;
  else
    return 0;
}


void PanoCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {

  assert(ordered.empty());

  float score;//, lum;
  const unsigned char* I;
  int x, y;

  for (unsigned int i=0; i<_id->size(); ++i) {
    score = 0;
    const ImageAbs* im = _id->images(i);
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s) {
	I = im->data(x+s,y);
	if (!P_BLACK(I))
	  score += 500;
      } // loop over span pixels
    } // loop over spans

    ordered.push(Ranker(i,score));
  } // loop over images

}
