addpath('pwmetric/');
addpath('/v/filer4b/v17q010/jaechul/caltech101/demo_code/vlfeat-0.9.8/toolbox/');
vl_setup
mex_ext = mexext;
if ( ~exist(['mexStringMatchCore2.' mex_ext], 'file') )
    mex mexStringMatchCore2.cpp
end

search_ratio = 0.25;
min_sift_mag = 5.0;
display = true;

nstep = 8;
nsize = [4,6,8,10];
% test 1 - fast match time
img1 = imread('Faces_image_0026.jpg');
img2 = imread('Faces_image_0030.jpg');
ucm1 = imread('Faces_0026.jpg_ucm2.bmp');
sift1 = ExtractSIFT('Faces_image_0026.jpg', nstep, nsize);
sift2 = ExtractSIFT('Faces_image_0030.jpg', nstep, nsize);

% test 2 - medium match time
% img1 = imread('Faces_image_0058.jpg');
% img2 = imread('Faces_image_0051.jpg');
% ucm1 = imread('Faces_0058.jpg_ucm2.bmp');
% sift1 = ExtractSIFT('Faces_image_0058.jpg', nstep, nsize);
% sift2 = ExtractSIFT('Faces_image_0051.jpg', nstep, nsize);


% test 3 - long match time
% img1 = imread('bonsai_image_0003.jpg');
% img2 = imread('bonsai_image_0005.jpg');
% ucm1 = imread('bonsai_0003.jpg_ucm2.bmp');
% sift1 = ExtractSIFT('bonsai_image_0003.jpg', nstep, nsize);
% sift2 = ExtractSIFT('bonsai_image_0005.jpg', nstep, nsize);

% note
% we resize the image when extracting sift.
% images will be resized when computing matching, accordingly.
[matchscore fptrs1 fptrs2] =...
asymMatch(img1,img2,sift1,sift2,ucm1, min_sift_mag, search_ratio);

