function [v i] = max2(x)

v = max(x(:));
[max_y max_x] = find(x == v);
max_x = floor(mean(max_x)+0.5);
max_y = floor(mean(max_y)+0.5);
i = sub2ind(size(x), max_y, max_x);