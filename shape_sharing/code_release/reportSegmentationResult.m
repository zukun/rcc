function [overlap_score, seg_inds, obj_size] =...
    reportSegmentationResult(segments, img_idx, db_name) 
% anaylsis
seg_bw_inds =  arrayfun(@(i)(uint32(find(segments(:,:,i)))), 1:size(segments,3), 'uniformoutput',false);
    
if ( strcmp(db_name, 'pascal') )
    [overlap_score, seg_inds, obj_size] = evaluation_pascal(img_idx,seg_bw_inds);
elseif ( strcmp(db_name, 'bsd') ) 
    [overlap_score, seg_inds, obj_size] = evaluation_bsds(img_idx,seg_bw_inds);
end