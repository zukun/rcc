function varargout = imdown(varargin)
% VL_IMDOWN  Downsample image 
%   J = VL_IMDOWN(I,'sample') downsamples the image I by half by
%   discarding each other pixel. This is the default downsampling
%   method.
%   
%   VL_IMDOWN(I,'avg') downsamples by averaging groups of four pixels.
%   
%   See also VL_IMUP(), VL_HELP().
[varargout{1:nargout}] = vl_imdown(varargin{:});
