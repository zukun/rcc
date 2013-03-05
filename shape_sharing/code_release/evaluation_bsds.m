function [max_overlap, max_overlap_inds obj_size] = evaluation_bsds(img_idx, merged_bw_inds)

% img path
img_path = './bsd/images/';
imgs = dir([img_path '/*.jpg']);
[f_path, f_name] = fileparts(imgs(img_idx).name);

% gt path
gt_path = './bsd/annotations/';

% get object segmentations
seg_img = imread([gt_path '/' f_name '.png']);
n_obj = max(seg_img(:));
gt_overlap_score = zeros(n_obj, numel(merged_bw_inds));
max_overlap = cell(1, n_obj);
max_overlap_inds = cell(1, n_obj);
care = seg_img ~= 255;
obj_size = zeros(1,n_obj);

for i = 1 : n_obj
    obj_gt = seg_img == i;
    obj_size(i) = sum(obj_gt(:));
    gt_overlap_score_ = zeros(1,numel(merged_bw_inds));
    for j = 1 : numel(merged_bw_inds)
        bw = false(size(seg_img));
        bw(merged_bw_inds{j}) = true;
        gt_overlap_score_(j) = sum(sum(bw & obj_gt & care))/sum(sum((bw | obj_gt) & care));
    end
        
    [max_overlap{i} max_overlap_inds{i}] = sort(gt_overlap_score_, 'descend');
    gt_overlap_score(i,:) = gt_overlap_score_(:)';
end

for k = 1 : n_obj
    top_k = min(1, numel(max_overlap{k}));
    score = max_overlap{k}(1:top_k);
    score = score(:)';
    disp(['obj ' num2str(k) ' overlap: ' num2str(score)]);
end
