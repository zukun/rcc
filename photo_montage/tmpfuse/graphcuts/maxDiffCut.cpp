#include "maxDiffCut.h"

#define MD_BRIGHT 0.5f

MaxDiffCut::MaxDiffCut(ImageData* id) : AbstractDataObj(id){
  _baseSel = -1;
}

float MaxDiffCut::getDataCost (Coord p, ushort d) {
  const unsigned char* I = _id->_imptr(d,p);
  const unsigned char* J = _id->_imptr(_baseSel, p);
  
  Vec3i colorI(I[0], I[1], I[2]);
  Vec3i colorJ(J[0], J[1], J[2]);
  
  float diff = sqrt(colorI.distanceTo2(colorJ));
  if (diff < 25.)
    return (MD_BRIGHT*(25. - diff));
  else
    return 0;
}


void MaxDiffCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {

  assert(ordered.empty());

  float score;
  int x, y;

  for (unsigned int i=0; i<_id->size(); ++i) {
    score = 0;
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s) {
	score += 25.f - getDataCost (Coord(x+s, y), i);
      } // loop over span pixels
    } // loop over spans
    ordered.push(Ranker(i,score));
  } // loop over images
}

