#ifndef PANOCUT_H
#define PANOCUT_H

#include "abstractDataObj.h"
#include "imageData.h"

class PanoCut : public AbstractDataObj {

 public:

  PanoCut(ImageData* id);

  float getDataCost (Coord p, ushort d);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return getDataCost (p,d); }
  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);



};

#endif 
