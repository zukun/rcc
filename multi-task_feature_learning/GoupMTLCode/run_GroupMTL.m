%% MTL Group learning on Toy data
clear all; close all;
load 'toy.mat'

%% parameters
numGroups = 3; % Number of task groups
gamma = 1e-1;  % Regularization coefficient
epsilon = 1e-1;

%% Initialization
Dini = eye(dim)/dim;
groupD = cell(numGroups,1); % the D matrix for each group
% reset the groupD and Q
Q = zeros(numGroups,T); Q(1,:) = 1; % initialize the tasks into one group
groupD{1} = Dini;
fD_isqrt = sqrtm(Dini);
for i = 2:numGroups
    groupD{i} = eye(dim);
end

%% Main Code
maxIterQ = 5; % MAX out-loop iterations
maxIterW = 5; % MAX inner-loop iterations
for iter = 1:maxIterQ % out-loop
    for iter2 = 1:maxIterW % inner-loop
        % Use variable transform to solve the regularization problem
        for t = 1:T
            invDt = zeros(dim,dim);
            vec_invDt = zeros(dim,1);
            % get the D matrix for this task
            for g = 1:numGroups
                invDt = invDt + Q(g,t)*inv(groupD{g});
            end
            Dt_isqrt = sqrtm(inv(invDt));
            % get the data for this task
            x = Dt_isqrt * trainx(:,(t-1)*m_tr+1:t*m_tr);
            y = trainy((t-1)*m_tr+1:t*m_tr)';
            K = x'*x;
            [w] = kernel_regression(K,y,gamma,x);
            W(:,t) = Dt_isqrt * w;
        end
        % get the D matrix for each group
        for g = 1:numGroups
            sqrtQ = sqrt(diag(Q(g,:)));
            WsqrtQ = W * sqrtQ;
            groupD{g} = (fastSqrtD(WsqrtQ, epsilon))^2;
        end
    end
    % Group learning
    [Q,Obj] = getFastGroupQ(W, epsilon, numGroups);
end

%% test the accuracy
err = 0;
for t = 1:T
    x = testx(:,(t-1)*m_test+1:t*m_test);
    y = testy((t-1)*m_test+1:t*m_test)';
    pred_y = x'*W(:,t);
    err = err + sqrt(((pred_y - y)'*(pred_y - y))/m_test);
end
err = err/T
imagesc(Q)