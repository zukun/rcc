function [Segs,Nsegs,Res] = multipleSegmentations(img)
% Segs = multipleSegmentations(img,Nsegs,Res)
%
% Compute multiple segmentations for an image using Ncuts.  We use the
% Ncuts code provided here:
%
% http://www.cis.upenn.edu/~jshi/software/
%
% and described in:
%
% J. Shi and J. Malik.  Normalized Cuts and Image Segmentation.  IEEE
% Transactions on Pattern Analysis and Machine Intelligence (PAMI), 2000.
%
% Please be sure that this code is in the Matlab path.
%
% Inputs:
% img - Input image.
% Nsegs - Vector containing the number of segments to find for the
%   different segmentations.
% Res - Vector containing the image widths, in pixels.  The image will be
%   resized to these widths before running the segmentation algorithm.
%
% Outputs:
% Segs - 3D matrix, where each segmentation output is stored along the
%   third dimension.
%
%% This code is by Bryan C. Russell
%% used/modified by Yong Jae Lee

% Parameters:
if nargin < 3
  Res = [150];
end

if nargin < 2
  Nsegs = [3:12];
end

if size(img,3)>1
  img = rgb2gray(img);
end
Segs = zeros(size(img,1),size(img,2),length(Nsegs),'uint8');
j = 0;
for r = Res
  imgResize = imresize(double(img),min(1,r/size(img,2)));
  for s = Nsegs
    j = j+1;
    if s > 1
      Segs(:,:,j) = imresize(NcutImage(imgResize,s),size(img),'nearest');
    else
      Segs(:,:,j) = ones(size(img),'uint8');
    end
  end
end
