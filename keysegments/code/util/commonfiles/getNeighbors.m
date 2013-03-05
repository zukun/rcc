function graph = getNeighbors(seg)

[h,w] = size(seg);
num_segs = max(max(seg));
graph = zeros(num_segs);

for i=1:num_segs
    tmp = zeros(size(seg));
    tmp(seg==i) = 1;
    e = bwperim(tmp);
    idx = find(e);
    [rows,cols] = ind2sub(size(e),idx);

%     check rows cols
    for j=1:length(rows)
        for k=-1:1
            for l=-1:1
              row = rows(j)+k;
              col = cols(j)+l;
              if((row <=0 || col <=0) || (row >h || col > w) || seg(row,col) == i)
                  continue;
              end
%                   tmp(row,col) = seg(row,col);
              graph(i,seg(row,col)) = 1;
              graph(seg(row,col),i) = 1;
            end
        end
    end
    %imagesc(tmp);
    %pause
end  
