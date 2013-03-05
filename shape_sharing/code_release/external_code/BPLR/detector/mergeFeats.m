function merge_feat_inds = mergeFeats(BPLR, quantizer)


%feat_size = arrayfun(@(x)(sum(x.member_scales)), feats.core_feats);
%feat_size = floor(feat_size./quantizer);
% center_ptrs = [feats.core_feats.center_ptr];
% feat_id = [center_ptrs;bbox];
% [unused merge_feat_inds] = unique(feat_id', 'rows');

% original version
% bbox = [feats.core_feats.bbox];
% bbox = floor(bbox./quantizer);
% center_ptrs = [feats.core_feats.center_ptr];
% feat_id = [center_ptrs;bbox];
% [unused merge_feat_inds] = unique(feat_id', 'rows');

ref_ptrs = [BPLR.feats.ref_ptr];
ref_ptrs = floor(ref_ptrs./quantizer);
scales = [BPLR.feats.magnif];
%bbox = [feats.core_feats.bbox];
%bbox = floor(bbox./quantizer);
%feat_id = [ref_ptrs;bbox];
feat_id = [ref_ptrs; scales(:)'];
[unused merge_feat_inds] = unique(feat_id', 'rows');


    
    