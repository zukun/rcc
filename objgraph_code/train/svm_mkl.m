function prob_estimates = svm_mkl(C,valclasslabels,featType)
%
% learns kernel weights using MKL, and trains SVM classifiers 
%
% Inputs: C (svm parameter)
%         valclasslabels (known class labels)
%         featType (struct that stores the kernel matrices, classlabels,
%           segmentation indices, segment indices, training and testing indices)
%
% Outputs: prob_estimates (N x L matrix of posterior probabilities where N 
%            is the number of test instances, and L is the number of known categories)
%

% load the train vs train matrix
Ktrainstack = [];
for i=1:length(featType)
    Ktrainstack = [Ktrainstack; featType(i).KTrain];   
    Kstack(:,:,i) = [featType(i).KTrain; featType(i).KTrainTest'];
end    
labels = featType(1).labels;
traininds = featType(1).traininds;
testinds = featType(1).testinds;

TRAINLABELS = labels(traininds);
prob_estimates = zeros(length(testinds),length(valclasslabels));

% one-vs-all weight learning and SVM
for i=1:length(valclasslabels)
    
    Y = zeros(size(TRAINLABELS));
    Y(TRAINLABELS==valclasslabels(i)) = 1;
    Y(TRAINLABELS~=valclasslabels(i)) = -1;
    
    [SKM] = learn_SKM2(C,Y,traininds,traininds,Ktrainstack);
    classweights(i).etas_tilde = SKM.etas_tilde;

    % obtain new weighted Kernel matrix
    thisK = zeros(size(Kstack(:,:,1)));
    for j=1:length(SKM.etas_tilde)
        thisK = thisK + Kstack(:,:,j)*SKM.etas_tilde(j);
    end
    
    % just to make sure
    for j=1:size(Ktrainstack,2)
        thisK(j,j) = 1;
    end
    
    Ktrain = thisK(traininds,traininds);
    Ktest = thisK(testinds,traininds);

    Ktrain = [(1:size(Ktrain,1))' Ktrain];
    Ktest = [(1:size(Ktest,1))' Ktest];

    Z = zeros(size(labels));
    Z(labels==valclasslabels(i)) = 1;
    Z(labels~=valclasslabels(i)) = -1;
    
    % set the training labels
    trainlabels = Z(traininds);
    % set the testing labels
    testlabels = Z(testinds);

    % perform SVM (one vs one)
    fprintf('Training...\n');
    model = svmtrain(trainlabels, Ktrain, ['-s 0 -c ' num2str(C) ' -t 4 -b 1']);

%     fprintf('Testing on train set...\n');
%     [predict_label1, accuracy1, prob_estimates1] = svmpredict(trainlabels, Ktrain, model, '-b 1');

    fprintf('Testing on test set...\n');
    [predict_label2, accuracy2, prob_estimates2] = svmpredict(testlabels, Ktest, model, '-b 1');

    % this is to make sure the +1 class output values are obtained
    if Z(1)==1
        firstind = 1;
    else
        firstind = 2;
    end
    prob_estimates(:,i) = prob_estimates2(:,firstind);

end
