%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% this script computes object-graph descriptors for each region, and
%% combines them with appearance features to discover novel categories.
%%
%% OUTPUT:
%% Purity rates and affinity matrices for the unknown instances.
%% Saved in [basedir outsubdir]
%% Call PlotClusterRates.m to view purity rates.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


addpath('discovery/');
addpath('commonfiles/');
addpath(genpath('external/'));

%% CHANGE THESE ACCORDINGLY
global basedir prdir gtdir segdir minispdir histdir textondim colordim pHOGdim

basedir = './set1/';
prdir = [basedir 'superpixeltopixelprob/'];
gtdir = './clean_msrc2_segmentations/newsegmentations_mats/';
segdir = './segfiles/';
minispdir = './segfiles_sp2_25/';
histdir = './histfiles/';
outsubdir = ['discovery/20sp_allSegs/'];
textondim = 400;
colordim = 69;
pHOGdim = 680;
%%

%% parameters
nbClusters = 1:35; 
NUMRESTARTS = 10; % number of runs
voidind = [0 5 8]; % void, horse, mountain (not used in MSRC-v2)
unknownclasses = [1 3 4 9 13]; % building, tree, cow, airplane, bike
featTypes = {'combined','appearance'}; % order is important!!!
testftype = 'Test.txt';

rmind = [voidind unknownclasses];  % void, building, tree, cow, horse, sheep, mountain, airplane, car, bike
discoveryLabels = unknownclasses; 
valclasslabels = 0:23;
valclasslabels([rmind+1]) = []; % known classes
numSp = 20; % number of superpixel regions to consider above and below for object-graph

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Perform discovery
discoverNovelCategories(discoveryLabels,featTypes,nbClusters,NUMRESTARTS,outsubdir,testftype,voidind,valclasslabels,numSp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Plot the purity of full-model vs. appearance-only
d = [basedir outsubdir 'threshold=2/'];
plotClusterRates(d,max(nbClusters),NUMRESTARTS,featTypes);

