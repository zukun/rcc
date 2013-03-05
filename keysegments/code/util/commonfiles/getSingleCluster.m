function [final_indic_vec, V, D] = getSingleCluster(A, vecnum, V, D)
    
if nargin<3
    [V,D] = eig(A);
%     [V,D] = eigs(A);
end

eigval = diag(D);
% [eigval,ind] = min(eigval);
% [eigval,ind] = max(eigval);
% eigvec = V(:,ind);  
[eigval,ind] = sort(eigval,'descend');
eigvec = V(:,ind(vecnum));  

% [eigvec,eigval] = eigs(A,1);


% 1) discretization of continuous indicator vector
[temp_eigvec tempind] = sort(eigvec,'descend');
indic_vec = zeros(length(eigvec),1);
num = 0;
for i=1:length(eigvec)
    indic_vec(i) = 1;
    curr_num = (indic_vec'*temp_eigvec)/norm(indic_vec);

    if num < curr_num
        num = curr_num;
        final_vec = indic_vec;
    end
end

if exist('final_vec','var')
    final_indic_vec = zeros(length(eigvec),1);
    final_indic_vec(tempind) = final_vec;
else
    
    % 2)
    [temp_eigvec tempind] = sort(eigvec,'descend');
    indic_vec = zeros(length(eigvec),1);
    tempval = 0;

    for i=1:length(tempind)
        indic_vec(tempind(i)) = 1;

        val = (indic_vec'*A*indic_vec)/i;
        if tempval < val
            tempval = val;
            final_indic_vec = indic_vec;
        end
    end
end
