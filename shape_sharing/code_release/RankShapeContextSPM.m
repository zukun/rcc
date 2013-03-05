function [shape_score_out overlap_score_out proj_area_out abs_err_out overlap_out out_inds_out proj_bw_out merged_bw_inds_out merge_mat_out matched_bplr_locs_out selected_inds_out] =...
    RankShapeContextSPM(overlap_score_src, proj_area_src, abs_err_src, overlap_src, out_inds_src, proj_bw_src, merged_bw_inds_src, merge_mat_src, merged_bw_area_src, matched_bplr_locs_src,...
    shape_contexts,...
    n_angle_bin, n_length_bin, img_size,...
    area_thresh_lb, area_thresh_ub,...
    target_rate,...
    rsz_min, rsz_max, n_int,...
    n_grid, use_all_shape)

grid_x = linspace(1,img_size(2)+1e-12, n_grid+1);
grid_y = linspace(1,img_size(1)+1e-12, n_grid+1);
bin_x = vl_binsearch(grid_x, double(matched_bplr_locs_src(1,:)));
bin_y = vl_binsearch(grid_y, double(matched_bplr_locs_src(2,:)));
bin_inds = sub2ind([n_grid n_grid], bin_y, bin_x);
n_max_bin = max(bin_inds);


overlap_score_out = [];
proj_area_out = [];
abs_err_out = [];
overlap_out = [];
out_inds_out = [];
proj_bw_out = cell(0);
merged_bw_inds_out = cell(0);
merge_mat_out = [];
matched_bplr_locs_out = [];
shape_score_out = [];
selected_inds_out = [];

for bin_idx = 1 : n_max_bin
    obj_inds = find(merged_bw_area_src >= area_thresh_lb & merged_bw_area_src < area_thresh_ub & bin_inds == bin_idx);
    merged_bw_inds = merged_bw_inds_src(obj_inds);
    merge_mat = merge_mat_src(:, obj_inds);
    overlap_score = overlap_score_src(obj_inds);
    proj_area = proj_area_src(obj_inds);
    abs_err = abs_err_src(:,obj_inds);
    overlap = overlap_src(:,obj_inds);
    out_inds = out_inds_src(obj_inds);
    proj_bw = proj_bw_src(obj_inds);
    matched_bplr_locs = matched_bplr_locs_src(:, obj_inds);

    n_proj = numel(merged_bw_inds);
    n_target = n_proj*target_rate;
    if ( n_proj == 0 )
        continue;
    end
    
    rate = exp(log(n_target/n_proj)/n_int);
    rszs = linspace(rsz_min,rsz_max,n_int);
    selected_inds = [];
    for iter = 1 : n_int
        n_proj = numel(overlap_score);
        if ( use_all_shape )
            shape_score = SCMatchScore(n_angle_bin, n_length_bin, merged_bw_inds, img_size, rszs(iter), shape_contexts, use_all_shape);
        else
            shape_score = SCMatchScore(n_angle_bin, n_length_bin, merged_bw_inds, img_size, rszs(iter), shape_contexts(:,obj_inds), use_all_shape);
        end
        [shape_score shape_rank] = sort(shape_score);
%         [overlap_score overlap_rank] = sort(overlap_score, 'descend');
%         ref = 1:n_proj;
%         overlap_rank_score = zeros(1,n_proj);
%         shape_rank_score = zeros(1,n_proj);
%         overlap_rank_score(overlap_rank) = ref;
%         shape_rank_score(shape_rank) = ref;
%         rank_score = overlap_rank_score + shape_rank_score;
%         [unused rr] = sort(rank_score);
        rr = shape_rank;
        cut_off = min(ceil(n_proj*(rate) + 1e-12), numel(rr));
        shape_score = shape_score(1:cut_off);
        inds = rr(1:cut_off);

        overlap_score = overlap_score(inds);
        proj_area = proj_area(inds);
        abs_err = abs_err(:,inds);
        overlap = overlap(:,inds);
        out_inds = out_inds(inds);
        proj_bw = proj_bw(inds);
        merged_bw_inds = merged_bw_inds(inds);
        merge_mat = merge_mat(:, inds);
        matched_bplr_locs = matched_bplr_locs(:, inds);
        selected_inds = [selected_inds obj_inds(inds)];

    end      
    
    overlap_score_out = [overlap_score_out overlap_score];
    proj_area_out = [proj_area_out proj_area];
    abs_err_out = [abs_err_out abs_err];
    overlap_out = [overlap_out overlap];
    out_inds_out = [out_inds_out out_inds];
    proj_bw_out = [proj_bw_out proj_bw];
    merged_bw_inds_out = [merged_bw_inds_out merged_bw_inds];
    merge_mat_out = [merge_mat_out merge_mat];
    matched_bplr_locs_out = [matched_bplr_locs_out matched_bplr_locs];
    shape_score_out = [shape_score_out shape_score(:)'];
    selected_inds_out = [selected_inds_out selected_inds];
end
