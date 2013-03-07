#include "colorCloneCut.h"
#define CC_BRIGHT .5f

ColorCloneCut::ColorCloneCut(ImageData* id) : AbstractDataObj(id) {

  //these are only really going to apply to global computations
  //not stroke based
  //constants for stroke based interaction is in dialcut

  _dialColor = Vec3f(-1, -1, -1);
}


float ColorCloneCut::getDataCost (const struct StrokeData* s, Coord p, ushort d) {
  _dialColor = s->_dialColor;
  const unsigned char* I = _id->_imptr(d,p);
  Vec3f thisColor(I[0], I[1], I[2]);
  thisColor *= (1.f/255.f);
  float res = CC_BRIGHT*20.*(sqrt(thisColor.distanceTo2(_dialColor)));
  return res;
}


void ColorCloneCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {

  assert(ordered.empty());
  assert(_dialColor.r() != -1);
  float score;
  const unsigned char* I;
  int x, y;
  
  for (unsigned int i=0; i<_id->size(); ++i) {
    score = 0;
    const ImageAbs* im = _id->images(i);
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s) {
	I = im->data(x+s,y);
	Vec3f thisColor;
	thisColor.Set( float(I[0]) / 255.f, float(I[1]) / 255.f, float(I[2]) / 255.f);
	thisColor -= _dialColor;
	score += 1.74f - thisColor.Length();	
      } // loop over span pixels
    } // loop over spans
    
    ordered.push(Ranker(i,score));
  } // loop over images
  
}
