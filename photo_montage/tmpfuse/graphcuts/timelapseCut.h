#ifndef TIMELAPSECUT_H
#define TIMELAPSECUT_H

#include "abstractDataObj.h"
#include "imageData.h"

class TimelapseCut : public AbstractDataObj {

 public:

  TimelapseCut(ImageData* id);


  float getDataCost (Coord p, ushort d);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return getDataCost (p,d); }
  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);


 protected:

  Vec2f _center;
  float _maxDist;


};


#endif
