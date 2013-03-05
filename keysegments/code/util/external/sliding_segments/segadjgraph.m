function ajgraph = segadjgraph(SEGI,nbrthresh)
%function ajgraph = segadjgraph(SEGI,nbrthresh)
%Get the adjancency graph for this segmentation
%where we look at adjancency by at least 5 pixels
%Tomasz Malisiewicz (tomasz@cmu.edu)

if ~exist('nbrthresh','var')
  nbrthresh = 5;
end
max_seg = max(SEGI(:));

ajgraph = segadjgraph_helper(SEGI,max_seg,nbrthresh);


function ajgraph = segadjgraph_helper(orig_SEGI,max_seg,nbrthresh,scaler)

if ~exist('scaler','var')
  SEGI = orig_SEGI;
else
  SEGI = imresize(orig_SEGI,scaler);
end

%this means that the superpixels have to be adjacent at at least 5
%pixels in order to be truly adjacent

%fprintf(1,'nbrthresh is %d\n',nbrthresh);
ajgraph = zeros(max_seg);
kern = [ 0 1 0; 1 1 1; 0 1 0];
for l=1:max_seg,
  % Select one region
  seg = (SEGI == l); 
  % find its border
  border = imdilate(seg,kern) & ~seg;
  % find all regions next to it
  nextto = SEGI(border);
  unext = unique(nextto);
  if length(unext) == 1
    sizes = length(nextto);
  else
    [sizes] = hist(nextto,unext);
  end
  v = zeros(max_seg,1);
 
  goods = find(sizes>=nbrthresh);
  
  if length(goods) == 0
    [tmp,goods] = max(sizes);
  end
  v(unext(goods)) = 1;
 
  ajgraph(l,:) = ajgraph(l,:) + v';
  ajgraph(:,l) = ajgraph(:,l) + v;

end

ajgraph = double(ajgraph>0);