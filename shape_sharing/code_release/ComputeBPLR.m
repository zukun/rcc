function feat = ComputeBPLR(im, ucm, gpb_orient, gpb_thin)

% BPLR parameters
order_k = 25; % topological scale
euc_f = 2.5; % euclidean scale
min_nseg = 20; % min # of segmentations in the segmnetation pool
max_nseg = 200; % max # of segmentations in the segmentation pool
grid_space = 10; % feature sample grid 
min_elem_scale = 4; % min radius of a single element circle

% phog parameters
phog_L = 2; % pyramid level
phog_magnif = 1.1; 

% extract BPLR
disp('Extract BPLR...');    
tic;
im_dim = max(size(im));
BPLR = detectBPLR(im, ucm, gpb_thin, order_k, euc_f, im_dim, min_nseg, max_nseg, grid_space, min_elem_scale);
bplr_t = toc;
disp([num2str(bplr_t) ' seconds for bplr']);

% gpb+pHOG descriptor for BPLR
tic;
[phog phog_scales phog_centers phog_area_inds] = feat2pbPhog(gpb_orient, BPLR, phog_L, phog_magnif, min_elem_scale);
phog_t = toc;
disp([num2str(phog_t) ' seconds for phog']);

desc.phog = single(phog);
desc.feat_scales = single(phog_scales);
desc.feat_centers = single(phog_centers);
desc.feat_area_inds = phog_area_inds; % just for annotation
        
feat.phog = desc.phog;
feat.scales = desc.feat_scales;
feat.centers = desc.feat_centers;
feat.order_k = order_k;
feat.euc_f = euc_f;

disp([num2str(bplr_t + phog_t) ' seconds in total']);