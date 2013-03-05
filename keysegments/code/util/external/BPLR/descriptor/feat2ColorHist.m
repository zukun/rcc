function [color_hist feat_size feat_center feat_area_inds] = feat2ColorHist(im, BPLR, magnif, min_scale)

disp('compute color histogram of each BPLR...');

% pixel indices in each BPLR
img_size = BPLR.img_size;
element_ptrs = BPLR.element_ptrs;
element_scales = magnif*BPLR.element_scales;
element_scales(element_scales < min_scale) = min_scale;
member_inds = {BPLR.feats.member_inds};
feat_mask_imgs = BPLRMaskImgMex(element_ptrs, element_scales, member_inds, img_size);
feat_area_inds = cellfun(@find, feat_mask_imgs, 'uniformoutput', false);
[yy xx] = cellfun(@find, feat_mask_imgs, 'uniformoutput', false);
cx = cellfun(@mean, xx);
cy = cellfun(@mean, yy);
feat_center = [cx(:)';cy(:)'];

% color histogram parameters
numColorHistBins = 22;
maxLValue = 100;
LBinSize = maxLValue / numColorHistBins;
abBinSize = 256 / numColorHistBins;
LBinEdges = 0:LBinSize:100;
abBinEdges = -128:abBinSize:128;

% color conversion
% if ( max(size(im)) ~= max(size(BPLR.im_dim)) )
%     im = imresize(im, BPLR.im_dim);
% end
if ( max(size(im)) ~= max(size(BPLR.img_size)) )
    im = imresize(im, BPLR.img_size);
end

if ( ndims(im) == 2 )
    im = repmat(im,[1,1,3]);
end
[L,A,B] = RGB2Lab(double(im(:,:,1)), double(im(:,:,2)), double(im(:,:,3)));
if(max(L(:)) > 100 || min(L(:)) < 0)
    disp('error in L range');
end
if(max(A(:)) > 128 || min(A(:)) < -128)
    disp('error in a range');
end
if(max(B(:)) > 128 || min(B(:)) < -128)
    disp('error in b range\n');
end

% color hist

    
color_hist = cellfun(@(x)(compute_colorhist(L,A,B,LBinEdges,abBinEdges,x)),...
             feat_area_inds, 'uniformoutput', false);
color_hist = cell2mat(color_hist);
%feat_size = cell2mat(feat_size);
feat_size = cellfun(@numel, feat_area_inds);

end


function color_hist = compute_colorhist(L, A, B, LBinEdges, abBinEdges, ptr_inds)
color_hist =...
    [histc(L(ptr_inds(:)), LBinEdges);...
     histc(A(ptr_inds(:)), abBinEdges);...
     histc(B(ptr_inds(:)), abBinEdges)];
color_hist = color_hist./sum(color_hist);

end