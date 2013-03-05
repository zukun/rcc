function [cut_score val_bdry img_bdry] = computeCutScore2(segments, im_dt, im_dtL, gPb_thin, img_size)
% compute cut score
% cut_score(:,1) --> with img boundary
% cut_score(:,2) --> without img boundary (for segments in image
% peripheral)

n_seg = size(segments,2);
segments = reshape(segments, [img_size,n_seg]);
dt_error_thresh = 2;
cut_score = zeros(n_seg,2);
val_bdry = zeros(n_seg,1);
img_bdry = zeros(n_seg,1);

gPb_thin_local = gPb_thin;

for i = 1 : n_seg
    bw = segments(:,:,i);
    bwc = bwperim(bw);
    
%     [y x] = find(bwc);
%     lx = min(x);
%     rx = max(x);
%     ty = min(y);
%     dy = max(y);
%     g = gPb_thin(ty:dy,lx:rx);
%     m = mean(g(g>0));
%     gPb_thin_local = gPb_thin./(m + 1e-10);
    
    p = sum(bwc(1,:)) + sum(bwc(end,:)) + sum(bwc(:,1)) + sum(bwc(:,end));
    img_bdry(i) = p/(sum(bwc(:)) + 1e-10);
    
    % valid bdry
    bwc_ind = find(bwc);
    n_bdry = numel(bwc_ind);    
    bwc_ind(im_dt(bwc)>dt_error_thresh) = [];
    bwc_val = false(img_size);
    bwc_val(bwc_ind) = true;
    n_val_bdry = numel(bwc_ind);
    
    
    % avg. contour strength
    cut_score(i,1) = sum(gPb_thin_local(im_dtL(bwc_val)))/(n_bdry+1e-10);
    val_bdry(i) = n_val_bdry/n_bdry;
    
    % remove image bdry
    bwc(1,1:end) = 0;
    bwc(end,1:end) = 0;
    bwc(1:end,1) = 0;
    bwc(1:end,end) = 0;
    bwc_ind = find(bwc);
    n_bdry = numel(bwc_ind);    
    bwc_ind(im_dt(bwc)>dt_error_thresh) = [];
    bwc_val = false(img_size);
    bwc_val(bwc_ind) = true;
    % avg. contour strength
    cut_score(i,2) = sum(gPb_thin_local(im_dtL(bwc_val)))/(n_bdry+1e-10);    
   
end
