function varargout = noprefix(varargin)
% VL_NOPREFIX  Creates a prefix-less version of VLFeat commands
%
%   Author:: Andrea Vedaldi
[varargout{1:nargout}] = vl_noprefix(varargin{:});
