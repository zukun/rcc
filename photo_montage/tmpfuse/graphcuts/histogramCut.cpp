#include "histogramCut.h"

HistogramCut::HistogramCut(ImageData* id, FilterType ft) :AbstractDataObj(id), _type(ft) {
}

float HistogramCut::getDataCost (Coord p, ushort d) {
  if (_hist == NULL) {
    assert (0);
    return 0.;
  }
  
  PixelHisto* ph = _hist->GetDataForPixel (p.x, p.y);
  const unsigned char* I = _id->_imptr(d,p);
  
  float prob = ph->GetProbability (I[0], I[1], I[2]);
  assert (prob >= 0 && prob <= 1.0);
  float cost;
  if (_type == Max) 
    cost = 1.f - prob;
  else if (_type == Min)
    cost = prob;
  
  return cost;
}

void HistogramCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {

  assert(ordered.empty());
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
	PixelHisto* ph = _hist->GetDataForPixel (x+s, y);
	float prob = ph->GetProbability (I[0], I[1], I[2]);
	assert (prob >= 0 && prob <= 1.0);
	if (_type == Max) 
	  score += prob;
	else if (_type == Min)
	  score += 1.f - prob;
	
      } // loop over span pixels
    } // loop over spans
    
    ordered.push(Ranker(i,score));
  } // loop over images
}
