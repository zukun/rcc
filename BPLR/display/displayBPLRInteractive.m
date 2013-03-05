function displayBPLRInteractive(im, BPLR)
% im: input image
% BPLR: extracted BPLRs for im

im = imresize(im, BPLR.img_size);
feats = BPLR.feats;
min_scale = BPLR.min_elem_scale;
clear BPLR

figure, 
imshow(im);

while (1)
    [x y button] = ginput(1);
    if ( button == 3 )
        close;
        break;
    end
    
    imshow(im);
    hold on
    
    ref_ptrs = [feats.ref_ptr];
    D = slmetric_pw([x y]', ref_ptrs, 'eucdist'); 
    min_dist = min(D);
    min_inds = find(D == min_dist);
    
    rinds = randperm(numel(min_inds));
    i = min_inds(rinds(1));
    n = feats(i).n_member;
    ptrs = feats(i).member_ptrs;
    plot(ptrs(1,:),ptrs(2,:),'r*');
    
    ref_ptr = feats(i).ref_ptr;
    ref_scale = max([feats(i).ref_scale min_scale]);
    
    radius = feats(i).member_scales;
    radius(radius < min_scale) = min_scale;
        
    for j = 1 : n
        rectangle('position', [ptrs(1,j)-radius(j), ptrs(2,j)-radius(j), 2*radius(j), 2*radius(j)], 'edgecolor', [0,0,1], 'linewidth', 0.5, 'curvature',[1 1]);
    end
    rectangle('position', [ref_ptr(1)-ref_scale, ref_ptr(2)-ref_scale, 2*ref_scale, 2*ref_scale], 'edgecolor', [1,0,0], 'linewidth', 2.0, 'curvature',[1 1]);        
    plot(ref_ptr(1),ref_ptr(2),'c*');

    title(['ref ptr: ' num2str(ref_ptr(1)) ',' num2str(ref_ptr(2)) ' euclidean scale: ' num2str(feats(i).magnif)]);
    hold off;
end