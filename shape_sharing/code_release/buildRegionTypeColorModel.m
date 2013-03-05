function [fg_color_hist, bg_color_hist bbox] =...
    buildRegionTypeColorModel(segments, cluster_id, color_index_map, n_color_bin, bbox_margin)

% paramter
n_cluster = numel(cluster_id);
img_size = size(color_index_map);
n_seg = size(segments,2);
segments = reshape(segments, [img_size, n_seg]);

% build color model
fg_color_hist = cell(n_cluster,1);
bg_color_hist = cell(n_cluster,1);
bbox = zeros(4,n_cluster); %lx,rx,ty,dy
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
    
    % pick the best boundbox
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

    % determine bandwidth
    fg_size = sum(cbw(:));
    w = rx-lx;
    h = dy-ty;
    bandwidth = ceil(0.5*fg_size/(w+h));

    % background
    lx2 = lx + bandwidth;
    rx2 = rx - bandwidth;
    ty2 = ty + bandwidth;
    dy2 = dy - bandwidth;
    bgbw = false(img_size);
    bgbw(ty:dy,lx:rx) = true;
    bgbw(ty2:dy2,lx2:rx2) = false;
    bgbw = bgbw & ~ubw;

    % histogram
    if ( ~isempty(cbw) )
        fg_h = histc(color_index_map(cbw), 1:n_color_bin);
    else
        fg_h = ones(n_color_bin,1);
    end
    if ( ~isempty(bgbw) )
        bg_h = histc(color_index_map(bgbw), 1:n_color_bin);
    else
        bg_h = ones(n_color_bin,1);
    end
    fg_h = fg_h./sum(fg_h);
    bg_h = bg_h./sum(bg_h);
   
    fg_color_hist{i} = fg_h;
    bg_color_hist{i} = bg_h;
    
    % bbox
    bbox(:,i) = [lx rx ty dy]';
    
end