function varargout = imseg(varargin)
% IMSEG Color an image based on the segmentation
%   ISEG = IMSEG(I,LABELS) Labels ISEG with the average color from I of 
%   each cluster indicated by LABELS
[varargout{1:nargout}] = vl_imseg(varargin{:});
