function groups = enumerateGroups(adjgraph,NNN)
%function groups = enumerateGroups(adjgraph,NNN)
%Returns all groups of size 1:NNN from adjacency matrix adjgraph
%as a cell array called groups
%The groups are the ids of the connected regions
%Tomasz Malisiewicz (tomasz@cmu.edu)

NUMVERTS = size(adjgraph,1);
for i = 1:NNN
  groups{i} = [];
end
groups{1} = (1:NUMVERTS)';

if (NNN==1 || NUMVERTS==1)  
  return
end

[a,b] = find(adjgraph == 1);
inds = find(b>a);
groups{2} = [a(inds) b(inds)];
groups{2} = sortrows(groups{2},[1 2]);


for curind = 3:min(NUMVERTS,NNN)
 
  oldsize = size(groups{curind-1}); 
  bigbool=(adjgraph(reshape(groups{curind-1}, ...
                                      prod(oldsize),1),:));
  
  bigbool = reshape(bigbool,[oldsize(1) oldsize(2) NUMVERTS]);
  bigbool = squeeze(sum(bigbool,2));

  %zero out used ones
  firstinder = repmat(1:oldsize(1),[1 oldsize(2)]);
  secondinder = reshape(groups{curind-1},1,prod(oldsize));
  if size(bigbool,2)==1
    break
  end
  inds = sub2ind(size(bigbool),firstinder,secondinder);

  bigbool(inds)=0;

  [bigbb,bigaa]=find(bigbool');
  
  
  newgroups = [groups{curind-1}(bigaa,:) bigbb];

  %this sort is definitely necessary
  newgroups = sort(newgroups')';
  %this unique call is the biggest bottle-neck here
  newgroups = unique(newgroups,'rows');
  

  if size(newgroups,1)==0
    %last group was null
    return
  end
  
  groups{curind} = newgroups;
end
