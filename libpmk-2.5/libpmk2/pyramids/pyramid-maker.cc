// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <vector>
#include "pyramids/pyramid-maker.h"

using namespace std;

namespace libpmk {

vector<MultiResolutionHistogram*> PyramidMaker::
MakePyramids(const PointSetList& psl) {
  vector<MultiResolutionHistogram*> answer;
  for (int ii = 0; ii < psl.point_set_size(); ++ii) {
    MultiResolutionHistogram* mrh(MakePyramid(psl.point_set(ii)));
    answer.push_back(mrh);
  }
  return answer;
}

}  // namespace libpmk
