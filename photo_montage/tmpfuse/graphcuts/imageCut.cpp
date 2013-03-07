#include "imageCut.h"

ImageCut::ImageCut(ImageData* id) : AbstractDataObj(id) {
  _num = -1;
}

//nothing to rank - we've picked an image
void ImageCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {
  assert(_num>=0);
  ordered.push (Ranker (_num, 0));
}

float ImageCut::getDataCost (const struct StrokeData* s, Coord p, ushort d) {
  _num = s->_num;
  assert(_num>=0);
  if (d==_num) 
    return 0.f; // good
  else 
    return A_INFINITY;       // bad
}
