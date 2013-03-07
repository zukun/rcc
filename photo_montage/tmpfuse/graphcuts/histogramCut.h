#ifndef HISTOGRAMCUT_H
#define HISTOGRAMCUT_H

#include "abstractDataObj.h"
#include "imageData.h"
#include "histogram.h"

class HistogramCut : public AbstractDataObj {

 public:
  
  enum FilterType { Min, Max };
  HistogramCut(ImageData* id, FilterType ft);
  
  float getDataCost (Coord p, ushort d);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return getDataCost (p,d); }
  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);
  
  FilterType _type;
  void setHistogram (const Histogram* h) { _hist = h; }
  
 protected:
  const Histogram* _hist;
  
};


#endif
