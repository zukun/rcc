function varargout = imarray(varargin)
% VL_IMARRAY  Flattens image array
%   J=VL_IMARRAY(A) flattens the array of images A. A can be either a
%   M*N*K array, storing one gray-scale image per slice, or a M*N*3*K
%   or M*N*K*3 array, storing one RGB image per slice.  The function
%   returns an image J which is a tiling of the images in the array.
%
%   VL_IMARRAY(...) display the image J rather than returning it.
%
%   VL_IMARRAY() accepts the following options:
%
%   'Spacing' [0]::
%     Orlates the images with a null border of the specified width.
%
%   'Layout' [[]]::
%     Specify a vector [TM TN] with the number of rows and columns of
%     the tiling. If equal to [] the layout is computed automatically.
%
%   'Movie' [0]::
%     Display/returns a movie rather than generating a tyling.
%
%   'CMap' [[]]::
%     Specify a colormap for indexed images and movies.
%
%   See also VL_IMARRAYSC(), VL_HELP().
[varargout{1:nargout}] = vl_imarray(varargin{:});
