img_file = 'test_data/image_0026.jpg';
im = imread(img_file);
rsz = 300/max(size(im)); % resize image such that its longest dimension is 300 pixels.
order_k = 25;
euc_f = 2.5;
min_nseg = 20;
max_nseg = 200;
grid_space = 4;
phog_L = 2;
n_angle_bin = 12;
n_length_bin = 20;
magnif = 1.1;
min_elem_scale = 4;
[BPLR, phog] =...
    demo(img_file, rsz, order_k, euc_f, min_nseg, max_nseg, grid_space, min_elem_scale, magnif, phog_L, n_angle_bin, n_length_bin);
