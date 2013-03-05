function added_edges =...
    computeBridgeEdge_(ptrs, edges, neighbor_r, min_neighbor_d,...
    cim, ucm_im, ucm_thresh, contour_thresh, ptrs_region)
% neighbor_r (=5.0): neighbor is defined as neighbor_r*max(tree_neighbor_d)
% min_neighbor_d (= 4.0)
%
% bridge_edges: edges linking different connected components
% bridge_ci_inds: component inds of bridge_edges

if ( size(ptrs,1) ~= 2 )
    ptrs = ptrs';
end
if ( size(edges,2) ~= 2 )
    edges = edges';
end

% build adjacent graph/list
n_ptr = size(ptrs,2);
adj_mat_tmp = zeros(n_ptr, n_ptr, 'uint8');
adj_inds = sub2ind(size(adj_mat_tmp), edges(:,1), edges(:,2));
adj_mat_tmp(adj_inds) = 1;
adj_mat_tmp = adj_mat_tmp + adj_mat_tmp';
adj_mat = false(n_ptr, n_ptr);
adj_mat(adj_mat_tmp > 0 ) = true;
clear adj_mat_tmp

% connected component
[i j v] = find(adj_mat);
v = double(v);
[m n] = size(adj_mat);
adj_s = sparse(i,j,v,m,n);
ci = components(adj_s);

% local distance (centered at each ptr)
ptr_dist = slmetric_pw(ptrs, ptrs, 'eucdist');
ptr_dist = replaceDiagonal(ptr_dist, inf);
local_d =...
    arrayfun(@(i)(localDistance(ptr_dist(i,:), find(adj_mat(i,:)))), 1:n_ptr, 'uniformoutput', false);

% define neighbors
neighbor_d = neighbor_r*cellfun(@max, local_d);
neighbor_d(neighbor_d < min_neighbor_d) = min_neighbor_d;
neighbor_mat = ptr_dist <= repmat(neighbor_d(:), 1, n_ptr);
neighbor_mat = replaceDiagonal(neighbor_mat, false);
[i j] = find(neighbor_mat);
new_edges = zeros(numel(i),2);
new_edges(:,1) = i;
new_edges(:,2) = j;

% connected components --> exclude connection to different components
new_ci = zeros(size(new_edges));
new_ci(:,1) = ci(i);
new_ci(:,2) = ci(j);
tf = new_ci(:,1) == new_ci(:,2);
bridge_edges_init = new_edges(~tf,:);
bridge_ci_inds_init = new_ci(~tf,:);

% contour check
contour_cut =...
    computeContourCut_(cim, ucm_im, ptrs', ptrs_region, bridge_edges_init, ucm_thresh, contour_thresh);
cut_thresh = 0.05;
added_edges_init =...
    linkComponents_(bridge_edges_init, bridge_ci_inds_init, contour_cut, cut_thresh);

% four connected neighbor
neighbor_mat = false(n_ptr,n_ptr);
neighbor_inds = sub2ind(size(neighbor_mat), added_edges_init(:,1), added_edges_init(:,2));
neighbor_mat(neighbor_inds) = true;
neighbor_inds = sub2ind(size(neighbor_mat), added_edges_init(:,2), added_edges_init(:,1));
neighbor_mat(neighbor_inds) = true;
neighbor_list = arrayfun(@(i)(ptrs(:, neighbor_mat(i,:))), 1:n_ptr, 'uniformoutput', false);

% pick 4-connected neighbors
four_neighbor_inds =...
    arrayfun(@(i)(fourNeighbors(neighbor_list{i}, find(neighbor_mat(i,:)), ptrs(:,i), i)),...
    1:n_ptr, 'uniformoutput', false);

neighbor_edges = cell2mat(four_neighbor_inds');
adj_mat = false(n_ptr, n_ptr);
adj_inds = sub2ind(size(adj_mat), neighbor_edges(:,1), neighbor_edges(:,2));
adj_mat(adj_inds) = true;
adj_mat = triu(adj_mat,1);
[i j] = find(adj_mat);
added_edges = zeros(numel(i),2);
added_edges(:,1) = i;
added_edges(:,2) = j;

end


function local_dist = localDistance(ptr_dist, adj_inds)
if ( isempty(adj_inds) )
    local_dist = 0;
else
    local_dist = ptr_dist(adj_inds);
end
end

function contour_cut =...
    computeContourCut_(cim, ucm_im, ptrs, ptrs_region, edges,...
    ucm_thresh, contour_thresh)

[contour_response ucm_response] = arrayfun(@(i)(computeContourResponse_(...
    cim, ucm_im, ptrs, ptrs_region, edges(i,1), edges(i,2))),...
    1:size(edges,1));
contour_cut = (ucm_response > ucm_thresh | contour_response > contour_thresh);

end

function added_edges =...
    linkComponents_(bridge_edges, bridge_ci_inds, is_cut, cut_thresh)

[u ii jj] = unique(bridge_ci_inds, 'rows');
group_inds = arrayfun(@(i)(jj == jj(i)), 1:numel(jj), 'uniformoutput', false);
group_inds = cell2mat(group_inds);
group_inds = group_inds(ii,:);

cut_rate =...
    arrayfun(@(i)(sum(is_cut(group_inds(i,:)))/sum(group_inds(i,:))), 1:size(group_inds,1));

link_edge_inds = sum(group_inds(cut_rate < cut_thresh,:), 1) > 0;
is_link = ~is_cut & link_edge_inds;

added_edges = bridge_edges(is_link,:);

end

