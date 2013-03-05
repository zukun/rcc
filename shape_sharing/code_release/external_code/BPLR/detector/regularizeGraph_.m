function [added_edges bridge_edges bridge_ci_inds] =...
    regularizeGraph_(ptrs, edges, neighbor_r, min_neighbor_d)
% neighbor_r (=5.0): neighbor is defined as neighbor_r*max(tree_neighbor_d)
% min_neighbor_d (= 4.0)
%
% added_edges: added edges for regularization
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
adj_mat = false(n_ptr, n_ptr);
adj_inds = sub2ind(size(adj_mat), edges(:,1), edges(:,2));
adj_mat(adj_inds) = true;
adj_inds = sub2ind(size(adj_mat), edges(:,2), edges(:,1));
adj_mat(adj_inds) = true;

% connected component
[i j v] = find(adj_mat);
v = double(v);
[m n] = size(adj_mat);
adj_s = sparse(i,j,v,m,n);
ci = components(adj_s);

% local distance (centered at each ptr)
% ptr_dist = slmetric_pw(ptrs, ptrs, 'eucdist');
% ptr_dist = replaceDiagonal(ptr_dist, inf);
% local_d =...
%     arrayfun(@(i)(localDistance(ptr_dist(i,:), find(adj_mat(i,:)))), 1:n_ptr, 'uniformoutput', false);
% 
% % define neighbors
% neighbor_d = neighbor_r*cellfun(@max, local_d);
% neighbor_d(neighbor_d < min_neighbor_d) = min_neighbor_d;
% neighbor_mat = ptr_dist <= repmat(neighbor_d(:), 1, n_ptr);
% neighbor_mat = replaceDiagonal(neighbor_mat, false);
% neighbor_list = arrayfun(@(i)(ptrs(:, neighbor_mat(i,:))), 1:n_ptr, 'uniformoutput', false);
% 
% % pick 4-connected neighbors
% four_neighbor_inds =...
%     arrayfun(@(i)(fourNeighbors(neighbor_list{i}, find(neighbor_mat(i,:)), ptrs(:,i), i)),...
%     1:n_ptr, 'uniformoutput', false);

% mex-version
four_neighbor_inds = FindFourNeighborsMex(ptrs, adj_mat, neighbor_r, min_neighbor_d);

neighbor_edges = cell2mat(four_neighbor_inds')';
adj_mat = false(n_ptr, n_ptr);
adj_inds = sub2ind(size(adj_mat), neighbor_edges(:,1), neighbor_edges(:,2));
adj_mat(adj_inds) = true;
adj_mat = triu(adj_mat,1);
[i j] = find(adj_mat);
new_edges = zeros(numel(i),2);
new_edges(:,1) = i;
new_edges(:,2) = j;

% connected components --> exclude connection to different components
new_ci = zeros(size(new_edges));
new_ci(:,1) = ci(i);
new_ci(:,2) = ci(j);
tf = new_ci(:,1) == new_ci(:,2);
added_edges = new_edges(tf,:);
bridge_edges = new_edges(~tf,:);
bridge_ci_inds = new_ci(~tf,:);

end


% function local_dist = localDistance(ptr_dist, adj_inds)
% if ( isempty(adj_inds) )
%     local_dist = 0;
% else
%     local_dist = ptr_dist(adj_inds);
% end
% end
% 
% function four_neighbor_inds = fourNeighbors(neighbor_ptrs, neighbor_inds, ref_ptr, ref_idx)
% if ( isempty(neighbor_inds) )
%     four_neighbor_inds = [];
% else
% 
%     offset_ptrs = neighbor_ptrs - repmat(ref_ptr, 1,size(neighbor_ptrs,2));
% 
%     d = sqrt(sum(offset_ptrs.^2,1));
%     angles = atan2(offset_ptrs(2,:), offset_ptrs(1,:));
%     angles(angles<0) = angles(angles<0) + 2*pi;
% 
%     n_class = 4;
%     axis_class = zeros(1, numel(angles));
%     axis_class(angles >= 0 & angles < pi/4) = 1;
%     axis_class(angles >= pi/4 & angles < 3*pi/4) = 2;
%     axis_class(angles >= 3*pi/4 & angles < 5*pi/4) = 3;
%     axis_class(angles >= 5*pi/4 & angles < 7*pi/4) = 4;
%     axis_class(angles >= 7*pi/4 & angles <= 2*pi) = 1;
% %     n_class = 4;
% %     axis_class = ceil(angles./(2*pi/n_class));
%     four_neighbor_inds = [];
%     for i = 1 : n_class
%         inds = neighbor_inds(axis_class==i);
%         if ( ~isempty(inds) )
%             [unused idx] = min(d(axis_class==i));
%             four_neighbor_inds = [four_neighbor_inds; [ref_idx inds(idx)]];
%         end
%     end
% end
% 
% end
