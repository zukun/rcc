#ifndef CONTRASTCUT_H
#define CONTRASTCUT_H

#include "abstractDataObj.h"
#include "imageData.h"

class ContrastCut : public AbstractDataObj {

 public:

  enum FilterType { Min, Max };
	
  ContrastCut(ImageData* id, FilterType ft);

  float getDataCost (Coord p, ushort d);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return getDataCost (p,d); }
  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);



 protected:

  FilterType _type;
  static float _gaussianK5[25];
  static float _gaussianK3[9];

};

#endif 
