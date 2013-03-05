function [BPLR, desc1, desc2] = demo(img_file, rsz, order_k, euc_f, min_nseg, max_nseg, grid_space, min_elem_scale, magnif, phog_L, n_angle_bin, n_length_bin)
% This demo contains routines for BPLR extraction and descriptor(PHOG) computation.
% Once BPLRs are extracted, users can represent them by any existing
% descriptors; for convenience, we also provide codes for some descriptors: PHOG, chordiogram and color historam.

% input
% Parameters for extracting BPLR
% img_file: input image file's path
% rsz: image resize factor (0-1)
% order_k: topology neighbor threshold (up to order_k hops in the graph)
% euc_f: euclidean neighbor threshold (up to euc_f times the scale of a
% reference)
% min_nseg: minumum number of segments in the multiple segmentations
% max_nseg: maximum number of segments in the multiple segmentations
% --> Among a pool of multiple segmentations, segmentations whose number of
% segments are in between min_nseg and max_nseg are chosen.
% grid_space: cell size to sample an element (a cell = grid_space by
% grid_space pixels)
% min_elem_scale: minimum size of an element

% Parameters for all descriptors
% magnif: magnifying factor of original feature

% Parameters of PHOG descriptor
% phog_L: pyramid level of PHOG descriptor 
% (level 0: entire image, level 1: 2 by 2, level 2: 4 by 4, and so on.) 

% Parameters of chordiogram descriptor
% n_angle_bin: the number of angular bins
% n_length_bin: the number of spatial bins

% output
% BPLR: a stucture of storing BPLR information
% BPLR.feats: struct array: BPLR.feats(i) --> the i-th BPLR
    % BPLR.feats(i).bbox: BPLR's boundbox coordinate: [lx ty rx dy]
    % BPLR.feats(i).bbox_size: BPLR's boundbox size
    % BPLR.feats(i).magnif: BPLR's euclidean neighbor threshold (this has
    % nothing to do with input parameter "magnif."
    % BPLR.feats(i).member_inds: indices of member elements which belong to the (i-th) BPLR
    % BPLR.feats(i).member_scales: scales of member elements
    % BPLR.feats(i).n_member: the number of member elements
    % BPLR.feats(i).ref_ptr: reference element's image coordinate
    % BPLR.feats(i).ref_scale: reference element's scale
% BPLR.order: topology neighbor threshold (same as the input parameter "order_k")
% BPLR.euf_scale: Euclidean neighbor threshold (same as the input parameter "euc_f")
% BPLR.sample_grid_size: cell size (same as the input parameter "grid_space")
% BPLR.min_elem_scale: minimum element scale (same as the input parameter "min_elem_scale")
% BPLR.img_size: image size from which BPLRs are extracted. (if the image
% is resized, the size of resized image is stored.)
% BPLR.element_ptrs: sampled elements' image coordinates
% BPLR.element_scales: sampled elements' scales
%
% desc: a structure of strong descriptors
% desc.feat_vecs: feature vector, desc.feat_vecs(:,i) = feature vector of
% the i-th BPLR
% desc.feat_scales: feature scale, desc.feat_scales(:,i) = scale (i.e., region
% size) of the i-th BPLR
% desc.feat_centers: feature location, desc.feat_centers(:,i) = image
% coordinate of the i-th BPLR's center.

if  (nargin == 1) % default parameters
    rsz = 1.0;
    order_k = 25;
    euc_f = 2.5;
    min_nseg = 20;
    max_nseg = 200;
    grid_space = 8;
    min_elem_scale = 4.0;
    
    magnif = 1.1;
    phog_L = 2;
    n_angle_bin = 12;
    n_length_bin = 20;
end

% 0. image resize
im = imread(img_file);
im = imresize(im ,rsz);
[img_path,img_name,img_ext] = fileparts(img_file);
img_file_rsz = [img_path '/' img_name '_rsz_' num2str(rsz) img_ext];
imwrite(im, img_file_rsz);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Pre-processing: Berkeley segmentation

% 1. compute globalPb
% gpb_out_file = [img_file '_gPb_rsz_' num2str(rsz) ' .mat'];
gpb_out_file = [img_file '_gPb.mat'];
if ( ~exist(gpb_out_file, 'file') )
    disp('Compute gPb...');
    [gPb_orient gPb_thin] = globalPb(img_file_rsz, gpb_out_file, 1.0);
else
    load(gpb_out_file, 'gPb_orient', 'gPb_thin');
end

% 2. compute Hierarchical Regions
% ucm_out_file = [img_file '_ucm_rsz_' num2str(rsz) '.mat'];
ucm_out_file = [img_file '_ucm.mat'];
if ( ~exist(ucm_out_file, 'file') )
    ucm = contours2ucm(gPb_orient, 'imageSize');
    save(ucm_out_file, 'ucm');
else
    load(ucm_out_file, 'ucm');
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% BPLR extraction

% 3. extract BPLR    
disp('Extract BPLR...');    
tic;
im_dim = max(size(im));
BPLR = detectBPLR(im, ucm, gPb_thin, order_k, euc_f, im_dim, min_nseg, max_nseg, grid_space, min_elem_scale);
bplr_t = toc;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Examples of descriptor computation.

% 4. gpb+pHOG descriptor for BPLR
tic;
[phog phog_scales phog_centers] = feat2pbPhog(gPb_orient, BPLR, phog_L, magnif, min_elem_scale);
phog = single(phog); %type conversion for saving space
feat_scales = single(phog_scales);
feat_centers = single(phog_centers);
desc1.feat_vecs = phog;
desc1.feat_scales = feat_scales;
desc1.feat_centers = feat_centers;
desc1_t = toc;

% 4. chordiogram descriptor for BPLR
% tic;
% [chordiogram chordiogram_scales chordiogram_centers] = feat2Chordiogram(BPLR, magnif, min_elem_scale, n_angle_bin, n_length_bin);
% chordiogram = single(chordiogram);
% feat_scales = single(chordiogram_scales);
% feat_centers = single(chordiogram_centers);
% desc.feat_vecs = chordiogram;
% desc.feat_scales = feat_scales;
% desc.feat_centers = feat_centers;
% desc_t = toc;

% 4. color histogram descriptor for BPLR
tic;
[color_hist feat_scales feat_centers] = feat2ColorHist(im, BPLR, magnif, min_elem_scale);
color_hist = single(color_hist);
feat_scales = single(feat_scales);
feat_centers = single(feat_centers);
desc2.feat_vecs = color_hist;
desc2.feat_scales = feat_scales;
desc2.feat_centers = feat_centers;
desc2_t = toc;

disp([num2str(bplr_t) ' seconds for BPLR extraction']);
disp([num2str(desc1_t) ' seconds for phog descriptor']);
disp([num2str(desc2_t) ' seconds for colorhist descriptor']);
disp([num2str(bplr_t + desc1_t + desc2_t) ' seconds in total']);
        

