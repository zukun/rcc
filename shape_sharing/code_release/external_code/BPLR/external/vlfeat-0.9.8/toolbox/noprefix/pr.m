function varargout = pr(varargin)
% VL_PR Compute precision-recall curve
%  [RECALL, PRECISION] = VL_PR(Y, SCORES) computes the precision-recall
%  (PR) curve of the specified data. Y are the ground thruth labels
%  (+1 or -1) and SCORE is the discriminant score associated to the
%  data by a classifier (lager scores correspond to positive
%  guesses). 
%
%  Remark:: Assign -INF score to data which is never retrieved
%    (this will result in maximum recall < 1).
%
%  RECALL and PRECISION are the recall and the precision for
%  increasing values of the decision threshold.
%
%  About the PR curve::
%    We use the same symbols as for the VL_ROC() function. We define the
%    quantities
%
%      P = TP / (TP + FP) = precision
%      R = TP / P = recall
%
%    The precision P is the fraction of positivie predictions which
%    are correct, and the recall R is the fraction of trurly positive
%    labels that have been correctly classified (recalled).
%
%    Notice that the recall is also equal to the true positive rate in
%    a ROC curve (see VL_ROC()).
%
%  Remark:: precision (P) is undefined for those values of the
%    classifier threshold for which no example is classified as
%    positive. Conventionally, we assign a precision of P=1 to such
%    cases.
%
%  See also:: VL_ROC(), VL_HELP().
[varargout{1:nargout}] = vl_pr(varargin{:});
