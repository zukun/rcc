function [sel, pixel_overlap] = clusterGraphCutSegments(segments, overlap_thresh, gPb_thin, img_size)

% cut score
e = gPb_thin > 0.0;
[im_dt im_dtL] = bwdist(e);
cut_score =...
    computeCutScore2(segments, im_dt, im_dtL, gPb_thin, img_size);
bdry_tf = pickBdrySegments(segments, img_size);
n_seg = size(segments,2);
cut_score_ = zeros(n_seg,1);
cut_score_(bdry_tf) = cut_score(bdry_tf,2);
cut_score_(~bdry_tf) = cut_score(~bdry_tf,1);
seg_score = cut_score_;

% overlap
segments = reshape(segments, img_size(1),img_size(2),n_seg);
segm = imresize(segments, 0.4, 'nearest');
segm = reshape(segm, size(segm,1)*size(segm,2), n_seg);
pixel_overlap = segm_overlap_mex(segm);       
clear segm

% clustering
sel = clusterSegmentMini2_(pixel_overlap, seg_score, overlap_thresh);

