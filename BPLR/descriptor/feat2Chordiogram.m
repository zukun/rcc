function [chordiogram feat_size feat_center feat_area_inds] = feat2Chordiogram(BPLR, magnif, min_scale, n_angle_bin, n_length_bin)
% This computes chordiograms of BPLRs.
% Chordiogram is introduced by "Object Detection via Boundary
% Structure Segmentation", Alexander Toshev et.al., CVPR 2010
%
% input
% BPLR : BPLR data structure
% magnif : enlargement factor for feature region
% min_scale: min element scale
% n_angle_bin: # of angle bin 
% n_length_bin: # of length bin
%
% output
% chordiogram: chordiogram descriptor
% feat_size: feature size (area)
% feat_center: feature centroid
% feat_area_inds: feature areas' indices in the image:
% e.g.,
% im = false(BPLR.img_size);
% im(feat_area_inds{i}) = true; --> this shows the region of i-th feature in the image.



disp('compute chordiogram of each BPLR...');

img_size = BPLR.img_size;
element_ptrs = BPLR.element_ptrs;
element_scales = magnif*BPLR.element_scales;
element_scales(element_scales < min_scale) = min_scale;
member_inds = {BPLR.feats.member_inds};
angle_bin1 = linspace(-2*pi,2*pi+1e-12,2*n_angle_bin+1);
angle_bin2 = linspace(0,2*pi+1e-12,n_angle_bin+1);
l_bin = logspace(-1,log10(5),n_length_bin+1);
length_bin = zeros(1, numel(l_bin)+2);
length_bin(1) = -1;
length_bin(2:end-1) = l_bin;
length_bin(end) = 1e6;

[chordiogram feat_size feat_center feat_area_inds] =...
    Feat2ChordiogramMex(element_ptrs, element_scales, member_inds, img_size, angle_bin1, angle_bin2, length_bin);
s = sum(chordiogram,1);
chordiogram = chordiogram./repmat(s,size(chordiogram,1),1);

end


