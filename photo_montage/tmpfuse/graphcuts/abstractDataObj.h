#ifndef ABSTRACTDATAOBJ_H
#define ABSTRACTDATAOBJ_H

#include <vector>
#include <queue>
#include "defs.h"
#include "imageData.h"

class AbstractDataObj {

 public:

  AbstractDataObj(const ImageData* id) : _id(id) {}
  virtual ~AbstractDataObj() {}
  virtual float getDataCost (Coord p, ushort d) = 0;
  virtual float getDataCost (const struct StrokeData* s, Coord p, ushort d) = 0;
  virtual void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) = 0;

 protected:

  const ImageData* _id;

};





#endif
