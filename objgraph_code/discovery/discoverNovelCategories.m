function discoverNovelCategories(discoveryLabels,featTypes,nbClusters,NUMRESTARTS,outsubdir,testftype,voidind,valclasslabels,numSp)
%
% Computes entropy scores for all regions and takes those with scores > 0.5*max(range_of_entropy_value) to be unknown. 
% Then, for each unknown region, an object-graph descriptor is computed,
% and finally the regions are grouped using appearance and obj-graphs.
%
% Inputs: discoveryLabels (unknown category labels)
%         featTypes (name of features types for saving as output file, e.g., {'combined','appearance'})
%         nbClusters (number of categories to discover)
%         NUMRESTARTS (number of runs)
%         outsubdir (output files saved in [basedir outsubdir])
%         testftype (file containing image names of test data)
%         voidind ('void' classes, e.g., void, horse, mountain (not used in MSRC-v2))
%         valclasslabels (known category labels)
%         numSp (number of superpixel regions to consider above and below for object-graph)
%

global basedir

%% set entropy threshold to be mid-value of range
threshold = 0.5*log2(length(valclasslabels));
outdir = [basedir outsubdir 'threshold=' num2str(threshold) '/'];
mkdir(outdir);

%% get entropy scores for test set and select unknown segments from each image 
fprintf('computing entropy scores on testset and removing known segments...');
unknownSegs = getUnknownRegions_multiSegs(testftype,threshold);
fprintf('done\n');

%% get obj-graph (probability distribution) features for each retained
%% segment 
fprintf('computing object-graph features...');
im_region = getRegionFeatures_multiSegs(testftype,unknownSegs,numSp);
fprintf('done\n');

classlabel = [];
confeats = [];
for i=1:length(im_region)  
    confeats = [confeats; im_region(i).feats];
    classlabel = [classlabel; im_region(i).classlabel];   
end
confeats = normalizeFeats(confeats);

%% get appearance features for each segment
fprintf('computing appearance features...');
[F] = getFeatures_unknownSegs(testftype,unknownSegs);
fprintf('done\n');

textonfeats = F.textonfeats;
colorfeats = F.colorfeats;
phogfeats = F.phogfeats;
classlabels = F.classlabels;
imlabels = F.imlabels;
seglabels = F.seglabels;
multiseglabels = F.multiseglabels;

if ~isequal(classlabel,classlabels)
    keyboard;
end

%% normalize each appearance feature
textonfeats = normalizeFeats(textonfeats);
colorfeats = normalizeFeats(colorfeats);
phogfeats = normalizeFeats(phogfeats);

%% remove void regions
ind = [];
for i=1:length(voidind)
    ind = [ind; find(classlabels==voidind(i))];
end

confeats(ind,:) = [];
textonfeats(ind,:) = [];
colorfeats(ind,:) = [];
phogfeats(ind,:) = [];
classlabels(ind) = [];
imlabels(ind,:) = [];
seglabels(ind,:) = [];
multiseglabels(ind,:) = [];

%% compute chi-square kernels
fprintf('computing chi-square kernels...');
A1 = exp(-slmetric_pw(confeats', confeats', 'chisq'));
A2 = exp(-slmetric_pw(textonfeats', textonfeats', 'chisq'));
A3 = exp(-slmetric_pw(colorfeats', colorfeats', 'chisq'));
A4 = exp(-slmetric_pw(phogfeats', phogfeats', 'chisq'));
fprintf('done\n');

clusterClasslabels = [valclasslabels discoveryLabels];

%% cluster unknown regions and compute purity
fprintf('clustering...');
for nn = 1:length(featTypes)
    featType = char(featTypes(nn));
    
    if nn==1 % obj-graph (full model)     
        A = 1/2*A1 + 1/6*A2 + 1/6*A3 + 1/6*A4;
    else % appearance only        
        A = 1/3*A2 + 1/3*A3 + 1/3*A4;
    end

    % spectral clustering
    for i=1:size(A,1)
       A(i,i) = 0; 
    end
    D = zeros(size(A));
    for i=1:size(A,1)
        D(i,i) = sum(A(i,:));
    end   
    L = D^(-.5)*A*D^(-.5);
    [V,D] = eig(L);
%     [V,D] = eigs(L,nbClusters(end));
    
    for k=1:length(nbClusters)
        nbCluster = nbClusters(k);

        clear Y
        X = V(:,1:nbCluster);
        for i=1:size(X,1)
            Y(i,:) = X(i,:)./norm(X(i,:));
        end
            
        for l=1:NUMRESTARTS            
            [clusterindex,dummy,dummy] = kmeansML(nbCluster,Y');          

            clear clusters
            for i=1:nbCluster
                clusters(i).count = zeros(length(clusterClasslabels),1);
                temp = classlabels(clusterindex==i);
                for j=1:length(clusterClasslabels)
                    clusters(i).count(j) = length(find(temp==clusterClasslabels(j)));
                end
            end

            numregions = zeros(length(clusterClasslabels),1);
            for i=1:length(clusters)
                numregions = numregions + clusters(i).count;
            end
            N = sum(numregions);    % total number of regions

            % get purity: i- class, j- cluster
            numClasses = length(clusterClasslabels);
            precision = zeros(numClasses,nbCluster);
            for i=1:numClasses
                for j=1:nbCluster
                    precision(i,j) = clusters(j).count(i)/sum(clusters(j).count);
                end
            end

            purity = 0;
            for j=1:nbCluster
                clusterpurity = max(precision(:,j)) * sum(clusters(j).count) / N;
                purity = purity + clusterpurity;   
            end

            iter(l).clusterindex = clusterindex;
            iter(l).clusters = clusters;
            iter(l).purity = purity;
            iter(l).precision = precision;

            fprintf('done with iter %d for nbCluster %d\n', l, nbCluster);
        end

        save('-v7',[outdir 'k=' num2str(nbCluster) '_' featType '.mat'],'iter','imlabels','seglabels','multiseglabels','classlabels');
        clear iter
    end
    save('-v7',[outdir 'affinitymat_' featType '.mat'],'A');
end
fprintf('done!');

