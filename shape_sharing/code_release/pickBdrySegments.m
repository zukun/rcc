function bdry_tf = pickBdrySegments(segments, img_size)

n_seg = size(segments,2);
segments = reshape(segments,[img_size,n_seg]);
[y x] = arrayfun(@(idx)(find(segments(:,:,idx))), 1:n_seg, 'uniformoutput', false);
cx = cellfun(@mean,x);
cy = cellfun(@mean,y);

n_grid = 10;
x_grid_size = img_size(2)/n_grid;
y_grid_size = img_size(1)/n_grid;
x_bin = ceil(cx./x_grid_size);
y_bin = ceil(cy./y_grid_size);

bdry_tf = x_bin <= 2 | x_bin > n_grid-2 | y_bin <=2 | y_bin > n_grid-2;
