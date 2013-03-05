function [contour_response ucm_response] =...
    computeContourResponse_(cim, ucm_im, ptrs, ptrs_region, ptr_idx1, ptr_idx2)

ptr1 = ptrs(ptr_idx1,:);
ptr2 = ptrs(ptr_idx2,:);

r1 = ptrs_region(ptr_idx1);
r2 = ptrs_region(ptr_idx2);

if ( r1 == r2 )
    contour_response = 0;
    ucm_response = 0;
    return;
end
    
lineptrs = linepoints(ptr1,ptr2);
line_inds = sub2ind(size(cim),lineptrs(2,:),lineptrs(1,:));
contour_response = max(cim(line_inds));
ucm_response = max(ucm_im(line_inds));