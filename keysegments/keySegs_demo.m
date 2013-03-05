% keySegs_demo
%
% do run the demo, download this data first: 
% vision.cs.utexas.edu/projects/keysegments/data.tgz
%
clear;

addpath(genpath('./code/'));

%% parameters
%%%%%%%%%%
BPLR_ = 1;

param.fgNbStates = 5;
param.bgNbStates = 5;
param.alpha_ = 0.5;
if BPLR_ == 1
    param.gamma_ = 4;
else
    param.gamma_ = 50;
end

param.skip = 1;
param.numTopRegions = 10;
param.knn = 5;

% videoName = 'birdfall2'; param.hypothesisNum = 1;
% videoName = 'girl'; param.hypothesisNum = 1;
% videoName = 'monkeydog'; param.hypothesisNum = 3;
% videoName = 'cheetah'; param.hypothesisNum = 2;
% videoName = 'parachute'; param.hypothesisNum = 1;
videoName = 'penguin'; param.hypothesisNum = 1;
 
param.imdir = ['./data/SegTrack/' videoName '/'];
param.opticalflowdir = ['./data/opticalFlow/' videoName '/'];
param.regiondir = ['./data/regionProposals/' videoName '/'];
param.bplrdir = ['./data/bplr/' videoName '/'];
%%%%%%%%%%

[our_mask, Segs, selind] = videoSegmentation(BPLR_,param);
% save('-v7',['./test/' videoName '.mat'],'our_mask', 'param', 'Segs', 'selind');
% save('-v7',['./test/' videoName '_colorOnly.mat'],'our_mask', 'param', 'Segs', 'selind');

evaluateSegTrackResults(param.imdir,our_mask,Segs,selind);

