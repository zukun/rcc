function [fg_color_hist, bg_color_hist, bbox] =...
    buildContourTypeColorModel(segments, cluster_id,...
    gPb_thin, gPb_orient, pick_rate, color_index_map, n_color_bin, bbox_margin)

% paramter
n_cluster = numel(cluster_id);
n_orient = size(gPb_orient,3);
img_size = size(gPb_thin);
n_seg = size(segments,2);
segments = reshape(segments, [img_size,n_seg]);
edgel_thresh = 10;
min_d = 3;   

% bottom-up orient map
[unused orient_inds] = max(gPb_orient,[],3);
e = gPb_thin > 0;
img_orient_map = zeros(img_size, 'uint8');
img_orient_map(e) = orient_inds(e);
[dt dtl] = bwdist(e);

% normal vector
normal_vec = zeros(2,n_orient);
for i = 1 : n_orient
    theta = pi/2 + (i-1)*pi/n_orient;
    tan_vec = [cos(theta), -sin(theta)];
    normal_vec(:,i) = [-tan_vec(2) tan_vec(1)]';
end

fg_color_hist = cell(n_cluster,1);
bg_color_hist = cell(n_cluster,1);
bbox = zeros(4,n_cluster); % lx,rx,ty,dy

% get seed and histogram
for i = 1 : n_cluster
    try
    
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

        % seed boundary pixels
        seed_bdry = getReliableContour(cbw, gPb_thin, dt, dtl, pick_rate);
        n_seed_bdry = numel(seed_bdry);
        seed_orient = img_orient_map(dtl(seed_bdry));

        %% get foreground seeds

        [bdry_y bdry_x] = ind2sub(img_size, seed_bdry);

        % determin the max sample step
        cum_sum_y = cumsum(cbw,1);
        h = max(cum_sum_y,[],1);
        cum_sum_x = cumsum(cbw,2);
        w = max(cum_sum_x,[],2);

        n_step = 2;   
        x = repmat(bdry_x(:)', n_step,1);
        y = repmat(bdry_y(:)', n_step,1);
        dx = repmat(normal_vec(1,seed_orient),n_step,1);
        dy = repmat(normal_vec(2,seed_orient),n_step,1);
        sx = zeros(2, n_seed_bdry);
        sy = zeros(2, n_seed_bdry);
        sx(1,:) = min_d;
        sx(2,:) = 0.45*w(bdry_y);
        sy(1,:) = min_d;
        sy(2,:) = 0.45*h(bdry_x);

        % sample ptrs
        sx1 = floor(x + sx.*dx + 0.5);
        sy1 = floor(y + sy.*dy + 0.5);
        sx2 = floor(x - sx.*dx + 0.5);
        sy2 = floor(y - sy.*dy + 0.5);

        % remove out-of-range pixels
        tfsx1 = sx1 > 1 & sx1 < img_size(2);
        tfsy1 = sy1 > 1 & sy1 < img_size(1);
        tfsx2 = sx2 > 1 & sx2 < img_size(2);
        tfsy2 = sy2 > 1 & sy2 < img_size(1);
        tf1 = tfsx1 & tfsy1;
        tf2 = tfsx2 & tfsy2;
        ptr_x1 = sx1(tf1);
        ptr_y1 = sy1(tf1);
        ptr_x2 = sx2(tf2);
        ptr_y2 = sy2(tf2);
        ptr1 = sub2ind(img_size, ptr_y1,ptr_x1);
        ptr2 = sub2ind(img_size, ptr_y2,ptr_x2);

        % remove out-of-object pixels
        local_dt = bwdist(bwperim(cbw));
        tf1 = cbw(ptr1) & local_dt(ptr1) >= min_d - 1;
        tf2 = cbw(ptr2) & local_dt(ptr2) >= min_d - 1;
        ptr1 = ptr1(tf1);
        ptr2 = ptr2(tf2);

        % remove noisy, isolated pixels
        edgels = false(img_size);
        edgels([ptr1(:);ptr2(:)]) = true;
        cc = bwconncomp(edgels);
        numPixels = cellfun(@numel,cc.PixelIdxList);
        fg_seed_pixel = cell2mat(cc.PixelIdxList(numPixels > edgel_thresh)');

        %% get background seeds

        % boundbox
        [y x] = find(cbw);
        lx = min(x);
        rx = max(x);
        ty = min(y);
        dy = max(y);
        w = rx-lx;
        h = dy-ty;
        bg_margin = bbox_margin;
    %     bg_margin = max(bbox_margin - sum(cbw(:))/prod(img_size), 0.25);
        s = ceil(bg_margin*mean([w,h]));
        se = strel('disk',s);
        cbw_dil = imdilate(cbw,se);

        bg_seed_pixel = bwperim(cbw_dil) & ~ubw;

        %% build histogram
        if ( ~isempty(fg_seed_pixel) )
            fg_h = histc(color_index_map(fg_seed_pixel), 1:n_color_bin);
        else
            fg_h = ones(n_color_bin,1);
        end
        if ( ~isempty(bg_seed_pixel) )
            bg_h = histc(color_index_map(bg_seed_pixel), 1:n_color_bin);
        else
            bg_h = ones(n_color_bin,1);
        end

        fg_h = fg_h./sum(fg_h);
        bg_h = bg_h./sum(bg_h);

        fg_color_hist{i} = fg_h;
        bg_color_hist{i} = bg_h;

        %% bbox
    %     [y x] = find(cbw_dil);
    %     lx = min(x);
    %     rx = max(x);
    %     ty = min(y);
    %     dy = max(y);
        lx = max(1,ceil(lx-w*bbox_margin));
        rx = min(img_size(2),ceil(rx+w*bbox_margin));
        ty = max(1,ceil(ty-h*bbox_margin));
        dy = min(img_size(1),ceil(dy+h*bbox_margin));
        bbox(:,i) = [lx rx ty dy]';
    end
    
end

