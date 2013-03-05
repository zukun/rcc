disp('compling detector/FindFourNeighborsMex.cc');
mex -outdir detector/ detector/FindFourNeighborsMex.cc

disp('complingdetector/FindTopologyNeighborMex.cc');
mex -outdir detector/ detector/FindTopologyNeighborMex.cc

disp('compling detector/GroupNeighborMex.cc');
mex -outdir detector/ detector/GroupNeighborMex.cc

disp('compling detector/SamplePtrsFromRegionsMex.cc');
mex -outdir detector/ detector/SamplePtrsFromRegionsMex.cc

disp('compling descriptor/BPLRMaskImgMex.cc');
mex -outdir descriptor/ descriptor/BPLRMaskImgMex.cc

disp('compling descriptor/ChordiogramMex.cc');
mex -outdir descriptor/ descriptor/Feat2ChordiogramMex.cc

disp('compling descriptor/ComputePHOGMex.cc');
mex -outdir descriptor/ descriptor/Feat2PHOGMex.cc

disp('compling descriptor/PHOGBoundBoxMex.cc');
mex -outdir descriptor/ descriptor/PHOGBoundBoxMex.cc

