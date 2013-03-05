function sqrtD = fastSqrtD(W, epsilon)
% fast way to compute sqrt of D using thin SVD
%
%
[U, S] = svd(W, 0);
S = diag(S);
S = S .^ 2 ; %W*W': eigenvalues
S = S + epsilon; %W*W' + epsilon: eigenvalues
S = sqrt(S);    % square root of W*W'+epsilon: sqrt
totalTrace = sum(S) + sqrt(epsilon)*(size(W,1)-length(S));  % trace
S = S /totalTrace; % normalize such that D will have trace of 1
S = sqrt(S); % sqrtD's eigenvalues
shift = (sqrt(sqrt(epsilon)/totalTrace));
sqrtD = U*diag(S-shift)*U'+ shift*eye(size(W,1));
return
