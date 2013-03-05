n_label = 25;

DataCost = rand(450,416,n_label);
Smoothness = 1 - eye(n_label,n_label);
% for i = 1 : n_label-1
%     Smoothness(i,i+1) = 0.5;
% end
% for i = 2 : n_label
%     Smoothness(i,i-1) = 0.5;
% end
% [x y] = meshgrid(linspace(1,416,5), linspace(1,450,5));
x = linspace(1,416,5);
y = linspace(1,450,5);
tic; 

for i = 1 : numel(x)-1
    for j = 1 : numel(y)-1
    
        D = DataCost(ceil(y(j)):ceil(y(j+1)), ceil(x(i)):ceil(x(i+1)),:); 

        [gch] = GraphCut('open', D, Smoothness);
        iter = 60;
        [gch labels] = GraphCut('expand', gch, iter);
        [gch] = GraphCut('close', gch);
    end
end

toc;