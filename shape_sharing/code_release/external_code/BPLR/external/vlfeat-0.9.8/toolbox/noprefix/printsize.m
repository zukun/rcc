function varargout = printsize(varargin)
% VL_PRINTSIZE  Set the print size of a figure
%   VL_PRINTSIZE(R) sets the PaperPosition property of a figure so
%   that the width is the fraction R of the current paper page width.
%
%   VL_PRINTSIZE(FIG,R) opearates on the specified figure FIG.
%
%   This command is useful to resize figures before printing them so
%   that elements are scaled to match the final figure size in
%   print. The function accepts the following optional arguments:
%
%   Aspect:: Set the figure aspect ratio (widht/height) to this value.
%
%   Reference:: If 'horizontal' the ratio R is the widht of the figure
%     over the width of the page. If 'vertical', the ratio R is the
%     height of the figure over the height of the page.
%
%   See also:: VL_HELP().
[varargout{1:nargout}] = vl_printsize(varargin{:});
