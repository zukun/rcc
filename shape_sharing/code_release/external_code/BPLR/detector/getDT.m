function regions = getDT(regions)

for r = 1 : numel(regions)
    img = false(regions(r).img_size);
    img(regions(r).contour) = true;
    dt = bwdist(img);
    regions(r).dt = dt(regions(r).segment);
end
    
   