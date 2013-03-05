function [matched_img_inds, matched_obj_inds, matched_bplr_scales, matched_bplr_locs, test_indices] =...
    BPLRMatch(im, ucm, gpb_orient, gpb_thin, n_max_feat, fg_k, bg_k)

%% exmemplar
% flann data
data_file = 'data/2010_1_flann_data.mat';
load(data_file, 'fg_phog', 'bg_phog', 'obj_inds', 'img_inds', 'locs', 'scales');
train_obj_inds = obj_inds;
train_img_inds = img_inds;
train_bplr_locs = locs;
train_bplr_scales = scales;
fg_train_phogs = fg_phog;
bg_train_phogs = bg_phog;
clear fg_phog bg_phog obj_inds img_inds locs scales

% load flann index (phog)
fg_phog_index_file = 'data/2010_1_fg_phog_flann_index.ind';
fg_phog_params_file = 'data/2010_1_fg_phog_flann_params.mat';
bg_phog_index_file = 'data/2010_1_bg_phog_flann_index.ind';
bg_phog_params_file = 'data/2010_1_bg_phog_flann_params.mat';
fg_phog_flann_index = flann_load_index(fg_phog_index_file, fg_train_phogs);
load(fg_phog_params_file, 'params');
fg_phog_flann_params = params;
bg_phog_flann_index = flann_load_index(bg_phog_index_file, bg_train_phogs);
load(bg_phog_params_file, 'params');
bg_phog_flann_params = params;

%% test
feat = ComputeBPLR(im, ucm, gpb_orient, gpb_thin);
test_phog = feat.phog;
test_bplr_locs = feat.centers;
test_bplr_scales = feat.scales;
clear feat
    
%% shape match
% fg match
n_test_feat = size(test_phog,2);
[unused fg_dist] = flann_search(fg_phog_flann_index, test_phog, 1, fg_phog_flann_params);
% bg match
[unused bg_dist] = flann_search(bg_phog_flann_index, test_phog, 1, bg_phog_flann_params);

% final match
fg_inds = find(fg_dist < bg_dist);
bg_inds = setdiff(1:n_test_feat, fg_inds);
n_fg = numel(fg_inds);
n_bg = numel(bg_inds);
%base_n = ceil(n_max_feat/(fg_k*n_fg + n_bg));
blow_up = 1;
knn = blow_up*max(fg_k, bg_k);
indices = flann_search(fg_phog_flann_index, test_phog, knn, fg_phog_flann_params);

% take exemplars
fg_test_indices = repmat(fg_inds, knn,1)';
fg_test_indices = fg_test_indices(:)';
fg_train_indices = indices(:, fg_inds)';
fg_train_indices = fg_train_indices(:)';
fg_test_indices = fg_test_indices(1:n_fg*fg_k);
fg_train_indices = fg_train_indices(1:n_fg*fg_k);

bg_test_indices = repmat(bg_inds, knn,1)';
bg_test_indices = bg_test_indices(:)';
bg_train_indices = indices(:, bg_inds)';
bg_train_indices = bg_train_indices(:)';
bg_test_indices = bg_test_indices(1:n_bg*bg_k);
bg_train_indices = bg_train_indices(1:n_bg*bg_k);

test_indices = [fg_test_indices bg_test_indices];
match_indices = [fg_train_indices bg_train_indices];

matched_img_inds = train_img_inds(match_indices);
matched_obj_inds = train_obj_inds(match_indices);
matched_bplr_scales = [test_bplr_scales(test_indices); train_bplr_scales(match_indices)];
matched_bplr_locs = [test_bplr_locs(:,test_indices);train_bplr_locs(:,match_indices)];

% remove duplicate
id = [matched_bplr_scales ; matched_bplr_locs]';
[unused uid] = unique(id, 'rows');
uid = sort(uid);
matched_img_inds = matched_img_inds(uid);
matched_obj_inds = matched_obj_inds(uid);
matched_bplr_scales = matched_bplr_scales(:,uid);
matched_bplr_locs = matched_bplr_locs(:,uid);
test_indices = test_indices(uid);

% cut by max_feat
if (n_max_feat < numel(matched_img_inds) )
    matched_img_inds = matched_img_inds(1:n_max_feat);
    matched_obj_inds = matched_obj_inds(1:n_max_feat);
    matched_bplr_scales = matched_bplr_scales(:,1:n_max_feat);
    matched_bplr_locs = matched_bplr_locs(:,1:n_max_feat);
    test_indices = test_indices(1:n_max_feat);
end    

% deallocate
flann_free_index(fg_phog_flann_index);
flann_free_index(bg_phog_flann_index);

end





    
    

    
    
    