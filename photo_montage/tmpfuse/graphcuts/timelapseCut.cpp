#include "timelapseCut.h"

TimelapseCut::TimelapseCut(ImageData* id) : AbstractDataObj (id) {
  // _center.Set(_w/2., _h/2.);
  //_center.Set(390,288); // construction
  //_center.Set(260,150); // daugther or, 205, 260
  _maxDist = MAX( _id->w()/2, _id->h()/2);
}


float TimelapseCut::getDataCost(Coord p, ushort d) {
  float dist;
  //dist = fabs ( (float(p.x) / _w) - 
  //	float(d) / _n);

  dist = fabs ( (float(p.x) / float(_id->w())) - 
  	float(d) / float(_id->size()));

  //float tx = float(p.x) - _center.x(), ty = float(p.y) - _center.y();
  //dist = sqrt(tx*tx + ty*ty) / _maxDist;
  
  //const unsigned char* I = _imptr(d,Coord(p.x,p.y));
  //if (I[0] == 212 && I[1] == 209 && I[2] == 206)
  //return 100;

  //if (fabs(dist*_n - d) > 10)
  //return 50;
  //else
  //return 0;

  //dist = 10.*(float(d)/float(_n) - dist);
  //return dist*dist;
  //return 1.f * fabs( float(d)/float(_n) - dist  );
  return dist;
}



void TimelapseCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {
  assert(ordered.empty());

  float score; //, lum;
  //unsigned char* I;
  int x, y;

  for (unsigned int i=0; i<_id->size(); ++i) {
    score = 0;
    //ImageAbs* im = _images[i];
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s)
		  score += 1.f - fabs ( (float(x+s) / float (_id->w())) - float (i) / float (_id->size ())); 
    } // loop over spans
    ordered.push(Ranker(i,score));
  } // loop over images

}
