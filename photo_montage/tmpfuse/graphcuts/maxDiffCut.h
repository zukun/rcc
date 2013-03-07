#ifndef _MAXDIFFCUT_H_
#define _MAXDIFFCUT_H_

#include "abstractDataObj.h"
#include "imageData.h"

class MaxDiffCut : public AbstractDataObj {

 public:
  MaxDiffCut(ImageData* id);
  
  float getDataCost (Coord p, ushort d);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return getDataCost (p,d); }
  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);

  void setSel(const int i) { _baseSel = i; }

 private:
  int _baseSel;

};

#endif
