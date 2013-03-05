function [obj_graph] = getObjectGraphDescriptor(reg_locs,sp_locs,prim,r)
%
% Compute object-graph descriptor for unknown region 
%
% Inputs: reg_locs (region centroid location, [y x])
%         sp_locs (superpixel centroid locations)
%         prim (superpixel-level posteriors)
%         r (number of superpixel regions to consider above and below for object-graph)
%
% Outputs: obj_graph (object-graph descriptor)
%

ypos = reg_locs(1);
% include self
above_ind = find(sp_locs(:,1)<=ypos);
iter(1).segs = [reg_locs; sp_locs(above_ind,:)];

below_ind = find(sp_locs(:,1)>ypos);
iter(2).segs = [reg_locs; sp_locs(below_ind,:)];

num_classes = size(prim,3);
for i=1:2       
    iter(i).cpd_vec = zeros(r,num_classes);
 
    % find r nearest-neighbor superpixels for the unknown region
    nearest_neighbors = dist2(reg_locs,iter(i).segs);    
    [nnval, nnind] = sort(nearest_neighbors,'ascend');

    % if shorter than the number of specified neighbors
%     if length(nnind) < r
%         nnind = [nnind nnind(end)*ones(1,r-length(nnind))];
%     end
    if length(nnind) < r
        nnind = kron(nnind,ones([1,ceil(r/length(nnind))]));
    end

    for j=1:r 
        % locs is (y,x)
        locs = iter(i).segs(nnind(j),:); 
        iter(i).cpd_vec(j,:) = prim(locs(1),locs(2),:);    
    end
end

cpd_vec = [iter(1).cpd_vec iter(2).cpd_vec];
% sum when considering farther regions
cpd_vec = cumsum(cpd_vec,1);

% normalization
nmat = repmat([1:size(cpd_vec,1)]',[1 size(cpd_vec,2)]);
obj_graph = cpd_vec./nmat;



