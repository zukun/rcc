#ifndef COLORCLONECUT_H
#define COLORCLONECUT_H


#include "abstractDataObj.h"
#include "imageData.h"


class ColorCloneCut : public AbstractDataObj {

 public:

  ColorCloneCut(ImageData* id);

  float getDataCost (Coord p, ushort d) { assert(0); return 0; }
  float getDataCost (const struct StrokeData* s, Coord p, ushort d);
  void rankImages(std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);

  Vec3f _dialColor; 
	

};



#endif
