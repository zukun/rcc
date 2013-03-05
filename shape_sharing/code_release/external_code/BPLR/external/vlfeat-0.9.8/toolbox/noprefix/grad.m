function varargout = grad(varargin)
% VL_GRAD Compute the gradient of an image
%   [Ix,Iy] = VL_GRAD(I) returns the finite differences gradient of
%   the image I. I must be a 2D array. The function uses central
%   differences and for all but the boundaries pixels, for which it
%   uses forward/backward differences as appropriate.
%
%   VL_GRAD(I,'forward') uses only forward differences (except on the
%   lower and right boundaries, where backward difference is used).
%   Similarly, VL_GRAD(I,'backward') uses only backward differences.
%
%   See also: GRADIENT(), VL_HELP().
[varargout{1:nargout}] = vl_grad(varargin{:});
