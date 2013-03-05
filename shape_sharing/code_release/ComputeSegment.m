function [masks, timing] = ComputeSegment(img_file)
%-------------------------------------------------------------------------%
% generating object segment hypotheses 
%-------------------------------------------------------------------------%
% img_file: full file path to an input image
% masks: h x w x n_seg : binary foreground masks for object hypotheses
% timing: computation time for each step


%% parameters
n_max_feat = 8000;
fg_k = 5;
bg_k = 3;
n_target1 = 800;
n_grid = 5;
overlap_threshs = 0.5;
do_adjust = true;
use_all_shape = false;

%% load db
db_file = 'data/db_trainset.mat';
load(db_file, 'shape_contexts', 'obj_mask', 'obj_mask_area', 'obj_mask_locs', 'img_obj_mask_index_table');
im = imread(img_file);
img_size = size(im);
img_size = img_size(1:2);

%% gpb
disp('-----------------------------------');
disp('gpb');

% [unused1, img_name, unused2] = fileparts(img_file);
% out_file = ['output/' img_name '_gpb.mat'];

% tic;
[ucm, ucm2, gpb_thin, gpb_orient, t_gpb] = ComputeGPB(img_file);
timing.t_gpb = t_gpb;
% t_gpb = toc;


%% get multiple segmentations
disp('-----------------------------------');
disp('multiple segmentation');

tic;
n_level = 100;
cutoff_thresh = 0.25;
while (1)
    regions = getHierarchicalRegions(ucm2, n_level, cutoff_thresh);
    if ( numel(regions) > 1000 )
        cutoff_thresh = cutoff_thresh + 0.05;
    else
        break;
    end
end
t_multiseg = toc;
timing.t_multiseg = t_multiseg;



%% match BPLR
disp('-----------------------------------');
disp('match bplr');

[matched_img_inds matched_obj_inds matched_bplr_scales matched_bplr_locs] =...
    BPLRMatch(im, ucm, gpb_orient, gpb_thin, n_max_feat, fg_k, bg_k);
timing.t_match = toc;

% gt mask
mask_inds = img_obj_mask_index_table(sub2ind(size(img_obj_mask_index_table), matched_img_inds, matched_obj_inds));
src_mask = obj_mask(mask_inds);
src_locs = obj_mask_locs(:,mask_inds);
if ( ~use_all_shape)
    shape_contexts = shape_contexts(:,mask_inds);
end
clear obj_mask

%% projection
disp('-----------------------------------');
disp('shape projection');
tic;
[overlap_score, shape_score, overlap, abs_err,... 
proj_bw, proj_tran, proj_area, merge_mat, merged_bw_inds, merged_bw_area, vote,...
out_inds t_shape_proj] =...
    BPLR2Segment__(src_mask, src_locs, regions,...
            matched_bplr_scales, matched_bplr_locs,...
            n_target1, overlap_threshs,...
            n_grid, shape_contexts,...
            do_adjust, use_all_shape);
timing.t_shape_proj = t_shape_proj;

%% prune merge
disp('-----------------------------------');
disp('pruning');
tic;
% parameters
overlap_thresh = 0.95;
n_grid = 10;
n_merge = numel(merged_bw_inds);
merge_segments = false(prod(img_size),n_merge);
for i = 1 : n_merge
    merge_segments(merged_bw_inds{i},i) = true;
end
e = gpb_thin > 0.0;
[im_dt im_dtL] = bwdist(e);
[merge_cut_score] =...
    computeCutScore2(merge_segments, im_dt, im_dtL, gpb_thin, img_size);
bdry_tf = pickBdrySegments(merge_segments, img_size);
merge_cut_score_ = zeros(n_merge,1);
merge_cut_score_(bdry_tf) = merge_cut_score(bdry_tf,2);
merge_cut_score_(~bdry_tf) = merge_cut_score(~bdry_tf,1);
sel = clusterSegments2(merge_segments, merge_cut_score_, merge_cut_score_(:,1), overlap_thresh, 600, n_grid, img_size, true);
merge_segments = merge_segments(:,sel);
t_prune = toc;
timing.t_prune = t_prune;

disp('-----------------------------------');
disp('multi-paramteric graph-cut');
tic;
%% graph-cut segmentation
segments = graphCutSegmentationWrapper(merge_segments, im, gpb_thin, gpb_orient);
t_gc = toc;
timing.t_gc = t_gc;

n_seg = size(segments,2);
masks = reshape(segments, [img_size, n_seg]);

% %% evaluation
% % pascal
% [max_overlap_seg, max_overlap_inds_seg, obj_size] = reportSegmentationResult(segments, img_idx, '2010');
% % bsds
% [max_overlap_seg, max_overlap_inds_seg, obj_size] = reportSegmentationResult(segments, img_idx, []);





