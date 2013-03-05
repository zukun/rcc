function [matchscore fptrs1_ fptrs2_] =...
    asymMatch(img1, img2, sift1, sift2, ucm1, minVariance, search_ratio)                                    

% fixed option                                       
display = true;
algorithm = 'varDistance';

tStartPreprocessing = tic;
matchscore = zeros(20,1);
fg_score = zeros(20,1);
bg_score = zeros(2,1);
fptrs1_ = [];
fptrs2_ = [];

nscales = numel(sift1);

h(1) = sift1(1).imheight;
w(1) = sift1(1).imwidth;
steps(1) = sift1(1).step;
h(2) = sift2(1).imheight;
w(2) = sift2(1).imwidth;
steps(2) = sift2(1).step;
ori_sift2 = sift2; % for later use

database.ptrs = [];
database.data = [];
for i = 1 : nscales
    database.ptrs = [database.ptrs sift2(i).ptrs];
    database.data = [database.data single(sift2(i).descriptors)];
end

% information from SIFT
%ori_nptrs1 = size(sift1(1).ptrs,2);
ori_nptrs2 = size(sift2(1).ptrs,2);
h = ceil(h);
w = ceil(w);
strength = [sift2.strength];% cut by query's distribution
medStrength = median(strength);
minVariance = min([0.5*medStrength, minVariance]);
maxVariance = max(strength);

%remove unnecessary points
idx1 = false(1,numel(sift1(1).strength));
idx2 = false(1,numel(sift2(1).strength));
% remove uniform areas
for i = 1 : nscales
     idx1 = idx1 | sift1(i).strength > minVariance;
     idx2 = idx2 | sift2(i).strength > minVariance;
end
for i = 1 : nscales
    sift1(i).descriptors = sift1(i).descriptors(:,idx1);
    sift1(i).strength = sift1(i).strength(:,idx1);
    sift1(i).ptrs = sift1(i).ptrs(:,idx1);
    sift2(i).descriptors = sift2(i).descriptors(:,idx2);
    sift2(i).strength = sift2(i).strength(:,idx2);
    sift2(i).ptrs = sift2(i).ptrs(:,idx2);
end

% using for a normalization
nptrs1 = sum(cell2mat(arrayfun(@(x)(size(x.ptrs,2)), sift1, 'UniformOutput', false)));
nptrs2 = sum(cell2mat(arrayfun(@(x)(size(x.ptrs,2)), sift2, 'UniformOutput', false)));
valmin = min([nptrs1, nptrs2]);
if ( valmin == 0 )
    return;
end

% fixed parameters 
nn_dist_thres_pixelwise = sqrt(1e5)*ones([4, size(sift1(1).ptrs,2)]);
pixelwise_distance_upperbound = 1.25*nn_dist_thres_pixelwise;
alpha_for_dist_adjustment_pixelwise = 1.125*ones(4, size(sift1(1).ptrs,2));

% normalize coordinates 
ruler1 = max([w(1), h(1)]);
ruler2 = max([w(2), h(2)]);
gridstep = mean([steps(1)/ruler1, steps(2)/ruler2]);
database.ptrs = database.ptrs./ruler2;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% params for DP = [max_allowable_deform, deform_weight,deform_range,
%                  ordering_weight1, ordering_weight2, ordering_range, 
%                  no_match_cost1, no_match_cost2,
%                  location_weight, location_threshold,
%                  matching_distance_weight];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
deform_weight = 1.0;
matching_distance_weight = 1.25;
no_match_cost = 1.25;
ordering_weight = 1.5;
location_weight = 1.0;
params1 = [gridstep*10.0, deform_weight, min([gridstep*10,0.5]),...
           ordering_weight, ordering_weight, 1.0,...
           0.0, no_match_cost,...
           location_weight, 0.25,...
           matching_distance_weight];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% read segmentation
ucm = ucm1(3:2:end, 3:2:end);
non_zero_response = ucm(ucm > 10.0);
mean_edge_strength = mean(non_zero_response(:));
threshold = 1.5*mean_edge_strength; % very ad-hoc, but working well.
labels = bwlabel(ucm <= threshold);
labels = imresize(labels, [h(1) w(1)], 'nearest');

tPreprocessing = toc(tStartPreprocessing);

tStartSIFTSearch = tic;

nlabels = max(max(labels));
refptrs = [];
matchptrs = [];
ncandidates = [];
labelinds = [];

