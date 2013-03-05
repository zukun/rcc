clear;

addpath(genpath('./code/util/external/BPLR/'));

rsz = 1.0; %300/max(size(im)); % resize image such that its longest dimension is 300 pixels.
order_k = 25;
euc_f = 2.5;
min_nseg = 20;
max_nseg = 200;
phog_L = 2;
n_angle_bin = 12;
n_length_bin = 20;
magnif = 1.1;
min_elem_scale = 4;

% videoName = 'birdfall2'; grid_space = 12; 
% videoName = 'cheetah'; grid_space = 6;
% videoName = 'girl'; grid_space = 12;
videoName = 'monkeydog'; grid_space = 6;
% videoName = 'parachute'; grid_space = 12;
% videoName = 'penguin'; grid_space = 12;

outdir = ['./data/bplr/' videoName '/'];
imdir = ['./data/SegTrack/' videoName '/'];

d = dir([imdir '/*.jpg']);
if isempty(d)
    d = dir([imdir '/*.png']);
end
if isempty(d)
    d = dir([imdir '/*.bmp']);
end

for i=1:numel(d)
    img_file = [outdir d(i).name];
    im = imread(img_file);

    [BPLR, pb_phog, color_hist] =...
        demo(img_file, rsz, order_k, euc_f, min_nseg, max_nseg, grid_space, min_elem_scale, magnif, phog_L, n_angle_bin, n_length_bin);
    
    save('-v7',[outdir d(i).name '_bplr.mat'],'BPLR', 'pb_phog', 'color_hist');
    i
end
