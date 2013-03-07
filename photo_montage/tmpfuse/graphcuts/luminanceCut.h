#ifndef LUMINANCECUT_H
#define LUMINANCECUT_H

#include "abstractDataObj.h"
#include "imageData.h"


class LuminanceCut : public AbstractDataObj {

 public:
  enum FilterType { Min, Max };
  
  LuminanceCut(ImageData* id, FilterType ft);
  
  float getDataCost (Coord p, ushort d);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return getDataCost (p,d); }
  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);

  FilterType _type;

};



#endif
