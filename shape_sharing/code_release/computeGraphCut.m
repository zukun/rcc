function [segments params n_comp] =...
    computeGraphCut(fg_cost, lambda, shape_prior, smoothness, smoothness_const,...
    img_size, bbox, min_overlap, min_obj_size, fix_seed)

% parameters
n_params = numel(lambda);
lx = bbox(1);
rx = bbox(2);
ty = bbox(3);
dy = bbox(4);
bg_cost = 1 - fg_cost;
if (fix_seed)
    
    fg_cost(ty:dy,lx) = 1e10;
    fg_cost(ty:dy,rx) = 1e10;
    fg_cost(ty,lx:rx) = 1e10;
    fg_cost(dy,lx:rx) = 1e10;
    len_bbox = 2*(rx-lx+1) + 2*(dy-ty+1);
    
    spbw = reshape(shape_prior, img_size);
    [y x] = find(spbw);
    cx = floor(mean(x) + 0.5);
    cy = floor(mean(y) + 0.5);
    t = ceil((sqrt(len_bbox)-1)/2);
    clx = max(cx-t, 1);
    crx = min(cx+t, img_size(2));
    cty = max(cy-t, 1);
    cdy = min(cy+t, img_size(1));
    fg_seed_bw = false(img_size);
    fg_seed_bw(cty:cdy, clx:crx) = true;
    fg_seed_bw = fg_seed_bw & spbw;
    
    bg_cost(fg_seed_bw) = 1e10;
    bg_cost(fg_seed_bw) = 1e10;
    bg_cost(fg_seed_bw) = 1e10;
    bg_cost(fg_seed_bw) = 1e10;
    
end

% resize by bbox
fg_cost = fg_cost(ty:dy,lx:rx);
bg_cost = bg_cost(ty:dy,lx:rx);
smoothness = smoothness(ty:dy, lx:rx);

% initial pruning (for saving computation)
sel = true(n_params,1);
n_fg_pixel = zeros(n_params,1);
shape_prior_size = sum(shape_prior);
init_seg = false(numel(fg_cost), n_params);
for i = 1 : n_params
    bc = bg_cost - lambda(i);
    fc = fg_cost + lambda(i);
    bw = fc < bc;
    init_seg(bw(:),i) = true;
    seg_size = sum(bw(:));
    n_fg_pixel(i) = seg_size;
    
    % prune by size
    max_seg_size = max(0.99*(rx-lx+1)*(dy-ty+1), shape_prior_size);
    sel(i) = seg_size < max_seg_size | seg_size > min_obj_size;
    
    if ( sel(i) )
        % pruning by overlap with shape_prior
        shape_prior_bw = reshape(shape_prior, img_size);
        shape_prior_bw = shape_prior_bw(ty:dy,lx:rx);
        seg_overlap =...
            computeSegmentOverlapWithReference(bw, find(shape_prior_bw), size(bw), false);
        sel(i) = seg_overlap >= min_overlap;    
    end
end
lambda = lambda(sel);
init_seg = init_seg(:,sel);

if ( isempty(lambda) )
    segments = [];
    params = [];
    n_comp = [];
    return;
end

% remove duplicate
% n_fg_pixel = n_fg_pixel(sel);
% resolution = 0.01*shape_prior_size;
% if ( resolution > 1 )
%     n_fg_pixel = ceil(n_fg_pixel/resolution);
% end
% [unused uid] = unique(n_fg_pixel);
% lambda = lambda(uid);
% n_params = numel(lambda);

% remove duplicate
cost_size = size(fg_cost);
n_init_seg = size(init_seg,2);
init_seg = reshape(init_seg, cost_size(1),cost_size(2), n_init_seg);
segm = imresize(init_seg, 0.4, 'nearest');
segm = reshape(segm, size(segm,1)*size(segm,2), n_init_seg);
pixel_overlap = segm_overlap_mex(segm);       
pixel_overlap = triu(pixel_overlap,1);
sel = true(1,n_init_seg);
for i = 1 : n_init_seg
    if ( sel(i) ) 
        sel(pixel_overlap(i,:) > 0.98) = false;
    end
end
lambda = lambda(sel);
n_params = numel(lambda);

% segmentation params
MIN_COMPONENT_SIZE_RATE = 0.25;
DataCost = zeros([size(fg_cost),2]);
segments = false(prod(img_size),n_params);
params = zeros(1, n_params);
n_comp = zeros(n_params,1);
% n_hole = zeros(n_params,1);

% graph-cut
for i = 1 : n_params
    % params
    params(i) = lambda(i);
    
    % unary
    DataCost(:,:,1) = bg_cost - lambda(i);
    DataCost(:,:,2) = fg_cost + lambda(i);
    
    % smoothness
    SmoothnessCost = smoothness_const*(1 - eye(2));
        
    % graph cut
    [gch] = GraphCut('open', DataCost, SmoothnessCost, smoothness, smoothness);
    gch = GraphCut('expand', gch, [], 0);
    [gch labels] = GraphCut('expand', gch, [], 1);
    [gch] = GraphCut('close', gch);
    labels = logical(labels);
    bw = false(img_size);
    bw(ty:dy,lx:rx) = labels;
        
    % remove noise
    cc = bwconncomp(bw);
    numPixels = cellfun(@numel,cc.PixelIdxList);
    biggest = max(numPixels);
    tf = numPixels < MIN_COMPONENT_SIZE_RATE*biggest;
    bw(cell2mat(cc.PixelIdxList(tf)')) = false;
    n_comp(i) = sum(~tf);
    segments(:,i) = bw(:);    
    
%     tic;
%     % bg holes
%     cc = bwconncomp(~bw);
%     numPixels = cellfun(@numel,cc.PixelIdxList);
%     tf = numPixels > min_obj_size;
%     if (sum(tf) > 1 )
%         [by bx] = arrayfun(@(idx)(ind2sub(img_size, cc.PixelIdxList{idx})), find(tf), 'uniformoutput', false);
%         blx = cellfun(@min, bx);
%         brx = cellfun(@max, bx);
%         bty = cellfun(@min, by);
%         bdy = cellfun(@max, by);
%         n_hole(i) = sum(blx > lx & brx <rx & bty > ty & bdy < dy);
%     end    
%     toc;
    
end
