function [fg_shape_prior_cost, shape_prior, bbox] =...
    computeShapePriorCost(segments, cluster_id, lambda, bbox_margin, img_size)

% paramter
n_cluster = numel(cluster_id);
n_seg = size(segments,2);
segments = reshape(segments, [img_size, n_seg]);

fg_shape_prior_cost = cell(n_cluster,1);
shape_prior = false(prod(img_size), n_cluster);
bbox = zeros(4,n_cluster);
for i = 1 : n_cluster
    seg_inds = cluster_id{i};
    
    % region
    ubw = false(img_size);
    cbw = true(img_size);
    for j = 1 : numel(seg_inds)
        idx = seg_inds(j);
        % union
        ubw(segments(:,:,idx)) = true;
        % common
        cbw = cbw & segments(:,:,idx);
    end
    
    fg_cost = 0.5*ones(img_size);
    fg_cost(~ubw) = 0.5 + lambda;
    fg_cost(cbw) =  0.5 - lambda;
    fg_shape_prior_cost{i} = fg_cost;
    shape_prior(:,i) = cbw(:);
    
    % boundbox
    [y x] = find(ubw);
    lx = min(x);
    rx = max(x);
    ty = min(y);
    dy = max(y);
    w = rx-lx;
    h = dy-ty;
    lx = max(1,ceil(lx-w*bbox_margin));
    rx = min(img_size(2),ceil(rx+w*bbox_margin));
    ty = max(1,ceil(ty-h*bbox_margin));
    dy = min(img_size(1),ceil(dy+h*bbox_margin));
    bbox(:,i) = [lx rx ty dy]';
end