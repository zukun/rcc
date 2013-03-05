// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <math.h>
#include <iostream>
#include <vector>
#include "detector/detector.h"
#include "detector/harris-detector.h"
#include "detector/mser-detector.h"
#include "detector/grid-detector.h"
#include "detector/surf-detector.h"
#include "detector/filtering-grid-detector.h"
#include "core/interest-point.h"
#include "extractor/location-extractor.h"
#include "extractor/surf-extractor.h"
#include "point_set/point-set.h"
#include "point_set/mutable-point-set-list.h"

using namespace libpmk_features;

void usage(const char* exec) {
   cout << "Usage: " << exec << " <output_psl> <img1> ... <imgN>" << endl;
}

int main(int argc, char** argv) {
   if (argc < 3) {
      usage(argv[0]);
      exit(0);
   }

   string output_file(argv[1]);
   int num_files = argc - 2;

   // Choose your detector by uncommenting one of these:
   // MSER detector:
   //MSERDetector detector;
   //detector.SetBinaryPath("./mser.ln");

   // Harris detector:
   // HarrisDetector detector;
   // detector.SetBinaryPath("./h_affine.ln");
   // detector.SetDetectorType(HarrisDetector::kHessianAffine);

   // Grid detector:
   // FilteringGridDetector detector(8, 8, 8, 80);

   // SURF detector:
   SURFDetector detector;
   detector.SetBinaryPath("../bin/surf.ln");

   // Choose your extractor by uncommenting one of these:
   
   // Location extractor (just report 2-d X/Y coordinates):
   //LocationExtractor extractor;

   // SIFT (or PCASIFT) extractor:
   // SIFTExtractor extractor;
   // extractor.SetBinaryPath("./compute_descriptors.ln");
   // extractor.SetPCA(true);
   // extractor.SetPCADim(12);

   // Others:
   // SteerableFilterExtractor extractor;
   // ShapeContextExtractor extractor;

   SURFExtractor extractor;
   extractor.SetBinaryPath("../bin/surf.ln");

   MutablePointSetList psl;

   int total_features = 0;
   int ignored_images = 0;
   for (int ii = 2; ii < argc; ++ii) {
      cout << "Processing image " << ii - 2 << " of " << num_files << "..."
           << flush;
      cout << argv[ii] << flush;
      vector<InterestPoint> vec;
      detector.DetectInterestPoints(argv[ii], &vec);
      if ((int)vec.size() > 0) {
         total_features += (int)vec.size();
         psl.add_point_set(extractor.ExtractFeatures(argv[ii], &vec));
      } else {
        cout << "ignored image " << argv[ii] << endl;
         ++ignored_images;
      }
      cout << " done.\n";
   }
   
   cout << "Total features: " << total_features << endl;
   cout << "Average num features per image: "
        << (double)total_features / (num_files - ignored_images) << endl;
   cout << "Num ignored images: " << ignored_images << endl;
   psl.WriteToFile(output_file.c_str());
   cout << "Complete." << endl;
   return 0;
}

