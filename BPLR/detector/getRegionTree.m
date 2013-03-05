function regions = getRegionTree(ucm, min_nseg, max_nseg, im_dim)

% adaptive threshold for multiple segmentation
disp('multiple segmentations...');
if ( strcmp(class(ucm),'uint8'))
    non_zero_response = ucm(ucm > 10.0);
else
    non_zero_response = ucm(ucm > 10.0/255);
end

seg_thres = mean(non_zero_response(:));
levels = [];
nsegs = [];
for l = 0.1 : 0.1 : 3.0 
    labels = bwlabel(ucm <= l*seg_thres);
    nseg = max(labels(:));
    levels = [levels l];
    nsegs = [nsegs nseg];
end
% levels = [];
% nsegs = [];
% for l = 0.025 : 0.025 : 1.0 
%     labels = bwlabel(ucm <= l);
%     nseg = max(labels(:));
%     levels = [levels l];
%     nsegs = [nsegs nseg];
% end
[nsegs uid] = unique(nsegs);
levels = levels(uid);
tf = nsegs >= min_nseg & nsegs <= max_nseg;
if (sum(tf) > 0)
    levels = levels(tf);
end
levels = sort(levels); % (smallest level = tree leaves)

% image re-scale
re_scale = 1;
if ( max(size(ucm)) > im_dim )
    re_scale = im_dim/max(size(ucm));    
end

%disp('generate regions: get information from multiple segmentation...');
% multiple segmentation 
rid = 1;
MIN_REGION_AREA = 100;
BORDER_MARGIN = 20;
for l = 1 : numel(levels)
    labels =  bwlabel(ucm <= levels(l)*seg_thres);
    if ( re_scale < 1 )
        labels = imresize(labels, re_scale, 'nearest');
    end
    rp = regionprops(labels, 'ConvexImage', 'BoundingBox');
    for r = 1 : max(labels(:)) % labels == 0 : contour

        region = bwlabel(labels == r);
        region_area = sum(region(:));
        if ( region_area < MIN_REGION_AREA ) 
            continue;
        end
        contour = bwperim(region);
        
        regions(rid).segment = find(region == 1)';
        regions(rid).contour = find(contour == 1)';
        [y x] = ind2sub(size(labels), regions(rid).segment);
        regions(rid).center = [mean(x), mean(y)]';
        regions(rid).area = region_area;
        regions(rid).marker = [x(1), y(1)]';
        regions(rid).convexhull = false(size(labels));
        
        lx = floor(rp(r).BoundingBox(1) + 0.5);
        ly = floor(rp(r).BoundingBox(2) + 0.5);
        w = rp(r).BoundingBox(3);
        h = rp(r).BoundingBox(4);
        regions(rid).convexhull(ly : ly+h-1, lx: lx+w-1) = rp(r).ConvexImage;
        regions(rid).convexhull = find(regions(rid).convexhull == 1); % for compactness
        
        n_imageBoundContours = get_border(contour, BORDER_MARGIN);
        regions(rid).n_imboundContours = n_imageBoundContours;
        regions(rid).perimeter = sum(contour(:));
        regions(rid).boundbox = [lx, ly, lx+w-1, ly+h-1]';
        
        regions(rid).img_size = size(labels);
        regions(rid).contour_thresh = levels(l)*seg_thres;
        
        rid = rid + 1;
    end
end

% identify the same segment
centers = [regions.marker];
areas = [regions.area];
rids = [centers; areas]'; %region id
[rids uids] = unique(rids, 'rows'); %find unique regions
regions = regions(uids);

% sort regions by area
[areas rank] = sort([regions.area]);
regions = regions(rank);

% build tree
%disp('generate regions:build tree...');
checked_child = false(1, numel(regions));
for r = 2 : numel(regions)
    markers = [regions(1:r-1).marker];
    marker_inds = sub2ind(size(labels), markers(2,:), markers(1,:));
    tf = ismember(marker_inds, regions(r).segment);
    child = find(tf == 1);
    regions(r).children = child(checked_child(child) == false);    
    for r2 = 1 : numel(regions(r).children)
        ridx = regions(r).children(r2);
        regions(ridx).parent = r;
    end
    checked_child(regions(r).children) = true;
end

% get skeleton
%disp('get skeleton...');
%regions = getSkeleton(regions);
%disp('generate regions:distance transform...');
regions = getDT(regions);

disp([num2str(numel(regions)) ' segments obtained']);

        
