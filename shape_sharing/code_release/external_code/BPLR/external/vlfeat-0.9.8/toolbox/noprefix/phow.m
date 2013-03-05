function varargout = phow(varargin)
% VL_PHOW  Extract PHOW features
%   [FRAMES, DESCRS] = VL_PHOW(IM) extracts PHOW features from the
%   image IM. This function is a wrapper around VL_DSIFT() and
%   VL_IMSMOOTH().
%
%   The PHOW descriptors where introduced in [1]. By default,
%   VL_PHOW() computes the gray-scale variant of the descriptor.  The
%   COLOR option can be used to compute the color variant instead.
%
%   Verbose:: false
%     Set to true to turn on verbose output.
%
%   Sizes:: [5 7 9 12]
%     Scales at which the dense SIFT features are extracted. Each
%     value is used as bin size for the VL_DSIFT() function.
%
%   Fast:: true
%     Set to false to turn off the fast SIFT features computation by
%     VL_DSIFT().
%
%   Step:: 2
%     Step (in pixels) of the grid at which the dense SIFT features
%     are extracted.
%
%   Color:: false
%     Set to true to compute PHOW-color rather than PHOW-gray.
%
%   LowContrastThreshold:: 0.002
%     Contrast threshold below which SIFT features are mapped to
%     zero. The input image is scaled to have intensity range in [0,1]
%     (rather than [0,255]) and this value is compared to the
%     descriptor norm as returned by VL_DSIFT().
%
%   See also VL_HELP(), VL_DSIFT().
[varargout{1:nargout}] = vl_phow(varargin{:});