nmatchptrs = size(database.ptrs,2);

k = ceil(search_ratio*nmatchptrs/nscales);
nptrs1 = sum(cell2mat(arrayfun(@(x)(size(x.ptrs,2)), sift1, 'UniformOutput', false)));
nptrs2 = sum(cell2mat(arrayfun(@(x)(size(x.ptrs,2)), sift2, 'UniformOutput', false)));
disp(['search SIFT (nQ: ' num2str(size(sift1(1).descriptors,2)*nscales) ', nDB: '  num2str(size(database.ptrs,2)) ')']);
matchinds = cell(1,nscales);
matchdists = cell(1,nscales);

for i = 1 : nscales
    d = slmetric_pw(database.data, single(sift1(i).descriptors), 'eucdist');
    [d sind] = sort(d,1);
    matchinds{i} = sind(1:k, :);
    matchdists{i} = double(d(1:k, :));
end
tSIFTSearch = toc(tStartSIFTSearch);

refidx = 1;
matchidx = 2;
valmin = min([nptrs1, nptrs2]);
if ( valmin == 0 )
    return;
end

tStartStringMatch = tic;
disp('string match');
best_dists = inf(1, nptrs1/nscales); % store each point's best SIFT distance, used for background match scoring.
seg_labels = labels;

nn_avg_dist_per_scale_in_query_image =...
    [261.5739;  244.9650; 237.3891;  232.6415];

