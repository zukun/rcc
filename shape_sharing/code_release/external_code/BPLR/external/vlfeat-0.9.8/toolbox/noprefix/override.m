function varargout = override(varargin)
% VL_OVERRIDE  Override structure subset
%   CONFIG = VL_OVERRIDE(CONFIG, UPDATE) copies recursively the fileds of
%   the structure UPDATE to the corresponding fields of the struture
%   CONFIG.
%
%   While CONFIG and UPDATE can arbitrary structures, usually CONFIG
%   is a list of defaul parameters and UPDATE is a list of parameter
%   updates.
%
%   VL_OVERRIDE(..., 'warn') prints a warning message whenever copying
%   UPDATE adds new fileds to CONFIG, or overrides a non-empty
%   substucture.
%
%   VL_OVERRIDE(..., 'skip') does not add any new parameters but only
%   updates existing ones.
%
%   VL_OVERRIDE(..., 'casei') matches field names up to their case.
%
%   Remark:: Each field path of UPDATE is matched as deeply as
%    possible to a path of CONFIG.  Two fields A(IA).(FA) and B(IA).FB
%    match if, and only if, (i) the struct arrays A and B have the
%    same dimensions, (ii) the indeces IA and IB are equal, and the
%    field names FA and FB are equal.
%
%   See also:: VL_HELP().
[varargout{1:nargout}] = vl_override(varargin{:});
