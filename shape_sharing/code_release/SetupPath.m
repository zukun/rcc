%% external codes
external_code_path = 'external_code/';

bplr_path = [external_code_path '/BPLR/'];
addpath(bplr_path);
addpath([bplr_path '/detector'])
addpath([bplr_path '/detector/util'])
addpath([bplr_path '/descriptor'])
addpath([bplr_path '/display'])
addpath([bplr_path '/external/matlab_bgl/'])
addpath([bplr_path '/external/pwmetric/'])
addpath([bplr_path '/external/spanning_tree'])
addpath([bplr_path '/external/vlfeat-0.9.8/toolbox'])
vl_setup


gc_path = [external_code_path '/GCmex1.4/'];
addpath(gc_path)

gpb_path = [external_code_path '/globalPb/'];
addpath(genpath(gpb_path));
system(['chmod u+rwx ' external_code_path '/globalPb/lib/*']);
flann_path = [external_code_path '/flann-1.21-src-64/build/matlab/'];
addpath(flann_path);

%% mex files
addpath('mex');

%% pascal
addpath('./pascal/VOCcode/');
