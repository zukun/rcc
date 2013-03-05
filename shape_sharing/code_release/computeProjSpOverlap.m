function [overlap abs_err proj_area t_total] =...
    computeProjSpOverlap(src_mask, regions, matched_bplr_scales, matched_bplr_locs)
t_total = 0;

tic;
proj_scales = sqrt(matched_bplr_scales(1,:)./matched_bplr_scales(2,:));
proj_trans = matched_bplr_locs(1:2,:) - matched_bplr_locs(3:4,:).*repmat(proj_scales,2,1);
proj_scales = double(proj_scales);
proj_trans = double(proj_trans);
out_size = regions(1).img_size;
[pgy ngy proj_area] = ImageSimilarityTransformMex(src_mask, proj_scales, proj_trans, out_size);
t_proj = toc;
%disp(['projection: ' num2str(t_proj) ' sec']);
t_total = t_total + t_proj;

% overlap score
tic;
overlap = ComputeRegionOverlapMex(pgy, ngy, {regions.integral_y}, [regions.area]);
abs_err = (1-overlap).*repmat([regions.area]', 1, size(overlap,2));
t_overlap = toc;
%disp(['compute superpixel overlap: ' num2str(t_overlap) ' sec']);
t_total = t_total + t_overlap;

end