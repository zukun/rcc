function bw_inds_out = trimSegment(bw_inds, img_size)

bw = false(img_size);
bw(bw_inds) = true;
[L num] = bwlabel(bw);
sizes = zeros(1,num);

for i = 1 : num
    sizes(i) = sum(L(:) == i);
end
max_size = max(sizes);
idx = find(sizes > 0.5*max_size);
bw_inds_out = [];
for i = 1 : numel(idx)
    bw_inds = find(L == idx(i));
    bw_inds_out = [bw_inds_out bw_inds(:)'];
end

bw_inds_out = uint32(bw_inds_out);