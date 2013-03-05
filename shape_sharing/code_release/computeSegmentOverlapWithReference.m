function pixel_overlap = computeSegmentOverlapWithReference(segments, ref_ind, img_size, do_reshape)

% reference
ref = false(img_size);
ref(ref_ind) = true;
ref_size = numel(ref_ind);
    
% segment
if ( do_reshape )
    n_seg = size(segments,2);
    seg_size = arrayfun(@(idx)(sum(segments(:,idx))), 1:n_seg);
    ref = false(img_size);
    ref(ref_ind) = true;
    ref_size = numel(ref_ind);
    segments = reshape(segments, [img_size, n_seg]);
else
    n_seg = size(segments,3);
    seg_size = arrayfun(@(idx)(sum(sum(segments(:,:,idx)))), 1:n_seg);    
end
    

% pixel overlap
pixel_overlap = zeros(n_seg,1);
for i = 1 : n_seg
    seg_and = segments(:,:,i) & ref;
    common = sum(seg_and(:));
    u = seg_size(i) + ref_size - common;
    pixel_overlap(i) = common/u;
end