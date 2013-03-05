function lineptrs = linepoints(ptr1,ptr2)

x1 = ptr1(1);
y1 = ptr1(2);
x2 = ptr2(1);
y2 = ptr2(2);

w = abs(x1-x2);
h = abs(y1-y2);
max_dim = max([w h]);
n = max_dim + 1;

lineptrs(1,:) = linspace(x1,x2,2*n);
lineptrs(2,:) = linspace(y1,y2,2*n);

lineptrs = floor(lineptrs + 0.5);


