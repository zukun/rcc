#include "luminanceCut.h"
#define L_BRIGHT .5f

LuminanceCut::LuminanceCut(ImageData* id, FilterType ft) :
  AbstractDataObj(id), _type(ft) {
}


float LuminanceCut::getDataCost (Coord p, ushort d) {
  if (_type == Max)
    return L_BRIGHT*(255.f - _id->getLum(p, d)); // encourage brightness	
  else if (_type == Min)
    return L_BRIGHT*_id->getLum(p, d); // encourage the darkness!  My personal fav...
  else return 0;
}

void LuminanceCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {

  assert(ordered.empty());

  float score, lum;
  const unsigned char* I;
  int x, y;

  for (unsigned int i=0; i<_id->size(); ++i) {
    score = 0;
    const ImageAbs* im = _id->images(i);
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s) {
	I = im->data(x+s,y);
	lum = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];
	if (_type==Max) {
	  score +=  lum;
	}
	else if (_type==Min) {
	  score += 255.f - lum;
	}
      } // loop over span pixels
    } // loop over spans

    ordered.push(Ranker(i,score));
  } // loop over images


}
