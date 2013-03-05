function [cluster_id cluster_cohesion seg_score] = groupSegmentWrapper(merge_segments, gPb_thin, n_target, min_overlap)

% parameter
img_size = size(gPb_thin);


% score
disp('scoring')
% cut score
e = gPb_thin > 0.0;
[im_dt im_dtL] = bwdist(e);
[merge_cut_score] =...
    computeCutScore2(merge_segments, im_dt, im_dtL, gPb_thin, img_size);
bdry_tf = pickBdrySegments(merge_segments, img_size);
n_merge = size(merge_segments,2);
merge_cut_score_ = zeros(n_merge,1);
merge_cut_score_(bdry_tf) = merge_cut_score(bdry_tf,2);
merge_cut_score_(~bdry_tf) = merge_cut_score(~bdry_tf,1);
seg_score = merge_cut_score_;

disp('clustering');
[cluster_id cluster_cohesion] = groupSegments(merge_segments, seg_score, img_size, n_target, min_overlap);
tf = cellfun(@isempty, cluster_id);
cluster_id = cluster_id(~tf);
cluster_cohesion = cluster_cohesion(~tf);

% add very large ones into cluster
try
    in_cluster_seg_inds = unique(cell2mat(cluster_id'));
catch
    in_cluster_seg_inds = unique(cell2mat(cluster_id));
end
merge_size = sum(merge_segments,1)./prod(img_size);
large_inds = setdiff(find(merge_size > 0.5), in_cluster_seg_inds);
for i = 1 : numel(large_inds)
    cluster_id{end+1} = large_inds(i);
end
try
    in_cluster_seg_inds = unique(cell2mat(cluster_id'));
catch
    in_cluster_seg_inds = unique(cell2mat(cluster_id));
end
disp(['n_cluster:        ' num2str(numel(cluster_id))]);
disp(['n_seg in cluster: ' num2str(numel(in_cluster_seg_inds))]);
