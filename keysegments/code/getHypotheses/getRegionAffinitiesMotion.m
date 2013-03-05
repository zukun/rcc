function [A Segs imNdx scores hypotheses] = getRegionAffinitiesMotion(imdir, regiondir, opticalflowdir, skip, numTopRegions, knn)

[CombinedScores, scores_imNdx] = getAppMotionRegionScores(imdir, regiondir, opticalflowdir, skip);

%%%%%%%%%%%%%%%%%%%%%%%%
% parameters
NUM_COLORS = 69;

numColorHistBins = 22;
maxLValue = 100;

LBinSize = maxLValue / numColorHistBins;
abBinSize = 256 / numColorHistBins;
LBinEdges = [0:LBinSize:100];
abBinEdges = [-128:abBinSize:128];
%%%%%%%%%%%%%%%%%%%%%%%%

d = dir([imdir '*.jpg']);
if isempty(d)
    d = dir([imdir '*.png']);
end
if isempty(d)
    d = dir([imdir '*.bmp']);
end

N = length(d) - 1; % exclude last frame -- no optical flow estimates

color_hists = zeros(ceil(numTopRegions*N/skip), NUM_COLORS);
imNdx = zeros(ceil(numTopRegions*N/skip), 1);
scores = zeros(ceil(numTopRegions*N/skip), 1);

num = 1;
for i=1:skip:N
    im = imread([imdir d(i).name]); 
    
    regionname = [regiondir d(i).name '.mat'];   
    load(regionname, 'proposals', 'superpixels');   
    
    [sortedVal, sortedInd] = sort(CombinedScores(scores_imNdx==i), 'descend');    
    proposals = proposals(sortedInd(1:numTopRegions));    
    
    [L,a,b] = RGB2Lab(double(im(:,:,1)), double(im(:,:,2)), double(im(:,:,3)));
    if(max(L(:)) > 100 | min(L(:)) < 0)
        fprintf('error in L range\n');
        keyboard;
    end
    if(max(a(:)) > 128 | min(a(:)) < -128)
        fprintf('error in a range\n');
        keyboard;
    end
    if(max(b(:)) > 128 | min(b(:)) < -128)
        fprintf('error in b range\n');
        keyboard;
    end   
    
    for j=1:numTopRegions 
        segNdx = ismember(superpixels, proposals{j});            
        color_hists(num, :) = [histc(L(segNdx), LBinEdges); histc(a(segNdx), abBinEdges); histc(b(segNdx), abBinEdges)];
                
        imNdx(num) = i;
        scores(num) = sortedVal(j);
        
        Segs(num).proposal = segNdx;
        Segs(num).imname = [d(i).name];
        Segs(num).frameNdx = i;
        
        num = num + 1;        
    end
end

color_hists(num:end, :) = [];               
imNdx(num:end) = [];
scores(num:end) = [];
        
color_dist = slmetric_pw(color_hists', color_hists', 'chisq');
color_mean = mean(mean(color_dist));
color_K = exp(-1/color_mean*color_dist);

A = color_K;

% construct k-nn graph
newA = zeros(size(A));
for i=1:size(A,1)
    [val,ind] = sort(A(i,:),'descend');
    newA(i,ind(1:knn)) = val(1:knn);
end
A = (newA+newA')/2;

[final_indic_vec, V, D] = getSingleCluster(A, 1);

numHypotheses = min(49, size(A,1));

meanScore = zeros(numHypotheses,1);
for i=1:numHypotheses
    [final_indic_vec, V, D] = getSingleCluster(A, i, V, D);
    selind = find(final_indic_vec==1);
    meanScore(i) = mean(scores(selind));
    hypotheses(i).selind = selind;
end
[sortedScores, sortedNdx] = sort(meanScore,'descend');
hypotheses = hypotheses(sortedNdx);
rmind = [];
for i=1:length(hypotheses)
   for j=i+1:length(hypotheses)
       if ~isempty(intersect(hypotheses(i).selind,hypotheses(j).selind))
           rmind = [rmind; j];
       end
   end
end
rmind = unique(rmind);
hypotheses(rmind) = [];
sortedNdx(rmind) = [];
