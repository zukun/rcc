#ifndef IMAGECUT_H
#define IMAGECUT_H

#include "abstractDataObj.h"
#include "imageData.h"

class ImageCut : public AbstractDataObj {

 public:

  ImageCut(ImageData* id);

  float getDataCost (Coord p, ushort d) { assert(0); return 0; }
  float getDataCost (const struct StrokeData* s, Coord p, ushort d);
  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);

  int _num;


};

#endif
