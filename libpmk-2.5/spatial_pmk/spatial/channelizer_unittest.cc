// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "point_set/point-ref.h"
#include "point_set/mutable-point-set-list.h"
#include "spatial/channelizer.h"
#include "util/distance-computer.h"

using namespace std;
using namespace libpmk;
using namespace spatial_pmk;

void TestChannelization(KMeansChannelizer& channelizer) {
  // Try all pairs of ii, jj being positive and negative.
  for (int ii = -1; ii == -1 || ii == 1; ii += 2) {
    for (int jj = -1; jj == -1 || jj == 1; jj += 2) {
      Point center(2);
      center[0] = ii * 10;
      center[1] = jj * 10;

      Point test_point(2);
      test_point[0] = ii * 10.01;
      test_point[1] = jj * 10.01;

      Point non_cluster_point(2);
      non_cluster_point[0] = -test_point[0];
      non_cluster_point[1] = -test_point[1];

      assert(channelizer.Channelize(center) ==
             channelizer.Channelize(test_point));

      assert(channelizer.Channelize(center) !=
             channelizer.Channelize(non_cluster_point));
             
      assert(channelizer.Channelize(test_point) !=
             channelizer.Channelize(non_cluster_point));
      
    }
  }
}

void TestReadAndWrite(KMeansChannelizer& channelizer) {
  ostringstream output_stream(ios::binary);
  channelizer.WriteToStream(output_stream);

  string s = output_stream.str();

  // Re-read what we just wrote and compare it to the original
  L2DistanceComputer distance_computer;
  KMeansChannelizer channelizer_copy(6, 100, distance_computer);
  istringstream input_stream2(s, ios::binary);
  channelizer_copy.ReadFromStream(input_stream2);
  TestChannelization(channelizer_copy);
  
}

void RunAllTests() {
  // The synthetic data set contains 25 points, in 5 clusters of 5 points.
  // The cluster centers are (0,0), and (+/- 10, +/- 10).
  L2DistanceComputer distance_computer;
  KMeansChannelizer channelizer(6, 100, distance_computer);
  
  MutablePointSetList psl;
  psl.ReadFromFile("testdata/synthetic_easy.psl");
  vector<PointRef> point_refs;
  psl.GetPointRefs(&point_refs);
  channelizer.InitializeChannels(point_refs);

  TestChannelization(channelizer);
  TestReadAndWrite(channelizer);
}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "*******\nPASS\n*******\n";
  return 0;
}
