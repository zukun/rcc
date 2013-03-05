function im = getRegionFeatures_multiSegs(ftype,unknownSegs,numSp)
%
% Get object-graph features for each unknown segment 
%
% Inputs: ftype (file containing image names of test data)
%         unknownSegs (unknownSegs(i).segmentationInd = segmentation index
%            of image i, unknownSegs(i).segInd(num:end) = segment index of image i
%            unknownSegs(i).ordinalSegInd(num:end) = ordinal segment index
%            of image i (necessary due to way features are saved; if a segment is missing, 
%            e.g., [1 2 4 5] -> [1 2 3 4].  See 'getFeatures_unknownSegs.m')
%         numSp (number of superpixel regions to consider above and below for object-graph)
%
% Outputs: im (struct for i'th image with 
%            im(i).imname, im(i).classlabel [N x 1], for N unknown reigons, 
%            im(i).feats = [N x 2*R*numclasses] obj-graph descriptors
%

global basedir prdir gtdir segdir minispdir

[B,count] = readTrainValTestIndices(ftype,basedir);
for i=1:length(B)
    imname = char(B{i});

    load([minispdir imname '.mat'], 'Sp2'); % load oversegmented image
    load([gtdir imname(1:end-4) '_GT.mat'], 'newseg', 'newlabels'); % load ground-truth image       
    numsegs = length(unique(newseg(:)));    
    if length(newlabels) < numsegs 
        newlabels = [newlabels zeros(1,numsegs-length(newlabels))];
    end  
    gt_labels = newlabels(newseg);
        
    % average the posteriors over all segmentations to get pixel-level posteriors
    load([prdir imname '.mat'],'prim_multiSegs');    
    meanprim = zeros(size(prim_multiSegs(1).prim));
    for j=1:length(prim_multiSegs)
        meanprim = meanprim + prim_multiSegs(j).prim;
    end
    meanprim = meanprim/length(prim_multiSegs);
    
    clear overseg
    % record sp centroid position
    sp2segs = unique(Sp2(:));
    numsp2 = length(sp2segs);
    sp_locs = zeros(numsp2,2);
    for j=1:numsp2
       inds = find(Sp2 == sp2segs(j)); 
       overseg(j).inds = inds;
       [r,c] = find(Sp2 == sp2segs(j));
       sp_locs(j,:) = round(mean([r,c],1));  
    end
    
    % summarize pixel-level posteriors at the Sp2 level          
    prim = zeros(size(meanprim));
    for j=1:size(meanprim,3)
       thismeanprim = meanprim(:,:,j);           
       thisprim = zeros(size(thismeanprim));      
        
       for k=1:numsp2
           inds = overseg(k).inds; 
           thisprim(inds) = mean(thismeanprim(inds));
       end
       prim(:,:,j) = thisprim;    
    end        
    
    segmentationInd = unknownSegs(i).segmentationInd;
    segInd = unknownSegs(i).segInd; 
    load([segdir imname '.seg.mat'], 'Segs');    
    Segs = double(Segs);
    
    im(i).classlabel = zeros(length(segmentationInd),1);
    im(i).feats = zeros(length(segmentationInd),numSp*2*size(meanprim,3)); % 2R*numclasses
    
    num = 1; 
    for j=1:length(segmentationInd)
        thisSeg = Segs(:,:,segmentationInd(j));              
        
        inds = find(thisSeg==segInd(j));
        [r,c] = find(thisSeg==segInd(j));
        reg_locs = round(mean([r,c],1));  % region centroid position
       
        im(i).classlabel(num) = mode(gt_labels(inds));        
        
        % remove superpixels that overlap with the segment
        skipind = [];
        for k=1:numsp2
           if ~isempty(intersect(inds,overseg(k).inds))
               skipind = [skipind; k];
           end   
        end
      
        thissp_locs = sp_locs;
        thissp_locs(skipind,:) = [];
        
        [obj_graph] = getObjectGraphDescriptor(reg_locs,thissp_locs,prim,numSp);
        
        obj_graph = obj_graph';
        im(i).feats(num,:) = obj_graph(:);
        
        num = num + 1;
    end
end
