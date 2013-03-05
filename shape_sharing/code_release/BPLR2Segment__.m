function [overlap_score, shape_score, overlap, abs_err,... 
    proj_bw, proj_tran, proj_area, merge_mat, merged_bw_inds, merged_bw_area,...
    vote, out_inds t_total] =...
    BPLR2Segment__(src_mask, src_locs, regions,...
    matched_bplr_scales, matched_bplr_locs,...
    n_target, overlap_threshs,...
    n_grid, shape_contexts,...
    do_adjust, use_all_shape)
    
t_total = 0;
proj_bw = cell(0);
merged_bw_inds = cell(0);
overlap_score = [];
merge_mat = [];
proj_area = [];
proj_tran = [];
abs_err = [];
overlap = [];
vote = [];
out_inds = [];
img_size = regions(1).img_size;

t_init = 0;
[overlap_ori abs_err_ori proj_area_ori t_] =...
    computeProjSpOverlap(src_mask, regions, matched_bplr_scales, matched_bplr_locs);
t_init = t_init + t_;
t_total = t_total + t_init;

n_scale = numel(overlap_threshs);
for i = 1 : n_scale
    [proj_bw_ merged_bw_inds_ overlap_ overlap_score_ abs_err_ merge_mat_ proj_tran_ proj_area_ out_inds_ t_] =...
        computeMerge(src_mask, src_locs, regions, overlap_ori, abs_err_ori, proj_area_ori, overlap_threshs(i), do_adjust);
    n_proj = numel(proj_bw_);
    proj_bw(end+1:end+n_proj) = proj_bw_;
    merged_bw_inds(end+1:end+n_proj) = merged_bw_inds_;
    overlap_score(end+1:end+n_proj) = overlap_score_;
    proj_area(end+1:end+n_proj) = proj_area_;
    proj_tran = [proj_tran proj_tran_];
    merge_mat = [merge_mat merge_mat_];
    abs_err = [abs_err abs_err_];
    overlap = [overlap overlap_];
    out_inds = [out_inds out_inds_(:)'];
    t_total = t_total + t_;
    t_init = t_init + t_;
end

% remove duplicates
[overlap_score rr] = sort(overlap_score, 'descend');
proj_area = proj_area(rr);
% proj_tran = proj_tran(:,rr);
merge_mat = merge_mat(:,rr);
abs_err = abs_err(:,rr);
overlap = overlap(:,rr);
out_inds = out_inds(rr);
proj_bw = proj_bw(rr);
merged_bw_inds = merged_bw_inds(rr);
matched_bplr_locs = matched_bplr_locs(:, out_inds);
if ( ~use_all_shape)
    shape_contexts = shape_contexts(:, out_inds);
end

% pruning
tic;
overlap_thresh = 0.05;
abs_err_thresh = 0.05;
tf =...
    pruneProjections(overlap_score, merge_mat, [regions.area], regions(1).img_size,...
    merged_bw_inds, overlap_thresh, matched_bplr_locs, abs_err, abs_err_thresh);

overlap_score = overlap_score(tf);
proj_area = proj_area(tf);
% proj_tran = proj_tran(tf);
abs_err = abs_err(:,tf);
overlap = overlap(:,tf);
out_inds = out_inds(tf);
proj_bw = proj_bw(tf);
merged_bw_inds = merged_bw_inds(tf);
matched_bplr_locs = matched_bplr_locs(:, tf);
merge_mat = merge_mat(:, tf);
if ( ~use_all_shape )
    shape_contexts = shape_contexts(:, tf);
end

t_prune = toc;
t_total = t_total + t_prune;

% trimming
tic;
img_size_cell = repmat({img_size}, 1, numel(merged_bw_inds));
merged_bw_inds = cellfun(@trimSegment, merged_bw_inds, img_size_cell, 'uniformoutput', false);
t_trim = toc;
t_total = t_total + t_trim;


% rank by shape
merged_bw_area = cellfun(@numel, merged_bw_inds);
merged_bw_area = merged_bw_area./prod(img_size);
n_angle_bin_sc = 12;
n_length_bin_sc = 12;


n_proj = numel(proj_area);
n1 = sum(merged_bw_area < 0.3);
n2 = n_proj - n1;
%n_very_large = sum(merged_bw_area > 0.5);
%large_rate = n_very_large/n_proj;
%target_rate = min(n_target/(n_proj-n_very_large), 1.0);
target_rate = min(n_target/n_proj, 1.0);
n2_bias = target_rate/2;
n1_bias = (n2/n1)*n2_bias;

% % for small object
area_thresh_lb = 0;
area_thresh_ub = 0.10;
rsz_min = 1;
rsz_max = 1;
n_int = 1;
% disp('ranking small objects...');
tic;
[shape_score_s overlap_score_s proj_area_s abs_err_s overlap_s out_inds_s proj_bw_s merged_bw_inds_s merge_mat_s matched_bplr_locs_s selected_inds_s] =...
    RankShapeContextSPM(overlap_score, proj_area, abs_err, overlap, out_inds, proj_bw, merged_bw_inds, merge_mat, merged_bw_area, matched_bplr_locs,...
    shape_contexts,...
    n_angle_bin_sc, n_length_bin_sc, img_size,...
    area_thresh_lb, area_thresh_ub,...
    target_rate + n1_bias,...
    rsz_min, rsz_max, n_int,...
    n_grid, use_all_shape);
comp_time_s = toc;
% for medium object
area_thresh_lb = 0.10;
area_thresh_ub = 0.30;
rsz_min = 1.0;
rsz_max = 1.0;
n_int = 1;
% disp('ranking medium objects...');
tic;
[shape_score_m overlap_score_m proj_area_m abs_err_m overlap_m out_inds_m proj_bw_m merged_bw_inds_m merge_mat_m matched_bplr_locs_m selected_inds_m] =...
    RankShapeContextSPM(overlap_score, proj_area, abs_err, overlap, out_inds, proj_bw, merged_bw_inds, merge_mat, merged_bw_area, matched_bplr_locs,...
    shape_contexts,...
    n_angle_bin_sc, n_length_bin_sc, img_size,...
    area_thresh_lb, area_thresh_ub,...
    target_rate + n1_bias,...
    rsz_min, rsz_max, n_int,...
    n_grid, use_all_shape);
comp_time_m = toc;

% for large object
area_thresh_lb = 0.3;
area_thresh_ub = 1.0;
rsz_min = 0.5;
rsz_max = 0.5;
n_int = 1;
% disp('ranking large objects...');
tic;
[shape_score_l overlap_score_l proj_area_l abs_err_l overlap_l out_inds_l proj_bw_l merged_bw_inds_l merge_mat_l matched_bplr_locs_l selected_inds_l] =...
    RankShapeContextSPM(overlap_score, proj_area, abs_err, overlap, out_inds, proj_bw, merged_bw_inds, merge_mat, merged_bw_area, matched_bplr_locs,...
    shape_contexts,...
    n_angle_bin_sc, n_length_bin_sc, img_size,...
    area_thresh_lb, area_thresh_ub,...
    target_rate - n2_bias,...
    rsz_min, rsz_max, n_int,...
    n_grid, use_all_shape);
comp_time_l = toc;


selected_inds = [selected_inds_s selected_inds_m selected_inds_l];

out_inds = [out_inds_s out_inds_m out_inds_l];
overlap_score = [overlap_score_s overlap_score_m overlap_score_l];
proj_area = [proj_area_s proj_area_m proj_area_l];
abs_err = [abs_err_s abs_err_m abs_err_l];
overlap = [overlap_s overlap_m overlap_l];
proj_bw = [proj_bw_s proj_bw_m proj_bw_l];
merged_bw_inds = [merged_bw_inds_s merged_bw_inds_m merged_bw_inds_l];
merge_mat = [merge_mat_s merge_mat_m merge_mat_l];
merged_bw_area = cellfun(@numel, merged_bw_inds);
merged_bw_area = merged_bw_area./prod(img_size);
matched_bplr_locs = [matched_bplr_locs_s matched_bplr_locs_m matched_bplr_locs_l];
shape_score = [shape_score_s(:)' shape_score_m(:)' shape_score_l(:)'];
proj_tran = proj_tran(:,out_inds);

t_shape_rank_sc= comp_time_s + comp_time_m + comp_time_l;
t_total = t_total + t_shape_rank_sc;

