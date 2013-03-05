
function [F] = getFeatures_GT(ftype,rmind,basedir)
%
% get features for each segment in ground-truth segmentations
%

% global variables
global gtdir gthistdir textondim colordim pHOGdim

% read in image filenames
[B,count] = readTrainValTestIndices(ftype,basedir);

% 10000 is maximum expected number of segments in data -- need to change
% this if more
textonfeats = zeros(10000,textondim);
colorfeats = zeros(10000,colordim);
phogfeats = zeros(10000,pHOGdim);
classlabels = zeros(10000,1);
imlabels = cell(10000,1);

num = 1;
for i=1:length(B)
    fname = char(B{i});
       
    % load gt data
    load([gtdir fname(1:end-4) '_GT.mat'], 'newseg', 'newlabels');    
    load([gthistdir fname '.hists.mat'], 'GTseg');
    
    N = unique(newseg(:));
    numsegs = length(N);
    
    % the void regions do not have labels -- label them to be 0
    if length(newlabels) < numsegs 
        newlabels = [newlabels zeros(1,numsegs-length(newlabels))];
    end  
    
    for j=1:numsegs      
        thislabel = newlabels(j);                  

        if isempty(find(rmind==thislabel,1))
            textonfeats(num,:) = GTseg.seg_blob(j).full_texton_hist';
            colorfeats(num,:) = GTseg.seg_blob(j).color_hist';
            phogfeats(num,:) = GTseg.seg_blob(j).phog;

            classlabels(num,:) = thislabel;
            imlabels(num,:) = {fname};
            num = num + 1;
        end
    end
    
    clear GTseg newseg newlabels
end

F.textonfeats = textonfeats(1:num-1,:);
F.colorfeats = colorfeats(1:num-1,:);
F.phogfeats = phogfeats(1:num-1,:);
F.classlabels = classlabels(1:num-1,:);
F.imlabels = imlabels(1:num-1,:);

    

