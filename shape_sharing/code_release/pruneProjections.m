function tf =...
    pruneProjections(overlap_score, merge_mat, region_area, img_size, merged_bw_inds, overlap_thresh, matched_bplr_locs, abs_err, abs_err_thresh)

n_proj = numel(overlap_score);

% bplr must be inside the merged region
matched_bplr_locs = ceil(matched_bplr_locs);
bplr_inds = sub2ind(img_size, matched_bplr_locs(2,:), matched_bplr_locs(1,:));
tf_bplr = arrayfun(@(i)(ismember(bplr_inds(i), merged_bw_inds{i})), 1:n_proj);

% absoulute error must be small
%abs_err_proj = sum(abs_err.*merge_mat,1);
%tf_abs_err = abs_err_proj < abs_err_thresh*prod(img_size);

% higher rankers prune out lower ones of large overlap
area_vec = merge_mat.*repmat(region_area(:), 1, n_proj);
d = slmetric_pw(area_vec, area_vec, 'cityblk');
[unused overlap_rank] = sort(overlap_score,'descend');
tf_overlap = arrayfun(@(i)(~nullLowerRankers(overlap_rank, d, i, sum(area_vec(:,i))+1e-12, overlap_thresh)),...
    1:n_proj, 'uniformoutput', false);
tf_overlap = cell2mat(tf_overlap');
[jj ii] = find(~tf_overlap);
ii = unique(ii);
tf_overlap = true(1, n_proj);
tf_overlap(ii) = false;

%tf = tf_bplr & tf_abs_err & tf_overlap;
tf = tf_bplr & tf_overlap;

end

function tf = nullLowerRankers(overlap_rank, d, idx, idx_area, thresh)
n_proj = numel(overlap_rank);
my_rank = find(overlap_rank == idx);
candid = overlap_rank(my_rank+1:end);
null_tf = d(idx,candid)./idx_area < thresh;
tf = false(1,n_proj);
tf(candid(null_tf)) = true;
end

