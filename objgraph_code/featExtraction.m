%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% this script computes for each image:
%% (1) multiple-segmentations
%% (2) Superpixel segmentations (about 50 per image)
%% (3) texton, color, and phog features for each segment
%%
%% need to add existing software:
%% (1) normalized cuts by J. Shi and J. Malik
%% http://www.cis.upenn.edu/~jshi/software/
%% (2) Berkeley benchmark and boundary detection code
%% http://www.eecs.berkeley.edu/Research/Projects/CS/vision/bsds/code/segbench.tar.gz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% CHANGE THESE ACCORDINGLY
% Where the images and ground-truth are stored:
imdir = './MSRC_ObjCategImageDatabase_v2/Images/';
gtdir = './clean_msrc2_segmentations/newsegmentations_mats/';
% Where to save the features and segmentations:
featdir = './filterfiles/';
vocoutdir = './vocabdata/';
segdir = './segfiles/';
gthistoutdir = './gthistfiles/';
histoutdir = './histfiles/';
sp2dir = './segfiles_sp2_25/';
sp2histoutdir = './histfiles_sp2_25/';

NUM_RESPONSES_PER_IMAGE = 1000; % how many samples (pixels) to take per image to compute texton vocab
NUM_TEXTONS = 400; % how many texton clusters to form 
%%

%% add Normalized Cut and Berkely detection code
addpath('Ncut/');
addpath('segbench/Util');

addpath(genpath('external/'));
addpath(genpath('feature_extraction/'));

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% compute textons for each image
features_filters(imdir, featdir, vocoutdir, 'bmp', NUM_RESPONSES_PER_IMAGE, NUM_TEXTONS); 

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% multiple-segmentations
features_segments(imdir, segdir, 'bmp');

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% multiple-segmentation features
features_hists(imdir, segdir, featdir, histoutdir, 'bmp');

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% ground-truth segmentation features
GT_features_hists(imdir, gtdir, featdir, gthistoutdir, 'bmp');

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Superpixel segmentations
features_sp2_segments(imdir, sp2dir, 'bmp');

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Superpixel features
features_sp2_hists(imdir, sp2dir, featdir, sp2histoutdir, 'bmp');