for i = 1 : nlabels
    seg_labels(labels ~= i) = 0;
    seg_labels(labels == i) = 1;
    se = strel('disk',10);
    seg_labels = imdilate(seg_labels,se);
    ptr_inds = h(1)*(sift1(1).ptrs(1,:)-1) + sift1(1).ptrs(2,:);
    seg_ptr_inds = seg_labels(ptr_inds) == 1;    
    seg_dist_thres_pixelwise =...
        nn_dist_thres_pixelwise(:, seg_ptr_inds);
    seg_dist_thres_upperbound_pixelwise =...
        pixelwise_distance_upperbound(:, seg_ptr_inds);
    seg_alpha_pixelwise =...
        alpha_for_dist_adjustment_pixelwise(:, seg_ptr_inds);
    seg_matchinds = cell(1, nscales);
    seg_matchdists = cell(1, nscales);
    for j = 1 : nscales
        seg_sift1(j).ptrs = sift1(j).ptrs(:,seg_ptr_inds);
        seg_sift1(j).descriptors = sift1(j).descriptors(:,seg_ptr_inds);
        seg_sift1(j).strength = sift1(j).strength(seg_ptr_inds);
        seg_sift1(j).ptrs(1,:) = seg_sift1(j).ptrs(1,:)./ruler1;
        seg_sift1(j).ptrs(2,:) = seg_sift1(j).ptrs(2,:)./ruler1;
        
        seg_matchinds{j} = matchinds{j}(:, seg_ptr_inds);
        seg_matchdists{j} = matchdists{j}(:, seg_ptr_inds);
    end

    seg_nptrs1 =...
        sum(cell2mat(arrayfun(@(x)(size(x.ptrs,2)), seg_sift1, 'UniformOutput', false)));
    if ( seg_nptrs1 == 0 )
        continue;
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % 1st round match    
    [seg_refptrs, seg_matchptrs, seg_ncandidates, seg_best_dists] =...
        findNN_stringmatch15(seg_sift1, database, 4, seg_matchinds, seg_matchdists,...
        maxVariance, medStrength,...
        seg_dist_thres_pixelwise,...
        nn_avg_dist_per_scale_in_query_image,...
        seg_dist_thres_upperbound_pixelwise,...
        seg_alpha_pixelwise,...
        algorithm, gridstep, params1);
    best_dists(seg_ptr_inds) = min([seg_best_dists; best_dists(seg_ptr_inds)], [], 1);
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %uniqueness in a segment
    [useg_matchptrs ii_mat] = unique(seg_matchptrs', 'rows');
    [useg_refptrs ii_ref] = unique(seg_refptrs', 'rows');
    ii = intersect(ii_mat, ii_ref);
    seg_refptrs = seg_refptrs(:,ii);
    seg_matchptrs = seg_matchptrs(:,ii); 
    seg_ncandidates = seg_ncandidates(ii);
    
    if ( ~isempty(seg_refptrs))
        %geometric consistency check
        if ( refidx == 1 )
            deformationCost = CheckGeometry(seg_refptrs, seg_matchptrs, min([gridstep*10,0.5]));
        else
            deformationCost = CheckGeometry(seg_matchptrs, seg_refptrs, min([gridstep*10,0.5]));
        end
        sorted_deformation = sort(deformationCost, 2);
        boundary_rank = max([floor(size(deformationCost,2)*0.20), 10]);
        boundary_deformation = sorted_deformation(:, min([boundary_rank,size(sorted_deformation,1)]));
        validgeoinds = boundary_deformation <= 2*gridstep;
        seg_refptrs = seg_refptrs(:, validgeoinds);
        seg_matchptrs = seg_matchptrs(:, validgeoinds);
        seg_ncandidates = seg_ncandidates(validgeoinds);

        refptrs = [refptrs seg_refptrs];
        matchptrs = [matchptrs seg_matchptrs];
        ncandidates = [ncandidates seg_ncandidates];
        labelinds = [labelinds i*ones(1, length(seg_ncandidates))];
    end
end
tStringMatch = toc(tStartStringMatch);

% initialize matching results
nfinalmatches = 0;
aspect_deviation = 0;
discscore = 0;
validgeoinds = 1; 

if ( isempty(refptrs) )
    return;
end

% final scoring
if ( ~isempty(refptrs) )
    
    tStartScoring = tic;
    disp('scoring');
    %uniqueness
    [fmatchptrs ii_mat] = unique(matchptrs', 'rows');
    [frefptrs ii_ref] = unique(refptrs', 'rows');
    ii = intersect(ii_mat, ii_ref);
    frefptrs = refptrs(:,ii);
    fmatchptrs = matchptrs(:,ii);      
    fncandidates = ncandidates(ii);
    flabelinds = labelinds(ii);
    
    nfinalmatches = size(frefptrs,2);        
    if ( nfinalmatches == 0 )
        return;
    end           
        
    %geometric consistency check
    if ( refidx == 1 )
        deformationCost = CheckGeometry(frefptrs, fmatchptrs, min([gridstep*10,0.5]));
    else
        deformationCost = CheckGeometry(fmatchptrs, frefptrs, min([gridstep*10,0.5]));
    end
    
    geoConsistencyCost = zeros(size(deformationCost));
    for i = 1 : nlabels
        my_labels = flabelinds == i;
        for j = i : nlabels
            other_labels = flabelinds == j;
            if ( i == j ) % geometric consistency within a  region
                geoConsistencyCost(my_labels, other_labels) =...
                    deformationCost(my_labels, other_labels)./(1.5*gridstep);
            else % geometric consistency between two regions
                geoConsistencyCost(my_labels, other_labels) =...
                    deformationCost(my_labels, other_labels)./(3*gridstep);                 
            end
            geoConsistencyCost(other_labels, my_labels) =...
                geoConsistencyCost(my_labels, other_labels)';
        end
    end
    % normalize such that 2 becomes 0.75. (ad-hoc)
    alpha = log((2-0.75)/0.75);    
    geoConsistencyCost = 2./(1 + exp(alpha.*(geoConsistencyCost-1)));
    geoConsistencyCost(geoConsistencyCost > 1) = 1;
        
    validgeoinds = true(1,nfinalmatches); %for compatibility to previous version
    
    %come back to original coordinate systems
    if ( refidx == 1 )
        fptrs1(1,:) = frefptrs(1,:).*ruler1;
        fptrs1(2,:) = frefptrs(2,:).*ruler1;
        fptrs2(1,:) = fmatchptrs(1,:).*ruler2;
        fptrs2(2,:) = fmatchptrs(2,:).*ruler2;
    else
        fptrs2(1,:) = frefptrs(1,:).*ruler2;
        fptrs2(2,:) = frefptrs(2,:).*ruler2;
        fptrs1(1,:) = fmatchptrs(1,:).*ruler1;
        fptrs1(2,:) = fmatchptrs(2,:).*ruler1;
    end
    
    % appearance cost
    fg_inds1 = zeros(1,size(fptrs1,2));
    fg_inds2 = zeros(1,size(fptrs2,2));
    for fi = 1 : size(fptrs1, 2)
        fg_inds1(fi) = find(uint32(sift1(1).ptrs(1,:)) == uint32(fptrs1(1,fi)) & uint32(sift1(1).ptrs(2,:)) == uint32(fptrs1(2,fi)));
        fg_inds2(fi) = find(uint32(ori_sift2(1).ptrs(1,:)) == uint32(fptrs2(1,fi)) & uint32(ori_sift2(1).ptrs(2,:)) == uint32(fptrs2(2,fi)));
    end
    SIFTdistance = zeros(4, size(fptrs1,2));
    for i = 1 : nscales
        cutoff = 1.25*nn_avg_dist_per_scale_in_query_image(i);
        desc1 = single(sift1(i).descriptors(:, fg_inds1));
        desc2 = single(ori_sift2(i).descriptors(:, fg_inds2));
        SIFTdistance(i,:) = sqrt(sum((desc1 - desc2).^2, 1));
        
        SIFTdistance(i, SIFTdistance(i,:) > pixelwise_distance_upperbound(i,fg_inds1)) = inf;
        SIFTdistance(i, SIFTdistance(i,:) > cutoff) = inf;
        
        % A  scaling of distance by absolute distance
        SIFTdistance(i,:) = SIFTdistance(i,:)./nn_avg_dist_per_scale_in_query_image(i);
    end
    appearanceCost = min(SIFTdistance, [], 1);
        
    %center of mass check
    com1 = 0.25*mean(fptrs1(:, validgeoinds), 2) + 0.75*median(fptrs1(:, validgeoinds), 2);
    com2 = 0.25*mean(fptrs2(:, validgeoinds), 2) + 0.75*median(fptrs2(:, validgeoinds), 2);
    deviation1 = norm(com1'-[w(1)/2, h(1)/2])./norm([w(1),h(1)]);
    deviation2 = norm(com2'-[w(2)/2, h(2)/2])./norm([w(2),h(2)]);
    deviationScore1 =  max([2./(1+exp(3.0*(deviation1-0.125))), 0.80]);
    deviationScore2 =  max([2./(1+exp(3.0*(deviation2-0.125))), 0.80]);
    if ( deviationScore1 > 1 ) 
        deviationScore1 = 1;
    end
    if ( deviationScore2 > 1 ) 
        deviationScore2 = 1;
    end
    deviationScore =...
        mean([mean([deviationScore1, deviationScore2]), min([deviationScore1, deviationScore2])]);
    
    % the number of points per scale which have variations in SIFT above a
    % threshold, which we call foreground
    nptr(1) = nptrs1/nscales;
    nptr(2) = nptrs2/nscales;

    % the number of matching points in the pool of foreground points
    overlap_inds = true(1, size(fptrs2,2));
    for ni = 1 : size(fptrs2,2)
        overlap_inds(ni) = ~(sum(uint32(sift2(1).ptrs(1,:)) == uint32(fptrs2(1,ni)) & uint32(sift2(1).ptrs(2,:)) == uint32(fptrs2(2,ni))) == 0);
    end
    n_overlaps = sum(overlap_inds);
    
    %discriminativeness check in an image
    fncandidates = fncandidates./ori_nptrs2;
    disc_threshold = 0.05;
    disc_score_scaler = (1+exp(1.5*(0.0./disc_threshold-1.0)));        
    disc_score = disc_score_scaler./(1+exp(1.5*(fncandidates./disc_threshold-1.0)));
    disc_score(disc_score > 0.5) = 0.5;     
    disc_score = 2*disc_score;
    disc_score(disc_score < 0.5) = 0.5;

    % outpur arguments
    fptrs1_ = fptrs1(:,validgeoinds);
    fptrs2_ = fptrs2(:,validgeoinds);

    % background scoring
    bg_inds = setdiff([1:nptr(1)], fg_inds1);
    bg_arg = best_dists(bg_inds);
    %bg_scale = alpha_for_dist_adjustment_pixelwise(bg_inds);
    %bg_arg = bg_arg./bg_scale;
    bg_arg = bg_arg(~isinf(bg_arg)); 
    background_score_each = 2.0./(1 + exp( 2.5*(bg_arg-1.0) ) );
    background_score_each(background_score_each > 1.0) = 1.0;
    background_score = sum(background_score_each);
        
    %%%%%%%  final scoring method 1 -- same normalization for fg and bg%%%%%%%%%%%%%
    % scoring method 1 = 0.5*geo + 0.5*app + bg
    geo_weight1 = 0.5;
    app_weight1 = 0.5;
    disc_weight1 = 0.0;
    bg_weight1 = 0.1; % 0.5/5: 5 bg ptrs = the worst 1fg ptr of 0.5*app(0.5) + 0.5*geo(0.5)
    
    % scoring method 2 = 1/3*geo + 1/3*app + 1/3*disc + bg
    geo_weight2 = 1/3;
    app_weight2 = 1/3;
    disc_weight2 = 1/3;
    bg_weight2 = 0.1; % 0.5/5: 5 bg ptrs = the worst 1fg ptr of 1/3*app(0.5) + 1/3*geo(0.5) + 1/3*disc(0.5)    
    
    % scoring method 3 = app*geo*disc + bg
    bg_weight3 = 0.025; % 0.5*0.5*0.5/5 : 5 bg ptrs = the worst 1fg ptr of app(0.5)*geo(0.5)*disc(0.5)
    
    % scoring method 4 = (0.5*app + 0.5*geo)*disc + bg
    bg_weight4 = 0.05; % 0.5*0.5/5: 5 bg ptrs = the worst 1fg ptr of (app(0.5)+ geo(0.5)))/2*disc(0.5)
    
    % scoring method 5 = app*geo
    bg_weight5 = 0.05; % 0.5*0.5/5 :  5 bg ptrs = the worst 1fg ptr of app(0.5)*geo(0.5)
    
    % score of each matching point
    appearanceScoreEach = 2.0./(1 + exp( 2.5*(appearanceCost-1.0) ) );
    appearanceScoreEach(appearanceScoreEach > 1.0 ) = 1.0;
    geoConsistencyScoreEach = mean(geoConsistencyCost, 1);
    discriminativeScoreEach = disc_score;
    
    % overall score
    geoConsistencyScore = sum(geoConsistencyScoreEach);
    appearanceScore = sum(appearanceScoreEach);
    discriminativeScore = sum(discriminativeScoreEach);
    
    combinedScore = 0;
    combinedScore2 = 0;
    combinedScore3 = 0;
    if ( ~isempty(geoConsistencyScoreEach) )
        % scoring method 3
        combinedScore = geoConsistencyScoreEach.*appearanceScoreEach.*discriminativeScoreEach;
        combinedScore = sum(combinedScore);    
        % scoring method 4
        combinedScore2 = discriminativeScoreEach.*((geoConsistencyScoreEach + appearanceScoreEach)./2);
        combinedScore2 = sum(combinedScore2);   
        % scoring method 5
        combinedScore3 = geoConsistencyScoreEach.*appearanceScoreEach;
        combinedScore3 = sum(combinedScore3);
    end        

    score1 = geo_weight1*geoConsistencyScore +...
                 app_weight1*appearanceScore +...
                 disc_weight1*discriminativeScore; 
    score2 = geo_weight2*geoConsistencyScore +...
                  app_weight2*appearanceScore +...
                  disc_weight2*discriminativeScore; 
    score1_plusbg = score1 + bg_weight1*background_score;
    score2_plusbg = score2 + bg_weight2*background_score;
    score3_plusbg = combinedScore +  bg_weight3*background_score;
    score4_plusbg = combinedScore2 + bg_weight4*background_score;
    score5_plusbg = combinedScore3 + bg_weight5*background_score;
    
    matchscore(1) = score1_plusbg/sqrt(nptr(1)*ori_nptrs2);
    matchscore(2) = score2_plusbg/sqrt(nptr(1)*ori_nptrs2);
    matchscore(3) = score3_plusbg/sqrt(nptr(1)*ori_nptrs2);
    matchscore(4) = score4_plusbg/sqrt(nptr(1)*ori_nptrs2);
    matchscore(5) = score5_plusbg/sqrt(nptr(1)*ori_nptrs2);
    matchscore(6:10) = deviationScore.*matchscore(1:5);
   
    % this is for convenience of testing different scoring methods
    % afterward.
    fg_score(1) = score1/sqrt(nptr(1)*ori_nptrs2);
    fg_score(2) = score2/sqrt(nptr(1)*ori_nptrs2);
    fg_score(3) = combinedScore/sqrt(nptr(1)*ori_nptrs2);
    fg_score(4) = combinedScore2/sqrt(nptr(1)*ori_nptrs2);
    fg_score(5) = combinedScore3/sqrt(nptr(1)*ori_nptrs2);
    fg_score(6:10) = deviationScore.*fg_score(1:5);
    bg_score(1) = background_score/sqrt(nptr(1)*ori_nptrs2);
    bg_score(2) = deviationScore.*bg_score(1);
    
    %%%%%%%  final scoring method 2 -- different normalization for fg and bg %%%%%%%%%%%%
    divider = sqrt(nptr(1)*nptr(2));
    if ( divider < 0.9*nptr(1) ) % penalize when the exemplar image has much fewer foreground points than a query
        divider = 0.9*nptr(1);
    end
    % score for foreground matched points
    geoConsistencyScoreEach_overlap = mean(geoConsistencyCost(:, overlap_inds), 1);
    appearanceScoreEach_overlap = 2.0./(1 + exp( 2.5*(appearanceCost(overlap_inds)-1.0) ) );
    appearanceScoreEach_overlap(appearanceScoreEach_overlap > 1.0 ) = 1.0;
    discriminativeScoreEach_overlap = disc_score(overlap_inds);
    
    % score for background matched points
    geoConsistencyScoreEach_nonoverlap = mean(geoConsistencyCost(:, ~overlap_inds), 1);
    appearanceScoreEach_nonoverlap = 2.0./(1 + exp( 2.5*(appearanceCost(~overlap_inds)-1.0) ) );
    appearanceScoreEach_nonoverlap(appearanceScoreEach_nonoverlap > 1.0) = 1.0;
    discriminativeScoreEach_nonoverlap = disc_score(~overlap_inds);
    
    % overall score
    geoConsistencyScore_overlap = sum(geoConsistencyScoreEach_overlap);
    geoConsistencyScore_nonoverlap = sum(geoConsistencyScoreEach_nonoverlap);    
    appearanceScore_overlap = sum(appearanceScoreEach_overlap);
    appearanceScore_nonoverlap = sum(appearanceScoreEach_nonoverlap);
    discriminativeScore_overlap = sum(discriminativeScoreEach_overlap);
    discriminativeScore_nonoverlap = sum(discriminativeScoreEach_nonoverlap);

    combinedScore_overlap = 0;
    combinedScore_overlap2 = 0;
    combinedScore_overlap3 = 0;
    if ( ~isempty(geoConsistencyScoreEach_overlap) )
        combinedScore_overlap =...
            geoConsistencyScoreEach_overlap.*appearanceScoreEach_overlap.*discriminativeScoreEach_overlap;    
        combinedScore_overlap = sum(combinedScore_overlap); 
        
        combinedScore_overlap2 =...
            ((geoConsistencyScoreEach_overlap + appearanceScoreEach_overlap)./2).*discriminativeScoreEach_overlap;    
        combinedScore_overlap2 = sum(combinedScore_overlap2); 
        
        combinedScore_overlap3 =...
            geoConsistencyScoreEach_overlap.*appearanceScoreEach_overlap;
        combinedScore_overlap3 = sum(combinedScore_overlap3); 
    end
        
    combinedScore_nonoverlap = 0;
    combinedScore_nonoverlap2 = 0;
    combinedScore_nonoverlap3 = 0;
    if ( ~isempty(geoConsistencyScoreEach_nonoverlap) )
        combinedScore_nonoverlap =...
            geoConsistencyScoreEach_nonoverlap.*appearanceScoreEach_nonoverlap.*discriminativeScoreEach_nonoverlap; 
        combinedScore_nonoverlap = sum(combinedScore_nonoverlap);
        
        combinedScore_nonoverlap2 =...
            ((geoConsistencyScoreEach_nonoverlap + appearanceScoreEach_nonoverlap)./2).*discriminativeScoreEach_nonoverlap; 
        combinedScore_nonoverlap2 = sum(combinedScore_nonoverlap2);
        
        combinedScore_nonoverlap3 =...
            geoConsistencyScoreEach_nonoverlap.*appearanceScoreEach_nonoverlap; 
        combinedScore_nonoverlap3 = sum(combinedScore_nonoverlap3);
    end
    
    score1_overlap = geo_weight1*geoConsistencyScore_overlap +...
                             app_weight1*appearanceScore_overlap +...
                             disc_weight1*discriminativeScore_overlap;
    score2_overlap = geo_weight2*geoConsistencyScore_overlap +...
                             app_weight2*appearanceScore_overlap +...
                             disc_weight2*discriminativeScore_overlap;
    score3_overlap = combinedScore_overlap;
    score4_overlap = combinedScore_overlap2;
    score5_overlap = combinedScore_overlap3;
    
    score1_nonoverlap = geo_weight1*geoConsistencyScore_nonoverlap +...
                                   app_weight1*appearanceScore_nonoverlap +...
                                   disc_weight1*discriminativeScore_nonoverlap;
    score2_nonoverlap = geo_weight2*geoConsistencyScore_nonoverlap +...
                                       app_weight2*appearanceScore_nonoverlap +...
                                       disc_weight2*discriminativeScore_nonoverlap;
    score3_nonoverlap = combinedScore_nonoverlap;
    score4_nonoverlap = combinedScore_nonoverlap2;
    score5_nonoverlap = combinedScore_nonoverlap3;
    
    matchscore(11) = score1_overlap/divider +...
                            score1_nonoverlap/sqrt(nptr(1)*ori_nptrs2) +...
                            bg_weight1*background_score/sqrt(nptr(1)*ori_nptrs2);
    matchscore(12) = score2_overlap/divider +...
                            score2_nonoverlap/sqrt(nptr(1)*ori_nptrs2) +...
                            bg_weight2*background_score/sqrt(nptr(1)*ori_nptrs2);
    matchscore(13) = score3_overlap/divider +...
                            score3_nonoverlap/sqrt(nptr(1)*ori_nptrs2) +...
                            bg_weight3*background_score/sqrt(nptr(1)*ori_nptrs2);
    matchscore(14) = score4_overlap/divider +...
                            score4_nonoverlap/sqrt(nptr(1)*ori_nptrs2) +...
                            bg_weight4*background_score/sqrt(nptr(1)*ori_nptrs2);
    matchscore(15) = score5_overlap/divider +...
                            score5_nonoverlap/sqrt(nptr(1)*ori_nptrs2) +...
                            bg_weight5*background_score/sqrt(nptr(1)*ori_nptrs2);
    matchscore(16:20) = deviationScore.*matchscore(11:15);
    
    fg_score(11) = score1_overlap/divider + score1_nonoverlap/sqrt(nptr(1)*ori_nptrs2);
    fg_score(12) = score2_overlap/divider + score2_nonoverlap/sqrt(nptr(1)*ori_nptrs2);
    fg_score(13) = score3_overlap/divider + score3_nonoverlap/sqrt(nptr(1)*ori_nptrs2);
    fg_score(14) = score4_overlap/divider + score4_nonoverlap/sqrt(nptr(1)*ori_nptrs2);
    fg_score(15) = score5_overlap/divider + score5_nonoverlap/sqrt(nptr(1)*ori_nptrs2);
    fg_score(16:20) = deviationScore.*fg_score(11:15);
    
    tScoring = toc(tStartScoring);
    % display score
    if (sum(isnan(matchscore)) ) % for dealing with the unexpected bug
        matchscore = zeros(20,1);
        fg_score = zeros(20,1);
        bg_score = zeros(2,1);
    else            
        disp('********************************************* Matching information ******************************************************');
        disp(['# of segments: ' num2str(nlabels) ' (threshold: ' num2str(threshold) ')']);
        disp(['nmatches: ' num2str(nfinalmatches) '('  num2str(100*n_overlaps/nfinalmatches) '% overlapped)' ]);
        disp('** timing **');
        disp(['Preprocessing: ' num2str(tPreprocessing) ' sec']);
        disp(['SIFT search: ' num2str(tSIFTSearch) ' sec']);
        disp(['String match: ' num2str(tStringMatch) ' sec']);
        disp(['Scoring: ' num2str(tScoring) ' sec']);
        disp(['Total = ' num2str(tPreprocessing + tSIFTSearch + tStringMatch) 'sec']);
        disp('********************************************************************************************************************');
        disp(' ');
        
        disp('********************************************* Matching score **********************************************************');
        disp('** details **');
        disp(['disc score(avg): ' num2str(discriminativeScore/nfinalmatches) ]);
        disp(['apperance score(avg): ' num2str(appearanceScore/nfinalmatches) ]);
        disp(['geometry score(avg): ' num2str(geoConsistencyScore/nfinalmatches) ]);
        disp(['combined score1(avg): ' num2str(combinedScore/nfinalmatches) ]);
        disp(['combined score2(avg): ' num2str(combinedScore2/nfinalmatches) ]);
        disp(['combined score3(avg): ' num2str(combinedScore3/nfinalmatches) ]);
        disp(['background score: ' num2str(bg_weight2*background_score/sqrt(nptr(1)*ori_nptrs2)) '/' num2str(bg_weight3*background_score/sqrt(nptr(1)*ori_nptrs2)) '/ ' num2str(bg_weight4*background_score/sqrt(nptr(1)*ori_nptrs2))]);
        disp(['deviation score: ' num2str(deviationScore) '/ ' num2str(deviation1) ' ' num2str(deviation2)]);
        disp('** scores **');
        disp(['match scores: ' num2str(matchscore(16)) '(0.5geo + 0.5app + 0.1bg) ' num2str(matchscore(17)) '(1/3geo + 1/3app + 1/3disc + 0.1bg)']);
        disp(['match scores: ' num2str(matchscore(18)) '(geo*app*disc + 0.025bg) ' num2str(matchscore(19)) '((0.5geo + 0.5app)*disc + 0.05bg) ' num2str(matchscore(20)) '(geo*app + 0.05bg) ']);
        disp('********************************************************************************************************************');
    end
end

%plot matchmap
if (display)
    img1 = imresize(img1, [h(1), w(1)]);
    img2 = imresize(img2, [h(2), w(2)]);
    geoConsistencyEach = mean(geoConsistencyCost, 1);

    %come back into original coordinate
    imgs = cell(2,1);
    imgs{1,1} = img1;
    imgs{2,1} = img2;
    figure;
    axis_x = max([w(refidx), w(matchidx)]);
    axis_y = max([h(refidx), h(matchidx)]);
    if ( matchscore(1) > 0 )
        subplot(1,2,1), imshow(imgs{1}); axis([1,axis_x,1,axis_y]);
        
        for i = 1 : size(fptrs1,2)
            
%             
if ( geoConsistencyEach(i) > 0.75 )
     rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [0.0, 1.0, 1.0]);
elseif (geoConsistencyEach(i) > 0.5)
    rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [1.0, 1.0, 0.0]);
elseif (geoConsistencyEach(i) > 0.25)
    rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [1.0, 0.0, 0.0]);
else
    rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [0.0, 0.0, 0.0]);
end

% if ( disc_score(i) > 0.75 )
%      rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [0.0, 1.0, 1.0]);
% elseif (disc_score(i) > 0.5)
%     rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [1.0, 1.0, 0.0]);
% elseif (disc_score(i) > 0.3)
%     rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [1.0, 0.0, 0.0]);
% else
%     rectangle('Position', [fptrs1(1,i)-2 fptrs1(2,i)-2 5 5], 'EdgeColor', [0.0, 0.0, 0.0]);
% end


        end
