// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdlib.h>
#include <time.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include "point_set/mutable-point-set-list.h"
#include "point_set/point-ref.h"
#include "clustering/hierarchical-clusterer.h"
#include "util/distance-computer.h"
#include "pyramids/pyramid-matcher.h"
#include "pyramids/input-specific-vg-pyramid-maker.h"
#include "pyramids/input-specific-vg-pyramid-maker.h"
#include "histograms/multi-resolution-histogram.h"
#include "experiment/svm-experiment.h"
#include "eth-selector.h"

using namespace std;
using namespace libpmk;
using namespace libpmk_util;

void usage(const char* exec_name) {
   cerr << "Usage: " << exec_name << " input.psl output.hc levels branch\n\n";
   cerr << "<input.psl>: A PointSetList file, where each PointSet contains\n"
        << "             features for one image\n";
   cerr << "<levels>:    (int) Number of levels in the cluster tree\n";
   cerr << "<branch>:    (int) The branch factor of the cluster tree\n";
   cerr << "<labels.txt>: Label file\n";
}

vector<int> ReadLabels(string filename) {
   FILE* f = fopen(filename.c_str(), "r");
   int rows;
   assert(fscanf(f, "%d", &rows) != 0);
   
   vector<int> labels;
   for (int ii = 0; ii < rows; ++ii) {
      int value;
      assert(fscanf(f, "%d", &value) != 0);
      labels.push_back(value);
   }
   return labels;
}


int main(int argc, char** argv) {
   if (argc != 5) {
      usage(argv[0]);
      exit(0);
   }

   // Set the random seed for hierarchical clustering
   srand(time(NULL));

   string input_file(argv[1]);
   int num_levels = atoi(argv[2]);
   int branch_factor = atoi(argv[3]);
   string label_file(argv[4]);
   
   cout << "Reading " << input_file << endl;
   MutablePointSetList* psl = new MutablePointSetList();
   ifstream input_stream(input_file.c_str(), ios::binary);
   psl->ReadFromStream(input_stream);
   input_stream.close();
   
   vector<PointRef> point_refs;
   psl->GetPointRefs(&point_refs);

   cout << "Running hierarchical clustering" << endl;
   HierarchicalClusterer* clusterer = new HierarchicalClusterer();
   L2DistanceComputer dist_computer;
   clusterer->Cluster(num_levels, branch_factor, point_refs, dist_computer);

   InputSpecificVGPyramidMaker* vgpm =
      new InputSpecificVGPyramidMaker(*clusterer, dist_computer);

   cout << "Making pyramids" << endl;
   vector<MultiResolutionHistogram*> vec = vgpm->MakePyramids(*psl);
   KernelMatrix km(vec.size());

   cout << "Computing kernel matrix" << endl;
   for (int ii = 0; ii < (int)vec.size(); ++ii) {
      for (int jj = 0; jj <= ii; ++jj) {
         km.at(ii, jj) = PyramidMatcher::GetPyramidMatchSimilarity(
            *vec[ii], *vec[jj], BIN_WEIGHT_INPUT_SPECIFIC);
      }
   }

   km.Normalize();

   // Free some memory:
   delete vgpm;
   delete clusterer;
   for (int ii = 0; ii < (int)vec.size(); ++ii) {
      delete vec[ii];
   }
   delete psl;

   vector<int> labels = ReadLabels(label_file);
   double sum = 0;
   // There are 80 different experiments to run; this is what the
   // which_test variable iterates over. There are 400 images total;
   // one run of this experiment takes 5 examples as testing and the
   // rest as training.
   for (int which_test = 0; which_test < 80; ++which_test) {
      ETHSelector selector(labels, which_test);

      SVMExperiment experiment(selector.GetTrainingExamples(),
                               selector.GetTestingExamples(),
                               km, 10000);
      experiment.Train();
      experiment.Test();
      
      // Also output what the SVM predicted for these 5 test examples.
      sum += (double)experiment.GetNumCorrect() /
         experiment.GetNumTestExamples();
   }

   // Report the average over all 80 runs.
   printf("Average accuracy: %f\n", sum / 80);
   
   
   return 0;
}
