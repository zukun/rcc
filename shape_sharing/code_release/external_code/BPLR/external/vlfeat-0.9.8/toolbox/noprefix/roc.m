function varargout = roc(varargin)
% VL_ROC Compute ROC curve
%  [TP,TN] = VL_ROC(Y, SCORE) computes the ROC curve for the specified
%  data. Y are the ground thruth labels (+1 or -1) and SCORE is the
%  discriminant score assigned to them by a classifier (higher scores
%  correspond to positive labels).
%
%  [TP,TN] are the true positive and true negative rates for
%  incereasing values of the decision threshold.
%
%  [TP,TN,INFO] = VL_ROC(...) returns the following additional
%  information:
%
%  INFO.EER::       Equal error rate.
%  INFO.AUC::       Area under the VL_ROC (AUC).
%  INFO.UR::        Uniform prior best op point rate.
%  INFO.UT::        Uniform prior best op point threhsold.
%  INFO.NR::        Natural prior best op point rate.
%  INFO.NT::        Natural prior best op point threshold.
%
%  VL_ROC(...) plots the VL_ROC diagram in the current axis.
%
%  About the ROC curve::
%    Consider a classifier that predicts as positive all lables Y
%    whose SCORE is not smaller than a threshold R. The ROC curve
%    represents the performance of such classifier as the threshold R
%    is varied. Let:
%
%      #P   = num of positive samples
%      #N   = num of negative samples
%      #TP = num of samples that are correctly classified as positive
%      #TN = num of samples that are correctly classified as negative
%      #FP = num of samples that are incorrectly classified as positive
%      #FN = num of samples that are incorrectly classified as negative
%
%    Consider also the rates:
%
%                TP = #TP / #P,      FN = #FN / #P,
%                TN = #TN / #N,      FP = #FP / #N.
%
%    Notice that:
%
%                 P = #TP + #FN ,    N = #TN + #FP,
%                 1 = TP + FN,       1 = TN + FP.
%
%    The ROC curve is the parametric curve (TP, TN) obtained as the
%    classifier threshold R is varied.
%
%    The ROC curve is contained in the square with vertices (0,0) The
%    (average) ROC curve of a random classifier is a line which
%    connects (1,0) and (0,1).
%
%    The ROC curve is independent of the prior probability of the
%    labels Y (i.e. of #P/(#P+#N) and #N/(#P+#N)).
%
%    An OPERATING POINT is a point on the ROC curve, corresponding to
%    a certain threshold R. Each operating point corresponds to
%    minimizing the empirical 01 error of the classifier for given
%    prior probabilty of the labels Y. VL_ROC() computes the following
%    operating points:
%
%     Natural operating point:: Assumes P[Y=+1] = P/(P+N).
%     Uniform operating point:: Assumes P[Y=+1] = 1/2.
%
%   Options:
%
%   Plot::
%     'TrueNegative' plots the true positives vs. the true negatives.
%     'FalseNegative' plots the true positives vs. the false
%     negatives.
%
%  See also:: VL_HELP().
[varargout{1:nargout}] = vl_roc(varargin{:});
