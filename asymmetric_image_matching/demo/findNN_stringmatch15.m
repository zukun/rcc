function [refptrs, matchptrs, ncandidates, return_best_dists] =...
    findNN_stringmatch15(refsift, database, dbnscales, matchinds, dists,...
                        maxVariance, medStrength,...
                        nn_dist_thres_pixelwise,...
                        nn_avg_dist_per_scale_in_query_image,...
                        nn_dist_thres_upperbound_pixelwise,...
                        alpha_for_dist_adjustment_pixelwise,...
                        algorithm, gridstep, params)
% ver-4
% the same as the findNN_stringmatch3
% except refptrs & matchptrs are arrays of struct to contain multi-scale features
% while in the previous they were a single struct to contain (jumbled-up) features of all scales.

% ver-5
% standard_dist is changed.

% ver-6
% totally new version for classwise matching

% ver-7
% standard_dist is changed, again.

% ver-8
% handle each scale differently: nn_dist_thres_global is defined for each
% scale

% ver-9
% using pre-built FLANN index structure

% ver-10
% using pixelwise threshold distance

% ver-11
% return each pixel's best distance

% ver-12
% pixelwise alpha for distance adjustment

% ver-13
% threshold upperbound added

% ver-14
% pixelwise alpha considers each scale differently.
% pixelwise_distance_upperbound is removed since it is not effective.

% ver-15
% nn_avg_dist_per_scale is added to weigh each pixel differently 
% depending on the its absolution SIFT descriptor error
% max_search_elems are added.
% FLANN search removed, instead it is given by input arguments, matchinds,
% dists.

%disp('FLANN search...');
%tic;

% merge matchdescriptors across scales
nscales = numel(refsift);
matchptrs = database.ptrs;
nmatchptrs = size(matchptrs,2);
weights = cell(1, nscales); % larger weight --> larger cost
return_best_dists_scales = zeros(nscales, size(nn_dist_thres_pixelwise,2));

for i = 1 : nscales
    weights{i} = dists{i}./nn_avg_dist_per_scale_in_query_image(i);
    weights{i}(weights{i}  <= sqrt(1.5)) = 1.0;    
    weights{i}(weights{i} > sqrt(1.5)) = 1.125; % high deviation --> high cost
    
    % distance normalize per pixel by its standard distance   
    k = size(dists{i}, 1);
    dists{i}(dists{i} > repmat(nn_dist_thres_upperbound_pixelwise(i,:), k,1)) = inf; % relative distance
    cutoff = 1.25*nn_avg_dist_per_scale_in_query_image(i);
    dists{i}(dists{i} > cutoff) = inf; % absolute distance
    
    return_best_dists_scales(i,:) = dists{i}(1,:)./nn_avg_dist_per_scale_in_query_image(i); % return distance scaled by the absolute distance   
    
    dists{i} = dists{i}./repmat(nn_dist_thres_pixelwise(i,:), k,1);    
    
    
    % distance adjustment by alpha
    dists{i} = dists{i}./repmat(alpha_for_dist_adjustment_pixelwise(i,:), k,1);
end
return_best_dists = min(return_best_dists_scales, [], 1);

%flann_free_index(index);

% compute standard_dist to be used as a threshold for good matches
% for performance, we have to consider best distances of all scales, not the single best distance
% among scales.
nrefptrs = sum(cell2mat(arrayfun(@(x)(size(x.ptrs,2)), refsift, 'UniformOutput', false)));
best_dists = zeros(1,nrefptrs);
sind = 0;
for i = 1 : nscales % consider all scales
    n = size(dists{i},2);    
    best_dists(sind+1:sind+n) = dists{i}(1,:); 
    sind = sind + n;
end

%standard_dist = nn_dist_thres_global;

standard_dist = inf;

% transform dist and matchinds into matrix form
matchinds = cell2mat(matchinds');
dists = cell2mat(dists');
weights = cell2mat(weights');
if ( isempty(dists) )
    return;
end

% sort distance (for later use at line 80, [cmatchinds{i} ui] = unique(cmatchinds{i}, 'first')
[dists sorted_inds] = sort(dists,1); 
sorted_inds = sorted_inds + repmat(size(dists,1)*[0:size(dists,2)-1], size(dists,1),1);
matchinds = matchinds(sorted_inds);
weights = weights(sorted_inds);

% build match-pairs
refptrs = refsift(1).ptrs;
refstrengths = zeros(nscales, size(refptrs,2)); 
for i = 1 : nscales
    refstrengths(i,:) = refsift(i).strength;
end
refstrengths = min(refstrengths,[], 1);
refcandidateinds = ~isinf(dists(1,:));
crefptrs = refptrs(:,refcandidateinds);
crefstrengths = refstrengths(refcandidateinds);

cmatchinds = matchinds(:, refcandidateinds);
cmatchinds = mat2cell(cmatchinds, size(cmatchinds,1), ones(1, size(cmatchinds,2)));
cdists = dists(:, refcandidateinds);
cdists = mat2cell(cdists, size(cdists,1), ones(1,size(cdists,2)));
cmatchptrs = cell(1, size(crefptrs,2));
cweights = weights(:, refcandidateinds);
cweights = mat2cell(cweights, size(cweights,1), ones(1,size(cweights,2)));

for i = 1 : size(crefptrs,2)
    texture_weight =... % texture_weight is important for performance    
        (-0.1/(maxVariance-medStrength))*(crefstrengths(i)-medStrength) + 1.0;
    
    matchcandidateinds = ~isinf(cdists{i});
    cmatchinds{i} = cmatchinds{i}(matchcandidateinds);
    cdists{i} = cdists{i}(matchcandidateinds);
    cweights{i} = cweights{i}(matchcandidateinds);
    cmatchptrs{i} = matchptrs(:, cmatchinds{i});
    
    cmatchinds{i} = mod(cmatchinds{i}, nmatchptrs/dbnscales);
    [cmatchinds{i} ui] = unique(cmatchinds{i}, 'first'); % select the best distance among multiple candidates of a given ref point
    cdists{i} = cdists{i}(ui);
    cweights{i} = cweights{i}(ui);
    
    
    cmatchptrs{i} = cmatchptrs{i}(:,ui);
    
    cdists{i} =...
        texture_weight.*cweights{i}./(1 + exp(-5.0*(cdists{i}-1.0)));
%     cdists{i} =...
%         1.0./(1 + exp(-5.0*(cdists{i}-1.0)));
end
%toc;
%disp('string matching...');
%tic;
%string match core
[refptrs, cell_matchptrs, ncandidates, matchmap] = ...
    xy_stringmatch4(crefptrs, cmatchptrs, cdists, algorithm, gridstep, params);
if ( isempty(matchmap) )
    refptrs = [];
    matchptrs = [];
    return;
end
%toc;
refptrs = refptrs(:, matchmap(1,:));
newmatchptrs = zeros(size(matchptrs,1), size(matchmap,2));
ncandidates = ncandidates(matchmap(1,:));
%ncandidates = ncandidates./k;

for i = 1 : size(matchmap,2)
    newmatchptrs(:,i) = cell_matchptrs{matchmap(1,i)}(:,matchmap(2,i));
end
matchptrs = newmatchptrs;
