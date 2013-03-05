function [max_overlap, max_overlap_inds obj_size] = evaluation_pascal(img_idx, merged_bw_inds)

% evaluation
VOCinit;
imgset = 'val';
ids=textread(sprintf(VOCopts.seg.imgsetpath,imgset),'%s');

% gt path
anno_path = sprintf(VOCopts.annopath,ids{img_idx});
seg_path = sprintf(VOCopts.seg.instimgpath,ids{img_idx});
rec = PASreadrecord(anno_path);

% get object segmentations
seg_img = imread(seg_path);
n_obj = numel(rec.objects);
gt_overlap_score = zeros(n_obj, numel(merged_bw_inds));
max_overlap = cell(1, n_obj);
max_overlap_inds = cell(1, n_obj);
care = seg_img ~= 255;
obj_size = zeros(1,n_obj);
img_size = numel(seg_img);
for i = 1 : n_obj
    obj_gt = seg_img == i;
    obj_size(i) = sum(obj_gt(:));
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
    disp(['obj ' num2str(k) ' overlap: ' num2str(score) ' size: ' num2str(obj_size(k)/prod(img_size))]);
end
