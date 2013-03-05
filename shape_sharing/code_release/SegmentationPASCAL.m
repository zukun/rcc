function SegmentationPASCAL(img_idx)
%-------------------------------------------------------------------------%
% segmentation for PASCAL 2010 dataset
%-------------------------------------------------------------------------%
% img_idx : test image index in the PASCAL 2010 validation set

VOCinit

imgset = 'val';
ids=textread(sprintf(VOCopts.seg.imgsetpath,imgset),'%s');
img_path = sprintf(VOCopts.imgpath,ids{img_idx});

% segmentation
[masks, timing] = ComputeSegment(img_path);

% evaluation
[overlap_score, seg_inds obj_size] =...
    reportSegmentationResult(masks, img_idx, 'pascal');