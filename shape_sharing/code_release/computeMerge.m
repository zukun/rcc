function [proj_bw merged_bw_inds overlap overlap_score abs_err merge_mat proj_trans proj_area out_inds t_total] =...
computeMerge(src_mask, src_locs, regions, overlap, abs_err, proj_area, overlap_thresh, do_adjust)
t_total = 0;

% initial merge
tic;
out_size = regions(1).img_size;
img_area = prod(out_size);

n_proj = size(overlap,2);
n_region = numel(regions);

region_area = [regions.area];
if ( do_adjust)
    tf1 = repmat(region_area(:),1,n_proj) < repmat(proj_area(:)', n_region, 1);
    tf2 = abs_err < 0.25*repmat(proj_area(:)', n_region, 1);
    tf3 = abs_err < 0.05*img_area;
    tf4 = repmat(proj_area(:)', n_region, 1) < 0.75*img_area;
    tf = tf1 & tf2 & tf3 & tf4;
else 
    tf = true(size(overlap));
end
overlap(~tf) = 0;

merge_mat = overlap > overlap_thresh;

[merged_bw_inds merged_bw_scales merged_bw_locs merged_bw_bbox] =...
    MergeRegionsMex({regions.segment}, merge_mat, out_size);
t_merge = toc;
t_total = t_total + t_merge;

if ( do_adjust ) 
    % adjustment
    tic;
    [pgy ngy proj_area proj_bw proj_trans] = ImageResizeMex(src_mask, merged_bw_bbox, src_locs, merged_bw_locs, out_size);
    

    % overlap score
    overlap = ComputeRegionOverlapMex(pgy, ngy, {regions.integral_y}, [regions.area]);
    abs_err = (1-overlap).*repmat([regions.area]', 1, size(overlap,2));

    n_region = numel(regions);
    region_area = [regions.area];
    tf1 = repmat(region_area(:),1,n_proj) < repmat(proj_area(:)', n_region, 1);
    tf2 = abs_err < 0.25*repmat(proj_area(:)', n_region, 1);
    tf3 = abs_err < 0.05*img_area;
    tf4 = repmat(proj_area(:)', n_region, 1) < 0.75*img_area;
    tf = tf1 & tf2 & tf3 & tf4;
    overlap(~tf) = 0;
    %%
    merge_mat = overlap > overlap_thresh;
    merged_bw_inds =...
       MergeRegionsMex({regions.segment}, merge_mat, out_size);
    %%

    t_adjust = toc;
    t_total = t_total + t_adjust;
else
    tic;
    [pgy ngy proj_area proj_bw proj_trans] = ImageResizeMex(src_mask, merged_bw_bbox, src_locs, merged_bw_locs, out_size);
    %[pgy ngy proj_area proj_bw] = ImageSimilarityTransformMex(src_mask, ones(1,numel(src_mask)), zeros(2,numel(src_mask)), out_size);
    % overlap score
    overlap = ComputeRegionOverlapMex(pgy, ngy, {regions.integral_y}, [regions.area]);
    abs_err = (1-overlap).*repmat([regions.area]', 1, size(overlap,2));
    t_adjust = toc;
    t_total = t_total + t_adjust;
end

tic;
overlap_score =...
    ComputeOverlapScoreMex(pgy, ngy, proj_area, merged_bw_inds, out_size);    
out_inds = 1:numel(overlap_score);
t_region_overlap = toc;
t_total = t_total + t_region_overlap;

% remove duplicates
% [overlap_score rr] = sort(overlap_score, 'descend');
% proj_bw = proj_bw(rr);
% merged_bw_inds = merged_bw_inds(rr);
% overlap = overlap(:, rr);
% abs_err = abs_err(:, rr);
% proj_area = proj_area(rr);
% out_inds = rr;
% merge_mat = merge_mat(:,rr);

% [merge_mat uid] = unique(merge_mat','rows', 'first');
% merge_mat = merge_mat';
% overlap_score = overlap_score(uid);
% proj_bw = proj_bw(uid);
% merged_bw_inds = merged_bw_inds(uid);
% overlap = overlap(:,uid);
% abs_err = abs_err(:,uid);
% proj_area = proj_area(uid);
% out_inds = out_inds(uid);
end