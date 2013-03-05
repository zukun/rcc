function [our_mask, Segs, selind] = videoSegmentation(BPLR_,param)

% %%%%%%%%%%
% 
% INPUT - default in ()
% BPLR_ (1): 0 - color likelihood only; 1 - full model (with partial shape match)
% 
% param.fgNbStates (5): # of fg GMMs
% param.bgNbStates (5): # of bg GMMs
% param.alpha_ (0.5): color-cost vs. partial shape match cost
% param.gamma_ (4): smoothness weight; use 50 for color-only (i.e., if BPLR_=0)
% 
% param.hypothesisNum (1): rank of hypothesis to process for video segmentation
% param.skip (1): # of frames to skip for hypothesis discovery.  Increase to reduce computation time
% param.numTopRegions (10): # of regions to consider per frame for hypothesis discovery
% param.knn (5): # parameter for constructing k-nn graph for hypothesis discovery
%  
% param.imdir: video frame directory (code assumes that images will be either jpg, png, or bmp)
% param.opticalflowdir:optical flow directory
% param.regiondir: region proposals directory
% param.bplrdir: bplr directory
%
% OUTPUT
% our_mask: binary segmentation of video corresponding to hypothesis param.hypothesisNum
% Segs: top regions per frame
% selind: selected regions across video
%
% Yong Jae Lee, 7/26/2012
% %%%%%%%%%%

finalToc = [];
%% get hypotheses (i.e., keysegment clusters)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tic;
fprintf(['\n\nComputing keysegment hypotheses...']);
[A Segs imNdx scores hypotheses] = ...
    getRegionAffinitiesMotion(param.imdir, param.regiondir, param.opticalflowdir, param.skip, param.numTopRegions, param.knn);
selind = hypotheses(param.hypothesisNum).selind;
disp(['done [' num2str(toc) ' seconds]']);
finalToc = [finalToc toc];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Training of GMM by EM algorithm, initialized by k-means clustering.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tic;
fprintf(['Computing GMMs...']);
[fg,bg] = gmmKeySegs(param.imdir,Segs,selind,param);
disp(['done [' num2str(toc) ' seconds]']);
finalToc = [finalToc toc];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% %% BPLRs of dominant cluster
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if BPLR_ == 1
    tic;
    fprintf(['Loading BPLRs of hypothesis...']);
    [fgFeats bgFeats] = getKeySegmentBPLRFeatures(Segs(selind), param.bplrdir);
    disp(['done [' num2str(toc) ' seconds]']);
    finalToc = [finalToc toc];
end
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% identify keyframes
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tic;
d = dir([param.imdir '*.jpg']);
if isempty(d)
    d = dir([param.imdir '*.png']);
end
if isempty(d)
    d = dir([param.imdir '*.bmp']);
end
numFrames = length(d);
keyFrames = zeros(1,numFrames);
for i=1:numFrames
    for j=1:length(selind)
        if strcmp(Segs(selind(j)).imname,d(i).name)
            keyFrames(i) = 1;
            break;
        end
    end
end
finalToc = [finalToc toc];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
tic;
fprintf(['Computing color and shape likelihoods...']);
for i=1:numFrames
    imname = d(i).name;        
    im = imread([param.imdir '/' imname]); 
    [nr, nc, z] = size(im);
    
    %% compute color likelihoods
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    [fgpim,bgpim,Data] = computePosteriors(im,nr,nc,param,fg,bg);    
    % adjust posteriors if frame contains keysegment
    if keyFrames(i) == 1
        [fgpim,bgpim,bgind] = adjustPosteriors(selind,nr,nc,Segs,imname,fgpim,bgpim);
    end    
    Dc = cat(3, bgpim, fgpim);
    
    % get vertical and horizontal smoothness
    [Hc,Vc] = computeSmoothness(im,param);
    
    frame(i).Dc = Dc;    
    frame(i).Hc = Hc;
    frame(i).Vc = Vc;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
%     %% compute partial shape match
%     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if BPLR_ == 1
    %     try
            bplr_file = [param.bplrdir '/' imname '_bplr.mat'];
            [shapeMask, pred_fg_bplrs] = computePartialShapeMatch(bplr_file,fgFeats,bgFeats,Segs,selind,nr,nc);

            if keyFrames(i) == 1
                shapeMask(bgind) = 0;
            end
            if isempty(pred_fg_bplrs)
                frame(i).BPLR_Dc = 0.5*ones(nr, nc, 2);
            else
                frame(i).BPLR_Dc = cat(3, 1-shapeMask, shapeMask);
            end
    %     catch
    %         frame(i).BPLR_Dc = 0.5*ones(nr, nc, 2);
    %     end
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
    
    %% compute inter frame connections
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if (i~=numFrames)
        nextIm = imread([param.imdir '/' d(i+1).name]); 
        [coloraff,opticalflow_map] = computeInterFrameConnections(param.opticalflowdir,Data,nextIm,d(i).name,d(i+1).name,param);
        
        frame(i).opticalflow_map = opticalflow_map;
        frame(i).opticalflow_coloraff = coloraff;
    else
        frame(i).opticalflow_map = [];
        frame(i).opticalflow_coloraff = [];
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   
end
disp(['done [' num2str(toc) ' seconds]']);
finalToc = [finalToc toc];


%% segment frames containing keysegments first; greedy-ordering
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tic;
keyFrameDist = bwdist(keyFrames);
keyFrameDist = [keyFrameDist(1)+1 keyFrameDist keyFrameDist(end)+1];
keyFrameL = convn(keyFrameDist,ones(1,3),'valid');
[sortVal, sortInd] = sort(keyFrameL,'ascend');
finalToc = [finalToc toc];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% compute segmentation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tic;
fprintf(['Compute segmentation for entire video...']);
our_mask = computeSegmentation(numFrames,nr,nc,param,sortInd,frame,BPLR_);
disp(['done [' num2str(toc) ' seconds]']);
finalToc = [finalToc toc];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

disp(['Total: ' num2str(sum(finalToc)) ' seconds']);

