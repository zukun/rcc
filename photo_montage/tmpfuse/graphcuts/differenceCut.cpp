#include "differenceCut.h"

DifferenceCut::DifferenceCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result) :
  PortraitCut(oWidth, oHeight, images, result, stdout) {
//  memset(_labels, 0, _w*_h*sizeof(ushort));
//  memset(_required, (short)-1, _w*_h*sizeof(short));
}

float DifferenceCut::BVZ_data_penalty(Coord p, ushort d) {

	const unsigned char* col = _imptr(d,p);
	Vec3i c(col[0], col[1], col[2]);
	float dist = sqrt(_avoidColor.distanceTo2(c));
	if (dist < 25.)
		return _bright * ( 25. - dist);
	else
		return 0;
}

float DifferenceCut::BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl) {

	return (_potts +  _regular_inter * (PortraitCut::BVZ_interaction_penalty(p,np,l,nl))); 
	
}

void DifferenceCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {
  assert(ordered.empty());
  float score, dist;
  const unsigned char *Itest, *Iavoid;
  Vec3i Vtest, Vavoid;
  int x, y, index;

  for (unsigned int i=0; i<_images.size(); ++i) {
    score = 0;
    ImageAbs* im = _images[i];
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s) {
	index = y*_w + x + s;
	Itest = im->data(x+s,y);
	Iavoid = _imptr(_labels[index], Coord(x+s,y));
	Vtest.Set(Itest[0], Itest[1], Itest[2]);
	Vavoid.Set(Iavoid[0], Iavoid[1], Iavoid[2]);
	dist = sqrt(Vtest.distanceTo2(Vavoid));
	//printf("%f\n",dist);
	if (dist > 25)
	  score += 25.;
	else
	  score += dist;
      }
    }
    printf("Image %d, score %f\n",i, score);
    ordered.push(Ranker(i,score));
  }
}