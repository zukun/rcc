function regionToPixelProbability(ftype,numclasses,basedir,outdir,featType,prob_estimates)
%
% converts region-level posterios computed on segments to pixel-level posteriors 
%
% Inputs: ftype (filename containing list of Testing instances)
%         numclasses (number of known categories)
%         basedir (main I/O directory)
%         outdir (directory to save pixel-level posteriors)
%         featType (struct that stores segmentation indices, segment indices, 
%           training and testing indices)
%         prob_estimates (N x L matrix of posterior probabilities where 
%           N is the number of test instances, and L is the number of known categories)
%

% global variables
global segdir

testimlabels = featType(1).testimlabels;
testseglabels = featType(1).testseglabels;
testmultiseglabels = featType(1).testmultiseglabels;

% normalize prob_estimates
normval = sum(prob_estimates,2);
prob_estimates = prob_estimates./repmat(normval,[1 size(prob_estimates,2)]);

[B,count] = readTrainValTestIndices(ftype,basedir);

num = 1;
for i=1:length(B)
    imname = char(B{i});
    load([segdir imname '.seg.mat'], 'Segs');
 
    for j=1:size(Segs,3)
        prim_multiSegs(j).prim = zeros(size(Segs,1),size(Segs,2),numclasses);
    end   
    
    while num<=length(testimlabels) && strcmp(testimlabels(num),imname)         
        multisegnum = testmultiseglabels(num);
        thisSeg = double(Segs(:,:,multisegnum));
        ind = find(thisSeg==testseglabels(num));

        thisim = zeros(size(thisSeg));
        for j=1:numclasses            
            thisim(ind) = prob_estimates(num,j);
            prim_multiSegs(multisegnum).prim(:,:,j) = prim_multiSegs(multisegnum).prim(:,:,j) + thisim;            
        end       
        num = num + 1;       
    end
    
    save('-v7', [outdir imname '.mat'],'prim_multiSegs');
    clear prim_multiSegs
end


    