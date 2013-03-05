function [regions, hier_mem_inds, dist_mat] = getHierarchicalRegions(ucm2, n_level, cutoff_thresh)
% ucm: ultrametric contour map
% n_level: number of levels in hierarchy
% cutoff_thresh (0.25) : small regions are removed for less than cutoff_thresh*mu,
% regions: 
% hier_mem_inds: cell(n_level,1): fine (1) to coarse (end)
% dist_mat: ucm distance between regions

% record unique levels
mu = mean(ucm2(ucm2>0));
min_t = 0.1*mu;
max_t = 1.0;
t = linspace(min_t,max_t,n_level);
n_seg = zeros(1,n_level);
for i = 1:n_level
    labels2 = bwlabel(ucm2 <= t(i));
    labels = labels2(2:2:end, 2:2:end);
    n_seg(i) = max(labels(:));
end
[unused uid] = unique(n_seg);
t = t(uid);
t = sort(t); % finer segment get smaller region idx

% extract regions
r_cnt = 0;
img_size = size(ucm2(2:2:end,2:2:end));
for i = 1:numel(t)
    labels2 = bwlabel(ucm2 <= t(i));
    segmentation_i = labels2(2:2:end, 2:2:end);
    n_seg_i = max(segmentation_i(:));
    
    for r = 1 : n_seg_i 
        region = segmentation_i == r;
        region_area = sum(region(:));
        if ( region_area < 25*25 && t(i) < cutoff_thresh*mu ) 
            continue;
        end
%         if ( region_area < 250 && t(i) < cutoff_thresh*mu ) 
%             continue;
%         end

        r_cnt = r_cnt + 1;
        regions(r_cnt).segment = uint32(find(region == 1)');
        regions(r_cnt).contour = uint32(find(bwperim(region)));
        regions(r_cnt).img_size = img_size;
        regions(r_cnt).area = region_area;
        [y x] = ind2sub(img_size, regions(r_cnt).segment);
        regions(r_cnt).center = [mean(x), mean(y)]';
        regions(r_cnt).level = i;
        regions(r_cnt).parent = []; % initialize 
        regions(r_cnt).children = [];
        regions(r_cnt).rid = [x(1) y(1) region_area]';
    end
end

% remove duplicates
[unused uid eid] = unique([regions.rid]', 'rows');
% which uregions each hierarchy has
hier_mem_inds = cell(1,numel(t));
for i = 1 : numel(eid)
    l = regions(i).level;
    hier_mem_inds{l} = [hier_mem_inds{l} eid(i)];
end
regions = regions(uid);
    
% define hierarchy - record parent of each region
for i = 1 : numel(t)-1
    p_inds = hier_mem_inds{i+1}; % parents
    c_inds = hier_mem_inds{i}; % children
    c_indicators =...
        arrayfun(@(idx)(regions(c_inds(idx)).segment(1)), 1:numel(c_inds));    
    for j =1: numel(p_inds)
        tf = ismember(c_indicators, regions(p_inds(j)).segment);
        idx = find(tf);
        % parent
        for k = 1 : numel(idx)
            regions(c_inds(idx(k))).parent = p_inds(j);
        end
    end
    if ( i+1 == numel(t) )
        for j = 1: numel(p_inds)
            regions(p_inds(j)).parent = [];
        end
    end
end
% record children of each region
for i = 1 : numel(regions)
    p = regions(i).parent;
    if ( ~isempty(p) )
        regions(p).children(end+1) = i;
    end
end
% intergral image
for r = 1 : numel(regions)
    bw = false(regions(r).img_size);
    bw(regions(r).segment) = true;
    integral_y = cumsum(bw);
    regions(r).integral_y = integral_y;
end

% distance matrix: t(i) at the least common ancestor
n_region = numel(regions);
dist_mat = ones(n_region, n_region);
kids = arrayfun(@(idx)(getDescendant(regions,idx,true)), 1:n_region, 'uniformoutput', false);
for i = numel(t) : -1 : 2
    inds =  hier_mem_inds{i};
    for j = 1 : numel(inds)
        r = inds(j);
        dist_mat(kids{r},kids{r}) = t(i);
    end
end
dist_mat = replaceDiagonal(dist_mat,inf);

disp([num2str(numel(regions)) ' regions']);

end

function k = getDescendant(regions,idx,self)
q = idx; %include itself
k = [];
while ( ~isempty(q) )
    new_elem = regions(q(1)).children;
    k = [k q(1)]; % add kid list
    q = [q(2:end) new_elem]; % update queue    
end
if (~self)
    k = k(2:end);
end
k = unique(k);
end



        
        




        
        
    
    
    

