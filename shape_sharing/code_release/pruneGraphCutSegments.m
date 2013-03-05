function sel = pruneGraphCutSegments(segments, n_comp, shape_prior, bbox,...
    img_size, MIN_OVERLAP, MIN_SEG_SIZE)

if ( isempty(segments) )
    sel = [];
    return;
end

n_seg = size(segments,2);
sel = true(1,n_seg);

% remove segments with holes or disconnected components
sel(n_comp>1) = false;

% remove too small segments
seg_size = sum(segments, 1);
sel(seg_size < MIN_SEG_SIZE) = false;

% remove too big segments
lx = bbox(1);
rx = bbox(2);
ty = bbox(3);
dy = bbox(4);
n_seg = size(segments,2);
seg_size = arrayfun(@(idx)(sum(segments(:,idx))), 1:n_seg);
MAX_SEG_SIZE = max(0.99*(rx-lx+1)*(dy-ty+1), sum(shape_prior(:)));
sel(seg_size > MAX_SEG_SIZE) = false;

if ( sum(sel) == 0 )
    return;
end

% remove segments with small overlap with shape prior
overlap_with_shape_prior =...
    computeSegmentOverlapWithReference(segments, find(shape_prior), img_size, true);
sel(overlap_with_shape_prior < MIN_OVERLAP) = false;

% remove duplicates
segments = reshape(segments, img_size(1),img_size(2),n_seg);
segm = imresize(segments, 0.4, 'nearest');
segm = reshape(segm, size(segm,1)*size(segm,2), n_seg);
pixel_overlap = segm_overlap_mex(segm);       
pixel_overlap = triu(pixel_overlap,1);
for i = 1 : n_seg
    if ( sel(i) ) 
        sel(pixel_overlap(i,:) > 0.98) = false;
    end
end



