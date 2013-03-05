function [ptrs scales ptrs_region] = uniformSamplePtrsFromRegions_(regions, grid_spacing, supp_thresh)

% addpath('../util');

ptrs = [];
scales = [];
ptrs_region = [];
% sample by distance transform
n_region = numel(regions);
dt = cell(1,n_region);
bbox = cell(1,n_region);

for r = 1 : n_region

    dt{r} = zeros(regions(r).img_size);
    dt{r}(regions(r).segment) = regions(r).dt;
    
    lx = max([floor(regions(r).boundbox(1)), 1]);
    rx = min([ceil(regions(r).boundbox(3)), regions(r).img_size(2)]);
    ty = max([floor(regions(r).boundbox(2)), 1]);
    dy = min([ceil(regions(r).boundbox(4)), regions(r).img_size(1)]);
    
    bbox{r} = [lx rx ty dy];
end
[dt_max sample_ptrs] = SamplePtrsFromRegionsMex(dt, bbox, grid_spacing);

for r = 1 : n_region
    
    dt_max_vals = dt_max{r};
    sample_x = sample_ptrs{r}(1,:);
    sample_y = sample_ptrs{r}(2,:);
    
    nz_inds = dt_max_vals > supp_thresh;
    nnz = sum(nz_inds(:));
    sample_x = sample_x(nz_inds);
    sample_y = sample_y(nz_inds);
    dt_max_vals = dt_max_vals(nz_inds);    
    
    nxy = [sample_x(:)';sample_y(:)'];
    
    ptrs = [ptrs nxy];
    scales = [scales dt_max_vals(:)'];
    ptrs_region = [ptrs_region r*ones(1, nnz)];
    
end

% for r = 1 : numel(regions)
% 
%     dt = zeros(regions(r).img_size);
%     dt(regions(r).segment) = regions(r).dt;
%     lx = max([floor(regions(r).boundbox(1)), 1]);
%     rx = min([ceil(regions(r).boundbox(3)), regions(r).img_size(2)]);
%     ty = max([floor(regions(r).boundbox(2)), 1]);
%     dy = min([ceil(regions(r).boundbox(4)), regions(r).img_size(1)]);
%     
%     [xx yy] = meshgrid(lx:grid_spacing:rx-grid_spacing+1, ty:grid_spacing:dy-grid_spacing+1);
%     [dt_max_vals, locs] = arrayfun(@(i,j)(max2(dt(i:i+grid_spacing-1, j:j+grid_spacing-1))), yy,xx);
%     
%     [sample_yy sample_xx] = ind2sub([grid_spacing, grid_spacing], locs);
%     sample_x = (sample_xx-1) + xx;
%     sample_y = (sample_yy-1) + yy;
%     nz_inds = dt_max_vals > supp_thresh;
%     nnz = sum(nz_inds(:));
%     sample_x = sample_x(nz_inds);
%     sample_y = sample_y(nz_inds);
%     dt_max_vals = dt_max_vals(nz_inds);    
%     
%     nxy = [sample_x(:)';sample_y(:)'];
%     
%     ptrs = [ptrs nxy];
%     scales = [scales dt_max_vals(:)'];
%     ptrs_region = [ptrs_region r*ones(1, nnz)];
%     
% end

%sore by scales
[scales inds] = sort(scales);
ptrs = ptrs(:,inds);
ptrs_region = ptrs_region(inds);

ptrs = ptrs';

%non-max suppression
sim = zeros(regions(1).img_size);
rim = zeros(regions(1).img_size);
ptr_inds = sub2ind(regions(1).img_size, ptrs(:,2), ptrs(:,1));
sim(ptr_inds) = scales;
rim(ptr_inds) = ptrs_region;
supp_radius = ceil(grid_spacing/2);
supp_im = nonmaxsupp_opt(sim, supp_radius, supp_thresh, 'sliding');
%supp_im = nonmaxsupp_opt(sim, grid_spacing, supp_thresh, 'distinct');
[r,c] = find(supp_im);
clear ptrs
ptrs(:,1) = c;
ptrs(:,2) = r;
ptr_inds = sub2ind(regions(1).img_size, ptrs(:,2),ptrs(:,1));
scales = sim(ptr_inds);
ptrs_region = rim(ptr_inds);
