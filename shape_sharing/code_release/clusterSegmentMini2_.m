function [sel cluster_ids] = clusterSegmentMini2_(pixel_overlap, segment_score, overlap_thresh)

n_seg = numel(segment_score);
[unused r] = sort(segment_score, 'descend');

checked = false(n_seg,1);
sel = [];
cluster_ids = zeros(n_seg,1);
cluster_cnt = 0;
for i = 1 : n_seg
    idx = r(i);
    if ( ~checked(idx) ) 
        sel = [sel;idx];
        
%         if ( i < 0.1*n_seg )
%             thresh = overlap_thresh + 0.075;
%         else
%             thresh = overlap_thresh;
%         end
        thresh = overlap_thresh;
        
        checked(pixel_overlap(idx,:) > thresh) = true;
        cluster_cnt = cluster_cnt + 1;
        cluster_ids(pixel_overlap(idx,:) > thresh) = cluster_cnt;        
    end
end
