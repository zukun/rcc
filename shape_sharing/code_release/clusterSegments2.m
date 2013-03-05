function sel = clusterSegments2(segments, seg_score, seg_score_for_bin, overlap_thresh, N_TARGET, n_grid, img_size, do_strict)


n_seg = size(segments,2);
% pixel_overlap = computeSegmentOverlap(segments, img_size, 0.25);

segments = reshape(segments, img_size(1),img_size(2),n_seg);
segm = imresize(segments, 0.4, 'nearest');
segm = reshape(segm, size(segm,1)*size(segm,2), n_seg);
pixel_overlap = segm_overlap_mex(segm);       
clear segm
segments = reshape(segments, img_size(1)*img_size(2),n_seg);

sel = clusterSegmentMini2_(pixel_overlap, seg_score, overlap_thresh);


if ( N_TARGET < numel(sel) )
    sample_rate= N_TARGET/n_seg;
    pick_sel = pickSegmentSPM(segments(:, sel), seg_score_for_bin(sel), img_size, n_grid, sample_rate, N_TARGET, do_strict);
    sel = sel(pick_sel);   
end
