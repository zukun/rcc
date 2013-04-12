#include "familySeamFinder.h"
#include "mrf.h"
#include "ICM.h"
#include "GCoptimization.h"
#include "MaxProdBP.h"
#include "TRW-S.h"
#include "BP-S.h"


//-------

MRF::CostVal edgySmoothCostFn(int pix1, int pix2, int dp,int dq) {
  //printf("yo %d %d %d\n",g_width, g_height, g_sources.size());

  Coord p(pix1%g_width, pix1/g_width);
  Coord q(pix2%g_width, pix2/g_width);

  if (dp==dq) return 0;
  if (g_sources[dp]->trans(p.x, p.y) && g_sources[dq]->trans(q.x, q.y))
    return 0;
  if (g_sources[dp]->trans(p.x, p.y) || g_sources[dq]->trans(q.x, q.y) ||
      g_sources[dq]->trans(p.x, p.y) || g_sources[dp]->trans(q.x, q.y))
    return A_INFINITY;

  captype M;
  M = (int)(g_sources[dp]->distTo(p,g_sources[dq]) + g_sources[dp]->distTo(q,g_sources[dq]));
  M+=1;

  // Edges

  //M /= 6;


  float G=0;
  if (p.x!=q.x) {// vertical cut
    Coord minp(std::min(p.x,q.x), p.y);
    float a = g_sources[dp]->vertGradMag(minp);
    float b = g_sources[dq]->vertGradMag(minp);
    if (a==-1 || b==-1)
      G = 1.f;
    else
      G = .5f*(a+b);
  }
  else { // horizontal cut
    Coord minp(p.x, std::min(p.y,q.y));
    float a = g_sources[dp]->horizGradMag(minp);
    float b = g_sources[dq]->horizGradMag(minp);
    if (a==-1 || b==-1)
      G = 1.f;
    else
      G = .5f*(a+b);
  }

  if (G==0)
    M = A_INFINITY;
  else
    M = (int) (float(M*100) / G);

  if (M>A_INFINITY) M = A_INFINITY;
  return M;

}

//-------



FamilySeamFinder::FamilySeamFinder(std::vector<Image*> ims, const Image* dataCostImage) :
  SeamFinder(ims) {

  _dataConstraints = new int[_w*_h];
  int ind=0;
  for (int j=0; j<_h; ++j)
    for (int i=0; i<_w; ++i, ++ind) {
      const CType* ptr = dataCostImage->getPixC(ind);
      if (ptr[0]!=255) {
	_dataConstraints[ind] = ptr[0] / 20;
	//printf("%d constrained to %d\n",ind,_dataConstraints[ind]);
      }
      else 
	_dataConstraints[ind] = -1;
    }

  //printf("%d\n",computeEnergy()); 
}

void FamilySeamFinder::initializeResult() {
  memset(_labels,0, _w*_h*sizeof(int));
  //printf("hi\n");
  Coord p;
  //int i;
  int ind;

  for (p.y=0, ind=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++, ++ind) {
      if (_dataConstraints[ind] !=-1)
	_labels[ind] = _dataConstraints[ind];
    }
}


captype FamilySeamFinder::data_penalty(const Coord& p, const int d) {
  int ind = p.y*_w+p.x;
  if (_dataConstraints[ind] == -1)
    return 0;
  else {
    if (_dataConstraints[ind] == d)
      return 0;
    else
      return A_INFINITY;
  } 
}

captype FamilySeamFinder::smoothness_penalty(const Coord& p, const Coord& q, const int dp, const int dq) {
  
   if (dp==dq) return 0;
  if (g_sources[dp]->trans(p.x, p.y) && g_sources[dq]->trans(q.x, q.y))
    return 0;
  if (g_sources[dp]->trans(p.x, p.y) || g_sources[dq]->trans(q.x, q.y) ||
      g_sources[dq]->trans(p.x, p.y) || g_sources[dp]->trans(q.x, q.y))
    return A_INFINITY;

  captype M;
  M = (int)(g_sources[dp]->distTo(p,g_sources[dq]) + g_sources[dp]->distTo(q,g_sources[dq]));
  M+=1;

  // Edges

  //M /= 6;


  float G=0;
  if (p.x!=q.x) {// vertical cut
    Coord minp(std::min(p.x,q.x), p.y);
    float a = g_sources[dp]->vertGradMag(minp);
    float b = g_sources[dq]->vertGradMag(minp);
    if (a==-1 || b==-1)
      G = 1.f;
    else
      G = .5f*(a+b);
  }
  else { // horizontal cut
    Coord minp(p.x, std::min(p.y,q.y));
    float a = g_sources[dp]->horizGradMag(minp);
    float b = g_sources[dq]->horizGradMag(minp);
    if (a==-1 || b==-1)
      G = 1.f;
    else
      G = .5f*(a+b);
  }

  if (G==0)
    M = A_INFINITY;
  else
    M = (int) (float(M*100) / G);

  if (M>A_INFINITY) M = A_INFINITY;
  return M;

}


EnergyFunction* FamilySeamFinder::getEnergyFunction(MRF::CostVal* dataCostArray) {
  DataCost *dataCostObj = new DataCost(dataCostArray);
  SmoothnessCost *smoothCostObj = new SmoothnessCost(&edgySmoothCostFn);
  return (new EnergyFunction(dataCostObj, smoothCostObj));

}
