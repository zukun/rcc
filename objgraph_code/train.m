%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% this script trains N known classifiers and outputs pixel-level
%% posterior probabilities.
%%
%% INPUT: 
%% texton,color,phog features stored in gthistdir and histdir as a struct, where
%% e.g., GTseg.seg_blob(j).full_texton_hist -- j indexes the segments given
%% by ground-truth segments
%% and e.g., multiSegs(k).seg_blob(j).full_texton_hist -- k indexes
%% multiple-segmentations and j indexes segments
%% (details are in feature_extraction directory)
%%
%% OUTPUT:
%% N-dim pixel-level posteriors, where
%% e.g., prim_multiSegs(k).prim -- k indexes multiple segmentations
%%
%% need to add existing software:
%% (1) multiple kernel learning code by F. R. Bach, G. R. G.
%% Lanckriet, and M. I. Jordan
%% http://www.stat.berkeley.edu/~gobo/SKMsmo.tar
%% (2) libsvm by C-C. Chang and C-J. Lin
%% http://www.csie.ntu.edu.tw/~cjlin/libsvm/
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% add appropriate paths
addpath('commonfiles/');
addpath(genpath('external/'));
addpath('libsvm/'); % libsvm
addpath('SKMsmo/'); % multiple kernel learning
addpath('train/');

global basedir gtdir gthistdir histdir segdir textondim colordim pHOGdim

%% CHANGE THESE ACCORDINGLY
basedir = './set1/';
gtdir = './clean_msrc2_segmentations/newsegmentations_mats/';
gthistdir = './gthistfiles/';
histdir = './histfiles/';
segdir = './segfiles/';
textondim = 400;
colordim = 69;
pHOGdim = 680;

trainftype = 'Train.txt'; % need to store this file in basedir 
testftype = 'Test.txt'; % need to store this file in basedir 

%% MSRC-v2
classnames = {'void','building','grass','tree','cow','horse','sheep','sky','mountain','aeroplane','water','face','car','bicycle'...
,'flower','sign','bird','book','chair','road','cat','dog','body','boat'};
voidind = [0 5 8]; % void, horse, mountain (not used in MSRC-v2)
unknownclasses = [1 3 4 9 13]; % building, tree, cow, airplane, bicycle
rmind = [voidind unknownclasses]; 
numFeatType = 3; % texton, color, phog
valclasslabels = 0:23;
valclasslabels([rmind+1]) = []; % known classes
numclasses = length(valclasslabels);

%% output directory to save the pixel-level posteriors
sp_prdir = [basedir 'superpixeltopixelprob/'];
if exist(sp_prdir,'dir')==0
    mkdir(sp_prdir);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% get chisq kernel with gamma = 0.5
featType = getKernels(trainftype,testftype,rmind,basedir);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% compute svm classification (via multiple kernel learning)
C = 100;
prob_estimates = svm_mkl(C,valclasslabels,featType);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% convert region probabilities to pixel-level probabilities
regionToPixelProbability(testftype,numclasses,basedir,sp_prdir,featType,prob_estimates);







