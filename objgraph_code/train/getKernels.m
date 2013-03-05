function featType = getKernels(trainftype,testftype,rmind,basedir)
%
% computes chi-squared kernels for train vs. train and train vs. test
% instances. 
%
% Inputs: trainftype (filename containing list of Training instances)
%         testftype (filename containing list of Testing instances)
%         rmind (class indices of 'void' and unknown categories)
%         basedir (I/O directory)
%
% Outputs: featType (struct that stores the kernel matrices, classlabels,
%          segmentation indices, segment indices, training and testing indices)
%


F1 = getFeatures_GT(trainftype,rmind,basedir);
F2 = getFeatures_multisegs(testftype,basedir);

Ftrain(1).feats = F1.textonfeats;
Ftrain(2).feats = F1.colorfeats;
Ftrain(3).feats = F1.phogfeats;

Ftest(1).feats = F2.textonfeats;
Ftest(2).feats = F2.colorfeats;
Ftest(3).feats = F2.phogfeats;

for i=1:length(Ftrain)
    % normalize training features
    trainfeats = normalizeFeats(Ftrain(i).feats);
    
    % normalize test features
    testfeats = normalizeFeats(Ftest(i).feats);  

    % chisq kernel (train vs train) (train vs test); gamma = 1
    featType(i).KTrain = exp(-slmetric_pw(trainfeats', trainfeats', 'chisq'));
    featType(i).KTrainTest = exp(-slmetric_pw(trainfeats', testfeats', 'chisq'));
end

classlabels1 = F1.classlabels;
classlabels2 = F2.classlabels;

featType(1).trainimlabels = F1.imlabels;
featType(1).testimlabels = F2.imlabels;
featType(1).testseglabels = F2.seglabels;
featType(1).testmultiseglabels = F2.multiseglabels;

featType(1).labels = [classlabels1; classlabels2];
featType(1).traininds = 1:length(classlabels1);
featType(1).testinds = 1+length(classlabels1):length(featType(1).labels);

