function [cluster_id overlap_score] = groupSegments(segments, seg_scores, img_size, n_target, min_overlap)

n_seg = size(segments,2);
seg_size = sum(segments,1)./prod(img_size);

% overlap score
segments = reshape(segments, img_size(1),img_size(2),n_seg);
segm = imresize(segments, 0.4, 'nearest');
segm = reshape(segm, size(segm,1)*size(segm,2), n_seg);
pixel_overlap = segm_overlap_mex(segm);       
clear segm

% grouping
[unused r] = sort(seg_scores,'descend');
n_sample = ceil(n_seg/n_target);
cluster_id = cell(n_target,1);
selected = false(n_seg,1);
cluster_cnt = 1;
seg_cnt = 1;
while (true)

    idx = r(seg_cnt);
    seg_cnt = seg_cnt + 1;
    
    
    if ( ~selected(idx) ) 

        [pov inds] = sort(pixel_overlap(idx,:), 'descend');
%         if (seg_size(idx) > 0.025 )
%             overlap_thresh = min_overlap(1);
%         else
%             overlap_thresh = min_overlap(2);
%         end
        
        tf = pov(1:n_sample) > min_overlap;
    
        
        neighbor = inds(1:n_sample);
        neighbor = neighbor(tf);
        
        % absolute error
        err = zeros(1,numel(neighbor));
        for i = 2 : numel(neighbor)
            xor_err = xor(segments(:,:,neighbor(1)), segments(:,:,neighbor(i)));
            err(i) = sum(xor_err(:));
        end
        tf = err < 0.05*prod(img_size);
        neighbor = neighbor(tf);      
        
       
        cluster_id{cluster_cnt} = neighbor;

        if ( numel(neighbor) > 1 )
            overlap_score(cluster_cnt) = mean(pixel_overlap(idx,neighbor(2:end)));
        else
            overlap_score(cluster_cnt) = 1;
        end

        selected(neighbor) = true;

        
        cluster_cnt = cluster_cnt + 1;
        
    end
    
    if ( cluster_cnt > n_target || seg_cnt > n_seg)
        break;
    end
end



