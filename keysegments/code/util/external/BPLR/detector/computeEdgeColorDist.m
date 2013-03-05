function edge_color_dist = computeEdgeColorDist(im, edges, ptrs, scales, min_feat_scale)

color_hist = computeColorHist_(im,ptrs,scales,min_feat_scale);
color_hist = single(color_hist);

edge_color_dist =...
    arrayfun(@(i)(edgeColorDist(edges(i,1),edges(i,2), color_hist)),1:size(edges,1));

end


function d = edgeColorDist(ptr_idx1, ptr_idx2, color_hist)

d = color_hist(:,ptr_idx1) - color_hist(:,ptr_idx2);
d = sqrt(sum(d.^2));

end

