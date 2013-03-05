function [refptrs, matchptrs, ncandidates, matchmap] =...
    xy_stringmatch4(crefptrs, cmatchptrs, cdists, algorithm, gridstep, params)
% the same as xy_stringmatch3
% except considering only core input and output

%compute # of candidates
ncandidates = cell2mat(cellfun(@numel, cdists, 'UniformOutput', false));
ncandidates = reshape(ncandidates, 1, numel(ncandidates));

%make strings
[refptrsx, sorted_inds] = sortPtrs(crefptrs, 0);
matchptrsx = cmatchptrs(sorted_inds);
matchdistsx = cdists(sorted_inds);
ncandidatesx = ncandidates(sorted_inds);

[refptrsy, sorted_inds] = sortPtrs(crefptrs, 1);
matchptrsy = cmatchptrs(sorted_inds);
matchdistsy = cdists(sorted_inds);
ncandidatesy = ncandidates(sorted_inds);

nrefs = size(crefptrs,2);
refptrs = [refptrsx refptrsy];
ncandidates = [ncandidatesx ncandidatesy];

nstrings = 2;
matchptrs = cell(1, nstrings*nrefs);
matchptrs(1:nrefs) = matchptrsx;
matchptrs(nrefs+1:2*nrefs) = matchptrsy;

%string match
nrefsx = size(refptrsx,2);
nmatchesx = max(cellfun('size',matchptrsx, 2));
if ( strcmp(algorithm, 'flatDistance') )
    [nrawmatchesx matchmapx] =...
        mexStringMatchCore(refptrsx, matchptrsx,...
                           nrefsx, nmatchesx, gridstep, params1);
elseif ( strcmp(algorithm, 'varDistance') )
    [nrawmatchesx matchmapx] =...
        mexStringMatchCore2(refptrsx, matchptrsx,...
                            matchdistsx, nrefsx, nmatchesx, gridstep, params);
end

nrefsy = size(refptrsy,2);
nmatchesy = max(cellfun('size',matchptrsy, 2));
if ( strcmp(algorithm, 'flatDistance') )
    [nrawmatchesy matchmapy] =...
        mexStringMatchCore(refptrsy, matchptrsy,...
                           nrefsy, nmatchesy, gridstep, params1);
elseif ( strcmp(algorithm, 'varDistance') )
    [nrawmatchesy matchmapy] =...
        mexStringMatchCore2(refptrsy, matchptrsy,...
                            matchdistsy, nrefsy, nmatchesy, gridstep, params);
end
if ( ~isempty(matchmapy) )
    matchmapy(1,:) = matchmapy(1,:) + nrefs;
end
matchmap = [matchmapx matchmapy]; 