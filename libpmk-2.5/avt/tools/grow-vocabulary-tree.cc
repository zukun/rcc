// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "point_set/point-ref.h"
#include "point_set/mutable-point-set-list.h"
#include "point_set/on-disk-point-set-list.h"
#include "trees/incremental-vocabulary-tree.h"
#include "trees/vocabulary-tree-retriever.h"

using namespace std;
using namespace libpmk;
using namespace incremental_vtree;

void usage(const char* exec_name) {
   cerr << "Usage: " << exec_name << " input.psl output.hc levels branch\n\n";
   cerr << "<input.psl>: A PointSetList file, where each PointSet contains\n"
        << "             features for one image\n";
   cerr << "<output.vtree>: where to save the produced vocabulary tree\n";
   cerr << "<branch>:      (int) The branch factor of the tree\n";
   cerr << "<capacity>:    (int) Maximum number of points allowed in each visual word\n";
   cerr << "<reduction_factor>:  (double) How much the number of points in each visual word we want\n";
   cerr << "                           to reduce each time the tree is restructured\n";
   cerr << "<retrain_size>:      (int) How many points do we want to use to reconfigure the\n";
   cerr << "                           vocabulary tree\n";
}


int main(int argc, char** argv) {
   if (argc < 7) {
      usage(argv[0]);
      exit(0);
   }

   // Set the random seed for hierarchical clustering
   srand(0);

   string input_file(argv[1]);
   string output_file(argv[2]);

   int branch_factor = atoi(argv[3]);
   int max_num_points = atoi(argv[4]);
   double reduction_factor = atof(argv[5]);
   int recluster_set_size = atoi(argv[6]);


   cout << "Reading the pointsetlist file..." << flush;
   OnDiskPointSetList psl(input_file, 1, 10);
   cout << "done. " << psl.point_set_size() << " point sets read." << endl;

   IncrementalVocabularyTree vt(branch_factor, max_num_points,
                                reduction_factor, recluster_set_size);

   for (int ii = 0; ii < psl.point_set_size(); ++ii) {
     cout << "Inserting image " << ii + 1 << " ("
          << psl[ii].size() << " points)... " << flush;
     int id = vt.InsertImage(psl[ii]);
     cout << " inserted ID " << id << "." << endl;

     //vector<int> match_ids;
     //vector<double> match_scores;
     //VocabularyTreeRetriever::Retrieve(vt, psl[ii],
     //5, &match_ids, &match_scores);
     //cout << "Retrieved " << flush;
     //for (int jj = 0; jj < (int)match_ids.size(); ++jj) {
     //cout << match_ids[jj] << "(" << match_scores[jj] << ") " << flush;
     //}
     //cout << endl;
     //cout << "done. " << vt.size() << " nodes in tree." << endl;
   }

   cout << "Writing... " << flush;
   ofstream out(output_file.c_str(), ios::binary);
   vt.WriteToStream(out);
   out.close();
}
