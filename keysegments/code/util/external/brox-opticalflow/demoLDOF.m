clear;

im1 = double(imread('tennis492.ppm'));
im2 = double(imread('tennis493.ppm'));

tic;
flow = mex_LDOF(im1,im2);
toc;
