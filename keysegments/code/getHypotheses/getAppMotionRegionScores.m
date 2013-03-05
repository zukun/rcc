function [CombinedScores, imNdx] = getAppMotionRegionScores(imdir, regiondir, opticalflowdir, skip)

bdryPix = 30;
flowBinEdges = [-15:0.5:15];

d = dir([imdir '*.jpg']);
if isempty(d)
    d = dir([imdir '*.png']);
end
if isempty(d)
    d = dir([imdir '*.bmp']);
end

IanScores = [];
MotionScores = [];
imNdx = [];

for imInd = 1:skip:length(d)-1;    
    
    imname1 = d(imInd).name;        
    imname2 = d(imInd+1).name;  
    
    im = imread([imdir imname1]); 
    [nr, nc, z] = size(im);

    %%%%%%%%%%%%%%%%%%%%
    % load superpixels and bg probability map
    segname = [regiondir imname1 '.mat'];
    load(segname, 'proposals', 'superpixels', 'unary');
    
    flowFile = [opticalflowdir imname1 '_to_' imname2 '.opticalflow.mat'];

    try
        load(flowFile,'vx','vy');
    catch
        im1 = double(imread([imdir imname1]));
        im2 = double(imread([imdir imname2]));

        flow = mex_LDOF(im1,im2);
        vx = flow(:,:,1);
        vy = flow(:,:,2);
    end
    %%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%

    diffUnary = diff(unary,1,1);
    ind = find(diffUnary>0,1);
    proposals = proposals(1:ind);

    IanScores = [IanScores; unary(1:ind)];
    
    N = length(proposals);
    flowAffs = zeros(N,1);

    clear Segs
    for j=1:N    
        segNdx = ismember(superpixels, proposals{j});

        Seg = zeros(nr,nc);   
        Seg(segNdx) = 1;    
    
        %%%%%%%%%%%%%%%%%%%%%%
        %% get bbox region of interest    
        [y,x] = find(Seg==1);    

        minx = min(x); maxx = max(x); 
        miny = min(y); maxy = max(y);          

        tbdry = max(miny-bdryPix,1);
        bbdry = min(maxy+bdryPix,nr);
        lbdry = max(minx-bdryPix,1);
        rbdry = min(maxx+bdryPix,nc);

        ROISeg = Seg(tbdry:bbdry, lbdry:rbdry);
        fg_inds = find(ROISeg==1);
        bg_inds = find(ROISeg==0);
        %%%%%%%%%%%%%%%%%%%%%%

        %%%%%%%%%%%%%%%%%%%%%%
        %% get fg bg region optical flow histograms    
        ROIvx = vx(tbdry:bbdry, lbdry:rbdry); 
        ROIvy = vy(tbdry:bbdry, lbdry:rbdry); 
        
        fg_flow_hist = normalizeFeats([histc(ROIvx(fg_inds), flowBinEdges); histc(ROIvy(fg_inds), flowBinEdges)]');
        bg_flow_hist = normalizeFeats([histc(ROIvx(bg_inds), flowBinEdges); histc(ROIvy(bg_inds), flowBinEdges)]');

        flowDist = pdist2(fg_flow_hist,bg_flow_hist,'chisq');        
        flowAffinity = exp(-flowDist);    
        %%%%%%%%%%%%%%%%%%%%%%  

        flowAffs(j) = flowAffinity;
    end

    MotionScores = [MotionScores; 1-flowAffs];    
    imNdx = [imNdx; imInd*ones(N,1)];
%     imInd
end

%%%%%%%%%%%%%%%%%%%%%%
%% compute means and stds
meanIanScores = mean(IanScores,1);
stdIanScores = std(IanScores,1);

meanMotionScores = mean(MotionScores,1);
stdMotionScores = std(MotionScores,1);
%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%
%% normalize to same scale
norm_IanScores = (IanScores-meanIanScores)/stdIanScores;
norm_MotionScores = (MotionScores-meanMotionScores)/stdMotionScores;
%%%%%%%%%%%%%%%%%%%%%%

CombinedScores = norm_IanScores+norm_MotionScores;
