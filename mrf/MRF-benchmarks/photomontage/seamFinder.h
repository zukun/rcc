#ifndef SEAMFINDER_H
#define SEAMFINDER_H

#include <vector>
#include "image.h"
#include "coord.h"
#include "includes.h"
#include "energy.h"
#include "globals.h"

MRF::CostVal smoothCostFn(int pix1, int pix2, int dp,int dq);
bool allBad(const Coord& p, const std::vector<Image*>& imgs);

typedef Graph::captype captype;
typedef Graph::flowtype flowtype;

#define A_INFINITY 100000


class SeamFinder {

 public:

  SeamFinder(std::vector<Image*> ims);
  virtual ~SeamFinder();

  void run();

  virtual void runStudy(char *outstem, int argc, char* argv[]);

  void comp(Image* outIm);

  const int* labels() const { return _labels;}

 protected:

  virtual captype smoothness_penalty(const Coord& p, const Coord& q, const int d, const int dq);
  virtual captype data_penalty(const Coord& p, const int d);
  flowtype computeEnergy();
  void expand(int a);
  //void initializeAllBad();
  virtual void initializeResult();
  virtual EnergyFunction* getEnergyFunction(MRF::CostVal* dataCostArray);

  const std::vector<Image*> _imgs;
  flowtype _E;
  int _w, _h;
  Coord _im_size;
  int* _labels;
  int _n;
  Energy::Var* _node_vars;
  //bool* _allBad;
};


#endif
