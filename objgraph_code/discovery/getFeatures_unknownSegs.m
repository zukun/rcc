
function [F] = getFeatures_unknownSegs(ftype,unknownSegs)
%
% Gets appearance features (textons, color histograms, pHOG) for each unknown region 
%
% Inputs: ftype (filename containing list of Testing instances)
%         unknownSegs (unknownSegs(i).segmentationInd = segmentation index
%            of image i, unknownSegs(i).segInd(num:end) = segment index of image i
%            unknownSegs(i).ordinalSegInd(num:end) = ordinal segment index
%            of image i (necessary due to way features are saved; if a segment is missing, 
%            e.g., [1 2 4 5] -> [1 2 3 4].  
%
% Outputs: F (F.textonfeats: textons
%            F.colorfeats: color histogram
%            F.phogfeats: pHOG
%            F.classlabels: class labels
%            F.imlabels: image names
%            F.multiseglabels: segmentation indices
%            F.seglabels: segment indices           
%

global basedir gtdir segdir histdir textondim colordim pHOGdim

[B,count] = readTrainValTestIndices(ftype,basedir);

% 50000 is maximum expected number of unknown segments in data -- need to change
% this if more
textonfeats = zeros(50000,textondim);
colorfeats = zeros(50000,colordim);
phogfeats = zeros(50000,pHOGdim);
classlabels = zeros(50000,1);
imlabels = cell(50000,1);
seglabels = zeros(50000,1);
multiseglabels = zeros(50000,1);

num = 1;
for i=1:length(B)
    fname = char(B{i}); 
    
    % load gt data
    load([gtdir fname(1:end-4) '_GT.mat'], 'newseg', 'newlabels');
    load([histdir fname '.hists.mat'],'multiSegs');  % load texton, color hist    
    load([segdir fname '.seg.mat'],'Segs');  % load superpixel image
    Segs = double(Segs);
    
    N = unique(newseg(:));
    numsegs = length(N);    
    % the void regions do not have labels -- label them to be 0
    if length(newlabels) < numsegs 
        newlabels = [newlabels zeros(1,numsegs-length(newlabels))];
    end  
    gt_labels = newlabels(newseg);
    
    segmentationInd = unknownSegs(i).segmentationInd;
    ordinalSegInd = unknownSegs(i).ordinalSegInd;
    segInd = unknownSegs(i).segInd;

    for j=1:length(segmentationInd)
        thisSeg = Segs(:,:,segmentationInd(j));       
        thisBlob = multiSegs(segmentationInd(j)).seg_blob;

        pixind = find(thisSeg==segInd(j));
        thislabel = mode(gt_labels(pixind));  
        
        textonfeats(num,:) = thisBlob(ordinalSegInd(j)).full_texton_hist';
        colorfeats(num,:) = thisBlob(ordinalSegInd(j)).color_hist';
        phogfeats(num,:) = thisBlob(ordinalSegInd(j)).phog;

        classlabels(num,:) = thislabel;
        imlabels(num,:) = {fname};
        multiseglabels(num,:) = segmentationInd(j);
        seglabels(num,:) = segInd(j);
        num = num + 1;
    end
    
    clear Segs newseg newlabels multiSegs
end


F.textonfeats = textonfeats(1:num-1,:);
F.colorfeats = colorfeats(1:num-1,:);
F.phogfeats = phogfeats(1:num-1,:);
F.classlabels = classlabels(1:num-1,:);
F.imlabels = imlabels(1:num-1,:);
F.multiseglabels = multiseglabels(1:num-1,:);
F.seglabels = seglabels(1:num-1,:);

    

