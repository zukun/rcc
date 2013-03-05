function SegmentationBSD(img_idx)
%-------------------------------------------------------------------------%
% segmentation for BSD 300 dataset
%-------------------------------------------------------------------------%
% img_idx : test image index in the BSD 300 dataset

img_path = './bsd/images/';
imgs = dir([img_path '/*.jpg']);
img_name = imgs(img_idx).name;
img_file = [img_path '/' img_name];

% segmentation
[masks, timing] = ComputeSegment(img_file);

% evaluation
[overlap_score, seg_inds obj_size] =...
    reportSegmentationResult(masks, img_idx, 'bsd');