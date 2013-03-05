
function [F] = getFeatures_multisegs(ftype,basedir)
%
% get features for each segment in multiple-segmentations
%

% global variables
global gtdir histdir textondim colordim pHOGdim

% read in image filenames
[B,count] = readTrainValTestIndices(ftype,basedir);

% 50000 is maximum expected number of segments in data -- need to change
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
    load([histdir fname '.hists.mat'],'multiSegs','segname');  % load texton, color hist    
    load([segname],'Segs');  % load superpixel image
    
    N = unique(newseg(:));
    numsegs = length(N);
    
    % the void regions do not have labels -- label them to be 0
    if length(newlabels) < numsegs 
        newlabels = [newlabels zeros(1,numsegs-length(newlabels))];
    end  
    gt_labels = newlabels(newseg);
    
    for k=1:size(Segs,3)
        thisSeg = double(Segs(:,:,k));
        thisBlob = multiSegs(k).seg_blob;

        for j=1:length(thisBlob)
            segind = thisBlob(j).segnum;
            pixind = find(thisSeg==segind);
            thislabel = mode(gt_labels(pixind));
            
            textonfeats(num,:) = thisBlob(j).full_texton_hist';
            colorfeats(num,:) = thisBlob(j).color_hist';
            phogfeats(num,:) = thisBlob(j).phog;

            classlabels(num,:) = thislabel;
            imlabels(num,:) = {fname};
            seglabels(num,:) = segind;
            multiseglabels(num,:) = k;
            num = num + 1;
        end
    end
    
    clear Segs newseg newlabels multiSegs segname
end


F.textonfeats = textonfeats(1:num-1,:);
F.colorfeats = colorfeats(1:num-1,:);
F.phogfeats = phogfeats(1:num-1,:);
F.classlabels = classlabels(1:num-1,:);
F.imlabels = imlabels(1:num-1,:);
F.seglabels = seglabels(1:num-1,:);
F.multiseglabels = multiseglabels(1:num-1,:);

    

