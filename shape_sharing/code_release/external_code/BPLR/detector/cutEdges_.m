function is_cut =...
    cutEdges_(contour_response, ucm_response, contour_thresh, ucm_thresh,...
    edge_color_dist, sort_edge_color_dist, ptrs_region, edges, color_thresh)
% small color_thresh --> more edges are cut

top_k = ceil(color_thresh*numel(sort_edge_color_dist));
ref_color_dist = sort_edge_color_dist(top_k);

is_cut = arrayfun(@(idx)(cutEdgeElem_(idx, edges(idx,1), edges(idx,2), ptrs_region,...
    contour_response, ucm_response, contour_thresh, ucm_thresh,...
    edge_color_dist, ref_color_dist)),...
    1:size(edges,1));
    
end

function is_cut = cutEdgeElem_(edge_idx, ptr_idx1, ptr_idx2, ptrs_region,...
    contour_response, ucm_response, contour_thresh, ucm_thresh,...
    edge_color_dist, ref_color_dist)

is_cut = false;
r1 = ptrs_region(ptr_idx1);
r2 = ptrs_region(ptr_idx2);
    
if ( r1 ~= r2 )
    contour_strength = contour_response(edge_idx);
    ucm_strength = ucm_response(edge_idx);
    
    if ( contour_strength > contour_thresh &&...
         ucm_strength > ucm_thresh )
        is_cut = true;
    
    elseif ( contour_strength/contour_thresh + ucm_strength/ucm_thresh > 2 )
        is_cut = true;
        
    elseif ( contour_strength > contour_thresh ||...
         ucm_strength > ucm_thresh )
        
        color_between_dist = edge_color_dist(edge_idx);
        
        if ( color_between_dist > ref_color_dist )
            is_cut = true;
        end
        
    end    
end

end



