function added_edges =...
    addEdges_(cim, ucm, contour_thresh, ucm_thresh,...
    ptrs, edges, neighbor_range, min_component_size)

% compute connected components
adj = false(size(ptrs,1)); 
inds = sub2ind(size(adj), edges(:,1), edges(:,2));
adj(inds) = 1;
inds = sub2ind(size(adj), edges(:,2), edges(:,1));
adj(inds) = 1;
[i j v] = find(adj);
v = double(v);
[m n] = size(adj);
adj_s = sparse(i,j,v,m,n);
[ci csizes] = components(adj_s);
clear adj adj_s i j v

% find small-size components
small_c_inds = find(csizes <= min_component_size);

% euclidean distance between small-size component and others
% euc_dist: n_small by n_component
% closest_ptrs: n_small by 1 cell, each cell: 2 by 2n_component: col: 2xn-1:2xn
% --> pair of closest ptrs

ptrs = ptrs';
[euc_dist closest_ptrs] =...
    arrayfun(@(i)(computeOne2AllCompGeoDist(ptrs, ci, i)), small_c_inds, 'uniformoutput', false);
euc_dist = cell2mat(euc_dist); 

% find neighbor components of small-size components
neighbor_c_flag = euc_dist <= neighbor_range;

% contour dist -- center
c_center_ptrs = arrayfun(@(i)(computeComponentCenter(ptrs, ci, i)), 1:max(ci),...
    'uniformoutput', false);
c_center_ptrs = cell2mat(c_center_ptrs);
[ii jj] = find(neighbor_c_flag);
[contour_response_ ucm_response_] =...
    arrayfun(@(i,j)(computeContourResponse(cim, ucm, c_center_ptrs(:,i), c_center_ptrs(:,j))),...
    ii, jj);
contour_response = inf(size(neighbor_c_flag));
ucm_response = inf(size(neighbor_c_flag));
contour_response(neighbor_c_flag) = contour_response_;
ucm_response(neighbor_c_flag) = ucm_response_;
contour_neighbor_flag1 = contour_response < contour_thresh | ucm_response < ucm_thresh;

% contour dist -- closest ptr
[contour_response_ ucm_response_] =...
    arrayfun(@(i,j)(computeContourResponse(cim, ucm, closest_ptrs{i}(:,2*j-1), closest_ptrs{i}(:,2*j))),...
    ii, jj);
contour_response = inf(size(neighbor_c_flag));
ucm_response = inf(size(neighbor_c_flag));
contour_response(neighbor_c_flag) = contour_response_;
ucm_response(neighbor_c_flag) = ucm_response_;
contour_neighbor_flag2 = contour_response < contour_thresh | ucm_response < ucm_thresh;

contour_neighbor_flag = contour_neighbor_flag1 & contour_neighbor_flag2;

% neighbor
neighbor_flag = neighbor_c_flag & contour_neighbor_flag;

% link to the component with the closest component
euc_dist(~neighbor_flag) = inf;
[min_dist min_inds] = min(euc_dist,[],2);
added_edges = arrayfun(@(i)(...
    convertPtr2Edge(ptrs, closest_ptrs{i}(:,2*min_inds(i)-1), closest_ptrs{i}(:,2*min_inds(i)))),...
    1:numel(closest_ptrs), 'uniformoutput', false);
added_edges = cell2mat(added_edges');
added_edges(isinf(min_dist),:) = [];
added_edges = unique(added_edges, 'rows');
end



function [d closest_ptrs]= computeOne2AllCompGeoDist(ptrs, ci, c_idx)
others = 1:max(ci);
this_ptrs = ptrs(:, ci == c_idx);
[d closest_ptrs] = arrayfun(@(i)(minDist(this_ptrs, ptrs(:,ci==i))), others, 'uniformoutput', false);
d = cell2mat(d);
d(c_idx) = inf;
closest_ptrs = cell2mat(closest_ptrs);
end

function [d closest_ptr_pair] = minDist(ptrs1, ptrs2)
all_d = slmetric_pw(ptrs1, ptrs2,'eucdist');
[d idx] = min(all_d(:));
[i j] = ind2sub(size(all_d), idx);
closest_ptr_pair(:,1) = ptrs1(:,i);
closest_ptr_pair(:,2) = ptrs2(:,j);
end

function center_ptr = computeComponentCenter(ptrs, ci, c_idx)
center_ptr = mean(ptrs(:,ci==c_idx),2);
end

function [contour_strength ucm_strength] = computeContourResponse(cim, ucm, ptr1, ptr2)

lineptrs = linepoints(ptr1,ptr2);
line_inds = sub2ind(size(cim),lineptrs(2,:),lineptrs(1,:));
contour_strength = max(cim(line_inds));
ucm_strength = max(ucm(line_inds));

end

function e = convertPtr2Edge(ptrs, ptr1, ptr2)
e = [find(ptrs(1,:) == ptr1(1) & ptrs(2,:) == ptr1(2)),...
    find(ptrs(1,:) == ptr2(1) & ptrs(2,:) == ptr2(2))];
end





