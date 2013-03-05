function [Q,Obj] = getFastGroupQ(W, epsilon, numGroups)
% get the Q vector for each group by minizing the sum of their trace-norm
% Zhuoliang Kang 1/14/2011

[U, SIGMA, V] = svd(W, 0);
SigmaV = SIGMA*V';

dim = size(W,1);    % num of features
T = size(W,2);      % num of total tasks
G = numGroups;      % num of groups
e = exp(1);         % e = 2.173...

if numGroups == 1
    currentQ = ones(1,T);
    [HValue,TraceObj,PP] = computeHValue(SigmaV, currentQ, epsilon, dim, T, 1);
    Q = currentQ;
    Obj = HValue;
    return;
elseif numGroups<0
    fprintf('# of groups is negative!\n');
    keyboard
elseif numGroups>T
    fprintf('# of groups is larger than # of total tasks!\n');
    keyboard
end

% randomly intialize Q and Alpha
currentAlpha = rand(G,T);
eToAlpha = e.^currentAlpha;
currentQ = eToAlpha./repmat(sum(eToAlpha),G,1); % not sure about the usage of SLOPE here

% ARMIJO RULE 
ARMIJO_C1 = 1e-4;
ARMIJO_STEPSIZE= 1;
ARMIJO_BETA = 0.3;
maxIter = 200;
SLOPE = 1;

% prepare variables
gradHAlpha = zeros(G,T);
gradHAlpha1 = zeros(G,T);
gradHAlpha2 = zeros(G,T);
gradHq = zeros(G,T);
gradqAlpha1 = zeros(G,T); % the grad when q and t are the same
S = zeros(G,T); % the sum of eToAlpha miuse the q* term
TraceObj = zeros(G,1);
HValue = zeros(G,1);
currentObj = sum(HValue);

% gradient descent
for iter = 1:maxIter
    % compute current gradient with respect to Q
    for idxGroup = 1:G
        [HValue(idxGroup), TraceObj(idxGroup), PP] = computeHValue(SigmaV, currentQ, epsilon, dim, T, idxGroup);
        for idxTask = 1:T
            gradHq(idxGroup,idxTask) = TraceObj(idxGroup)*SigmaV(:,idxTask)'*PP*SigmaV(:,idxTask);
        end
    end
    currentObj = sum(HValue);
    % computer current gradient with repsect to Alpha
    eToAlpha = e.^currentAlpha;
    sumeToAlpha2 = sum(eToAlpha).^2;
    S = repmat(sum(eToAlpha),G,1) - eToAlpha;
    gradqAlpha1 = S./(((1+S./eToAlpha).^2).*eToAlpha);
    gradHAlpha1 = gradHq.*gradqAlpha1;
    gradHAlpha2 = zeros(G,T);
    for idxGroup = 1:G
    for idxTask = 1:T
        for g = 1:G
            if(g ~= idxGroup)
                gradHAlpha2(idxGroup,idxTask) = gradHAlpha2(idxGroup,idxTask) + gradHq(g,idxTask)*(-eToAlpha(g,idxTask)*eToAlpha(idxGroup,idxTask))...
                    /sumeToAlpha2(idxTask); 
            end
        end
    end
    end
    gradHAlpha = (gradHAlpha1 + gradHAlpha2)*SLOPE;
    gradA2 = sum(sum(gradHAlpha.^2));
    % step search with Armijo rule
    stepsize = ARMIJO_STEPSIZE;
    for steps = 1:10
        newAlpha = currentAlpha - stepsize*gradHAlpha;
        neweToAlpha = e.^newAlpha;
        newQ = neweToAlpha*SLOPE./repmat(sum(neweToAlpha),G,1); % not sure about the usage of SLOPE here
        for idxGroup = 1:G
            [HValue(idxGroup), TraceObj(idxGroup), PP] = computeHValue(SigmaV, newQ, epsilon, dim, T, idxGroup);
        end
        newObj = sum(HValue);
        if (newObj - currentObj) < ARMIJO_C1*stepsize*gradA2
            currentObj = newObj;
            currentQ = newQ;
            currentAlpha = newAlpha;
            break;
        else
            stepsize = stepsize*ARMIJO_BETA;
            if stepsize < eps
                if quiet==0
                    fprintf('\t\t-->Armijo step size too small\n');
                end
                break;
            end
        end        
    end
    if stepsize < eps
        break;
    end    
end
Q = currentQ;
Obj = currentObj;
return


% get the derivative of H_idxGroup with q
function [hValue, traceObj, PP] = computeHValue(SigmaV, Q, epsilon, dim, T, idxGroup)
A = SigmaV*diag(Q(idxGroup,:))*SigmaV' + epsilon*eye(min(dim,T));
[P, LAMBDA] = eig(A);
LAMBDA = diag(LAMBDA);
sqrtLAMBDA = sqrt(LAMBDA);
traceObj = sum(sqrtLAMBDA) + sqrt(epsilon)*(dim-T);
hValue = traceObj*traceObj;
PP = P*diag(1./sqrtLAMBDA)*P';
return

