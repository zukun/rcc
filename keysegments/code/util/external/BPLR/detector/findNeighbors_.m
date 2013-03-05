function neighbors = findNeighbors_(links, node_idx, order_k)

n_elem = numel(links);
neighbors = false(n_elem,1);
neighbors_prev_i = false(n_elem,1);
neighbors_prev_i(node_idx) = true;
neighbors_i = false(n_elem,1);

for i = 1 : order_k
    neighbors_i_inds = [links{neighbors_prev_i}];
    neighbors_i(neighbors_i_inds) = true;
    neighbors_i = neighbors_i & ~neighbors_prev_i;
    neighbors(neighbors_i) = true;        
    neighbors_prev_i = neighbors_i;
end    

neighbors(node_idx) = false;