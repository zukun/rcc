function [ptrs edges scales] = createUniformSamplePtrsFromDT_(regions, im, ucm, contour_im, order_k, sample_grid_spacing)

% parameters
supp_radius_thresh = 0; %minimum scale of sampled ptrs

% sample points at each region
disp('sampling ptrs...');
[ptrs scales ptrs_region] = uniformSamplePtrsFromRegions_(regions, sample_grid_spacing, supp_radius_thresh);
[ptrs uid] = unique(ptrs,'rows');
scales = scales(uid);
ptrs_region = ptrs_region(uid);
n_ptrs = numel(scales);
disp([num2str(n_ptrs) ' ptrs obtained']);

% build spanning tree
disp('buidling min spanning tree...');
[L, edges] = computeEMSTofPoints(ptrs, 0, 1.0);

% post-processing: cut edges crossing strong contour and add small
% components
disp('post-processing: pruning spurious tree edges...');

% cut edges intervening strong contours
cim = imresize(contour_im, regions(1).img_size, 'bilinear'); 
cim = double(cim);
ucm_im = imresize(ucm, regions(1).img_size, 'bilinear');
ucm_im = double(ucm_im);
c_vals = cim(:);
u_vals = ucm_im(:);
c_vals = c_vals(c_vals>0);
u_vals = u_vals(u_vals>0);
contour_thresh = mean(c_vals);
ucm_thresh = mean(u_vals);
contour_cut =...
    computeContourCut_(cim, ucm_im, ptrs, ptrs_region, edges, ucm_thresh, contour_thresh);

% cut trivial links
%im = imresize(im, regions(1).img_size);
%edge_color_dist = computeEdgeColorDist(im, edges, ptrs, scales, min_feat_scale);
%sort_edge_color_dist = sort(edge_color_dist);
%color_thresh = sort_edge_color_dist(ceil(numel(sort_edge_color_dist)*0.95));
% trivial_cut =...
%     computeTrivialCut_(ptrs, edges, scales, im, min_feat_scale, color_thresh);
%is_cut = contour_cut | trivial_cut;

is_cut = contour_cut;
cut_edges = edges(is_cut,:);
edges(is_cut,:) = [];
disp(['cut: ' num2str(sum(is_cut))]);

% regularize graph 
disp('regularizing graph...');
neighbor_range = 5.0;
min_neighbor_d = sample_grid_spacing;
[unused bridge_edges bridge_ci_inds] =...
    regularizeGraph_(ptrs, edges, neighbor_range, min_neighbor_d);

% link different components
disp('linking different connected components...');
contour_cut =...
    computeContourCut_(cim, ucm_im, ptrs, ptrs_region, bridge_edges, ucm_thresh, contour_thresh);
% trivial_cut =...
%     computeTrivialCut_(ptrs, bridge_edges, scales, im, min_feat_scale, color_thresh);
%is_cut = contour_cut | trivial_cut;
is_cut = contour_cut;
%cut_thresh = 0.05;
cut_thresh = 1e-6;
added_edges = linkComponents_(bridge_edges, bridge_ci_inds, is_cut, cut_thresh);

% remove edges sharing nodes with cut_edges
cut_ptr_inds = cut_edges(:);
tf1 = ismember(added_edges(:,1), cut_ptr_inds);
tf2 = ismember(added_edges(:,2), cut_ptr_inds);
overlap_tf = tf1 | tf2;
added_edges = added_edges(~overlap_tf, :);
edges = [edges; added_edges];
edges = unique(edges, 'rows');
disp(['add: ' num2str(size(added_edges,1))]);

% display
% im = imresize(im, regions(1).img_size);
% figure,
% imshow(im);
% hold on;
% for i = 1 : size(edges,1)
%     line(ptrs(edges(i,:),1), ptrs(edges(i,:),2), 'color', [0 0 1]);
% end
% for i = 1 : size(cut_edges,1)
%     line(ptrs(cut_edges(i,:),1), ptrs(cut_edges(i,:),2), 'color', [1 0 0]);
% end
% for i = 1 : size(added_edges,1)
%     line(ptrs(added_edges(i,:),1), ptrs(added_edges(i,:),2), 'color', [0 1 1]);
% end

end

function contour_cut =...
    computeContourCut_(cim, ucm_im, ptrs, ptrs_region, edges,...
    ucm_thresh, contour_thresh)

[contour_response ucm_response] = arrayfun(@(i)(computeContourResponse_(...
    cim, ucm_im, ptrs, ptrs_region, edges(i,1), edges(i,2))),...
    1:size(edges,1));
contour_cut = (ucm_response > ucm_thresh | contour_response > contour_thresh);

end

function trivial_cut =...
    computeTrivialCut_(ptrs, edges, scales, im, min_feat_scale, color_thresh)

edges_length = arrayfun(@(i)(computePtrDist_(ptrs, edges(i,1), edges(i,2))),...
    1:size(edges,1));
scales_dist = arrayfun(@(i)(computeScaleDist_(scales, edges(i,1), edges(i,2))),...  
    1:size(edges,1));
edge_color_dist = computeEdgeColorDist(im, edges, ptrs, scales, min_feat_scale);
trivial_cut = edges_length > scales_dist & edge_color_dist > color_thresh;
end
    
function d = computePtrDist_(ptrs, ptr_idx1, ptr_idx2)
d = norm(ptrs(ptr_idx1,:) - ptrs(ptr_idx2,:));
end

function d = computeScaleDist_(scales, ptr_idx1, ptr_idx2)
d = scales(ptr_idx1) + scales(ptr_idx2);
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




