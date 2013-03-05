function BPLR = detectBPLR(im, ucm, contour_im, order_k, euc_f, im_dim, min_nseg, max_nseg, sample_grid_spacing, min_elem_scale)
% in : image, not used but for compatibility to stable version
% ucm : segmentation map
% contour_im : gpb map
% order_k : BPLR's topological scale (e.g., 25 hops)
% euc_f: BPLR's euclidean scale (e.g., 2.5)
% im_dim: longest image dim to be resized
% min_nseg: min number of segments in multiple segmentation
% max_nseg: max number of segments in multiple segmentation
% sample_grid_spacing: sample grid size (e.g., 4 pixels)
% min_elem_scale: minimum scale of elements

% parameters
min_feat_radius_sum = 100; % minimum scale of the BPLR (smaller BPLR than threshold will be discarded)
min_feat_scale = max([min_elem_scale, sample_grid_spacing/2]); %minimum scale of the sampled element
merge_loc_grid = sample_grid_spacing; 
%merge_scale_grid = 15*15; % pixels
%merge_scale_grid = 0.10; %percent

% get regions(i.e., segments) from ucm segmentation
regions = getRegionTree(ucm, min_nseg, max_nseg, im_dim);

% build spanning tree from the elements sampled at each region(i.e.,
% segment)
[ptrs edges scales] = createUniformSamplePtrsFromDT_(regions, im, ucm, contour_im, order_k, sample_grid_spacing);

% extract BPLRs from the spanning tree
feats = buildAdpativeSizeFeature_(ptrs, edges, scales, order_k, euc_f, regions(1).img_size, min_feat_scale, min_feat_radius_sum);

BPLR.feats = feats;
BPLR.order = order_k;
BPLR.euc_scale = euc_f;
BPLR.sample_grid_size = sample_grid_spacing;
BPLR.min_elem_scale = min_feat_scale;
BPLR.img_size = regions(1).img_size;
BPLR.element_ptrs = ptrs'; % 2xN
BPLR.element_scales = max(scales, min_feat_scale);
BPLR.element_scales = BPLR.element_scales';
disp([num2str(numel(BPLR.feats)) ' BPLRs initially obtained']);

% post-processing: merge similar BPLRs
disp('merge near-duplicate BPLRs...');
feat_size = arrayfun(@(x)(sum(x.member_scales)), BPLR.feats);
[feat_size size_rank] = sort(feat_size);
BPLR.feats = BPLR.feats(size_rank); %to pick the largest feature in a sub-region 
merge_feat_inds = mergeFeats(BPLR, merge_loc_grid);
BPLR.feats = BPLR.feats(merge_feat_inds);
disp([num2str(numel(BPLR.feats)) ' BPLRs finally obtained']);