rectangle('Position', [com1(1)-4, com1(2)-4, 9,9], 'FaceColor', [1.0, 0.0, 0.0]);
title('exemplar');

        subplot(1,2,2), imshow(imgs{2}); axis([1,axis_x,1,axis_y]);
        
        for i = 1 : size(fptrs2,2)

if ( geoConsistencyEach(i) > 0.75 )
     rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [0.0, 1.0, 1.0]);
elseif (geoConsistencyEach(i) > 0.5)
    rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [1.0, 1.0, 0.0]);
elseif (geoConsistencyEach(i) > 0.25)
    rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [1.0, 0.0, 0.0]);
else
    rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [0.0, 0.0, 0.0]);
end

% if ( disc_score(i) > 0.75 )
%      rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [0.0, 1.0, 1.0]);
% elseif (disc_score(i) > 0.5)
%     rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [1.0, 1.0, 0.0]);
% elseif (disc_score(i) > 0.3)
%     rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [1.0, 0.0, 0.0]);
% else
%     rectangle('Position', [fptrs2(1,i)-2 fptrs2(2,i)-2 5 5], 'EdgeColor', [0.0, 0.0, 0.0]);
% end
        end
 rectangle('Position', [com2(1)-4, com2(2)-4, 9,9], 'FaceColor', [1.0,  0.0, 0.0]);
title('query');
    else
        subplot(1,2,1), imshow(imgs{1}); 
        subplot(1,2,2), imshow(imgs{2}); 
    end
    %pause;
    %close;
end


