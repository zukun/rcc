function feats = buildAdpativeSizeFeature_(ptrs, edges, scales, order_k, euc_f, img_size, min_scale, min_feat_size)
% compute features of order k given graph by ptrs and edges

% addpath('../pwmetric');
disp(['computing neighbor elements within ' num2str(order_k) ' hops...']);

% compute shortest path between all-pairs of ptrs
[neighbor_inds topology_dist] = FindTopologyNeighborMex(edges', size(ptrs,1), order_k); 

%%%%%
% max_member = order_k*order_k;
% min_member = 5;
% min_feat_bbox_size = 10*10;
% min_octave = euc_f;
% scale_bound_const = 8^2*(4^2-1);
% min_feat_var = 1.5;
% max_feat_var = 16;
% max_iter = 2;
% feat_cnt = 0;
% for i = 1 : size(ptrs,1)
%        
%     for j = 1 : max_iter
%         
%         scale_factor = 2^(j-1);
%         feat_magnif = min_octave*scale_factor;
%         
%         % topology neighbor
%         member_inds = neighbor_inds{i};
%         member_td = topology_dist{i};
%         member_ptrs = ptrs(member_inds,:)';
%         member_scales = scales(member_inds);
%         ref_ptr = ptrs(i,:)';
%         ref_scale = max([scales(i), min_scale]);
% 
%         % euclidean neighbor
%         member_d = slmetric_pw(ref_ptr, member_ptrs, 'eucdist');
%         feat_var = sqrt(1+scale_bound_const/(scales(i)^2));
%         feat_var(feat_var<min_feat_var) = min_feat_var;
%         feat_var(feat_var>max_feat_var) = max_feat_var;
%         member_flag = member_d(:) < feat_magnif*ref_scale &...
%                       member_scales(:) <= feat_var*scales(i);
%         
%         member_inds = member_inds(member_flag);
%         member_td = member_td(member_flag);
%         member_ptrs = member_ptrs(:, member_flag);
%         member_scales = member_scales(member_flag);
%         feat_size = sum(pi*member_scales.^2); % pixel area
%         
%         if ( feat_size < min_feat_size )
%             continue;
%         end
%         
%         % cut discontinous jump in topological dist
%         [member_td sorted_inds] = sort(member_td);
%         td_diff = zeros(size(member_td));
%         td_diff(2:end) = diff(member_td);
%         td_discont = find(td_diff > 2, 1);
%         if ( ~isempty(td_discont) )
%             sorted_inds = sorted_inds(1:td_discont-1);
%         end        
%         member_ptrs = member_ptrs(:, sorted_inds);
%         member_scales = member_scales(sorted_inds);
%         member_inds = member_inds(sorted_inds);
%         n_sel = numel(member_inds);
%         
%         % pick max_member (for efficiency of descriptor computation)
%         if ( numel(member_inds) > max_member )
%             member_ptrs = member_ptrs(:, 1:max_member);
%             member_scales = member_scales(1:max_member);
%             member_inds = member_inds(1:max_member);
%             n_sel = max_member;
%         else
%             member_inds(end+1:max_member) = -1;        
%         end
%         
%         % center ptr
%         mean_loc = mean(member_ptrs,2);
%         d = slmetric_pw(member_ptrs, mean_loc, 'eucdist');
%         [unused center_idx] = min(d(:));
%         center_ptr = member_ptrs(:, center_idx);
% 
%         % compute boundbox of feat
%         lx = min(member_ptrs(1,:) - member_scales(:)');
%         rx = max(member_ptrs(1,:) + member_scales(:)');
%         ty = min(member_ptrs(2,:) - member_scales(:)');
%         dy = max(member_ptrs(2,:) + member_scales(:)');
% 
%         lx = max([1 lx]);
%         rx = min([img_size(2) rx]);
%         ty = max([1 ty]);
%         dy = min([img_size(1) dy]);
% 
%         
%         % build feat
%         feat_cnt = feat_cnt + 1;
%                 
%         feats(feat_cnt).ref_ptr = ref_ptr;
%         feats(feat_cnt).ref_scale = scales(i);
%         feats(feat_cnt).center_ptr = center_ptr;
%         feats(feat_cnt).member_ptrs = member_ptrs;
%         feats(feat_cnt).member_scales = member_scales(:)';
%         feats(feat_cnt).member_inds = member_inds(:)';
%         feats(feat_cnt).bbox = [lx ty rx dy]';
%         feats(feat_cnt).bbox_size = (rx-lx+1)*(dy-ty+1);
%         feats(feat_cnt).n_member = n_sel;
%         feats(feat_cnt).magnif = feat_magnif;
%         feats(feat_cnt).uid = sort(member_inds);
%         
%     end
%     
% end
% % merge feats
% elems = [feats.n_member];
% feat_bbox_size = [feats.bbox_size];
% feat_size = arrayfun(@(x)(sum(pi*(x.member_scales).^2)), feats);
% feat_flag = (elems >= min_member) & ((feat_size >= min_feat_size) | (feat_bbox_size >= min_feat_bbox_size));
% feats = feats(feat_flag);
% uids = [feats.uid];
% uids = reshape(uids, max_member, numel(uids)/max_member);
% [unused uid] = unique(uids', 'rows');
% feats = feats(uid);


[feats uids] = GroupNeighborMex(neighbor_inds, topology_dist, ptrs', scales, order_k, euc_f, min_scale, min_feat_size, img_size); 
[unused uid] = unique(uids', 'rows');
feats = feats(uid);

    
    