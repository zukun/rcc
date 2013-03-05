function unknownSegs = getUnknownRegions_multiSegs(ftype,thres)
%
% Compute entropy scores for all regions and select unknown segments from each image 
%
% Inputs: ftype (file containing image names of test data)
%         thres (takes those with scores > thres to be unknown)
%
% Outputs: unknownSegs (unknownSegs(i).segmentationInd = segmentation index
%            of image i, unknownSegs(i).segInd(num:end) = segment index of image i
%            unknownSegs(i).ordinalSegInd(num:end) = ordinal segment index
%            of image i (necessary due to way features are saved; if a segment is missing, 
%            e.g., [1 2 4 5] -> [1 2 3 4].  See 'getFeatures_unknownSegs.m')
%

global basedir prdir segdir

unknownSegs = [];
[B,count] = readTrainValTestIndices(ftype,basedir);

for i=1:length(B)
    imname = char(B{i});
    load([segdir imname '.seg.mat'],'Segs');   
    load([prdir imname '.mat'],'prim_multiSegs');
    
    % change number if more than 100 segments per image
    unknownSegs(i).segmentationInd = zeros(100,1);
    unknownSegs(i).segInd = zeros(100,1);
    unknownSegs(i).ordinalSegInd = zeros(100,1);
    
    num = 1;
    for j=1:size(Segs,3)
        thisSeg = double(Segs(:,:,j));     
        uniqueSegs = unique(thisSeg(:));
        N = length(uniqueSegs);
        
        entropy_scores = zeros(N,1);
        for k=1:N
            [r,c] = find(thisSeg==uniqueSegs(k));            
            pr_vec(:,1) = prim_multiSegs(j).prim(r(1),c(1),:);
            
            entropy_ = pr_vec.*log2(pr_vec);
            entropy_(isnan(entropy_)) = 0;
            entropy_scores(k) = -sum(entropy_);
        end
        
        % retain "unknown" segments
        segInd = find(entropy_scores>=thres);
        
        unknownSegs(i).segmentationInd(num:num+length(segInd)-1) = j*ones(length(segInd),1);
        unknownSegs(i).segInd(num:num+length(segInd)-1) = uniqueSegs(segInd);
        unknownSegs(i).ordinalSegInd(num:num+length(segInd)-1) = segInd;
        
        num = num + length(segInd);
    end
    
    unknownSegs(i).segmentationInd(num:end) = [];
    unknownSegs(i).segInd(num:end) = [];
    unknownSegs(i).ordinalSegInd(num:end) = [];
end

