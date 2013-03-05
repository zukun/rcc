function [sorted_ptrs sorted_inds] = sortPtrs(ptrs, x_sorted)

if ( x_sorted )
    [v, x_sorted_inds] = sort(ptrs(1,:));
    sorted_ptrs = ptrs(:,x_sorted_inds);
    sorted_inds = zeros(1,size(ptrs,2));
    idx = 1;
    toggle = 1;
    while ( idx <= size(sorted_ptrs,2) )
        l = length(find(sorted_ptrs(1,:) == sorted_ptrs(1,idx)));
        if ( toggle )
            [v inds] = sort(sorted_ptrs(2,idx:idx+l-1), 'ascend');
            toggle = 0;
        else
            [v inds] = sort(sorted_ptrs(2,idx:idx+l-1), 'descend');
            toggle = 1;
        end            
        inds = inds + idx - 1;
        sorted_ptrs(:,idx:idx+l-1) = sorted_ptrs(:,inds);
        sorted_inds(idx:idx+l-1) = x_sorted_inds(inds);
        idx = idx + l;
    end 
else
    [v, y_sorted_inds] = sort(ptrs(2,:), 'descend');
    sorted_ptrs = ptrs(:,y_sorted_inds);
    sorted_inds = zeros(1,size(ptrs,2));
    idx = 1;
    toggle = 1;
    while ( idx <= size(sorted_ptrs,2) )
        l = length(find(sorted_ptrs(2,:) == sorted_ptrs(2,idx)));
        if ( toggle )
            [v inds] = sort(sorted_ptrs(1,idx:idx+l-1), 'descend');
            toggle = 0;
        else
            [v inds] = sort(sorted_ptrs(1,idx:idx+l-1), 'ascend');
            toggle = 1;
        end            
        inds = inds + idx - 1;
        sorted_ptrs(:,idx:idx+l-1) = sorted_ptrs(:,inds);
        sorted_inds(idx:idx+l-1) = y_sorted_inds(inds);
        idx = idx + l;
    end 
end 
