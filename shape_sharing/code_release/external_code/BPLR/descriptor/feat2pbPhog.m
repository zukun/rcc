function [phog feat_size feat_center feat_area_inds] = feat2pbPhog(pb_orient_, BPLR, phog_L, magnif, min_scale)
% input
% pb_orient_ : pb values at each orientation
% BPLR : BPLR data structure
% phog_L : pyramid level
% magnif : enlargement factor for feature region
% min_scale: min element scale
%
% output
% phog: phog descriptor
% feat_size: feature size (area)
% feat_center: feature centroid
% feat_area_inds: feature areas' indices in the image:
% e.g.,
% im = false(BPLR.img_size);
% im(feat_area_inds{i}) = true; --> this shows the region of i-th feature in the image.

disp('compute phog of each BPLR...');
% pixel indices in each BPLR
% bplr ptr inds
element_ptrs = BPLR.element_ptrs;
element_scales = magnif*BPLR.element_scales;
element_scales(element_scales < min_scale) = min_scale;
member_inds = {BPLR.feats.member_inds};
% phog 
bin = size(pb_orient_,3);
% resize image (if necessary)
if ( max(size(pb_orient_)) ~= max(BPLR.img_size) )
    pb_orient = zeros([BPLR.img_size, bin]);
    for i = 1 : bin
        pb_orient(:,:,i) = imresize(pb_orient_(:,:,i), BPLR.img_size, 'bilinear');
    end
else
    pb_orient = pb_orient_;
end

% compute phog
[phog feat_size feat_center feat_area_inds] = Feat2PHOGMex(pb_orient, element_ptrs, element_scales, member_inds, phog_L);

end


% function p = compute_phog(pb_orient, ptr_inds, lx,rx,ty,dy,L)
% 
% [row col bin] = size(pb_orient);
% n = col*row;
% n_ptr = numel(ptr_inds);
% offset = zeros(n_ptr*bin,1);
% for i = 1 : bin
%     offset((i-1)*n_ptr+1:i*n_ptr) = (i-1)*n;
% end
% pb_roi_ptr_inds = repmat(ptr_inds(:), bin,1) + offset;
% pb_roi_bw = false(size(pb_orient));
% pb_roi_bw(pb_roi_ptr_inds) = true;
% pb_roi = zeros([dy-ty+1, rx-lx+1, bin]);
% 
% pb_roi(pb_roi_bw(ty:dy, lx:rx, :)) = pb_orient(pb_roi_ptr_inds);
% 
% p = pbPhogDescriptor_(pb_roi,L); 
% end