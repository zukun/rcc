cmap = colormap(lines(3));
x = [0.0013938, 0.00878945, -0.00798057, 0.0117477, 0.00487326, 0.00835225, 0.003692, -0.00236762, 0.00483042, 0.00729653, 0.00700795, -0.000607785];
y = [0.420967, 0.419825, 0.421958, 0.419838, 0.419561, 0.420527, 0.420193, 0.419769, 0.421485, 0.419019, 0.420389, 0.417837];
h = plot(x,y,'or');
set(h, 'Color', cmap(1,:));
hold on
xx = [0.00387962];
yy = [0.420204];
h = plot(xx,yy,'+r');
set(h, 'Color', cmap(1,:));
hold on
x = [0.500644, 0.50474, 0.492816, 0.50439, 0.492813, 0.502163, 0.51417, 0.496609, 0.504552, 0.504289, 0.506244, 0.502584, 0.507566, 0.502122];
y = [0.188019, 0.188147, 0.188025, 0.188629, 0.188119, 0.188546, 0.187674, 0.188953, 0.188201, 0.18874, 0.188111, 0.188588, 0.187898, 0.1882];
h = plot(x,y,'or');
set(h, 'Color', cmap(2,:));
hold on
xx = [0.50477];
yy = [0.188347];
h = plot(xx,yy,'+r');
set(h, 'Color', cmap(2,:));
hold on
x = [1.00223];
y = [0.573476];
h = plot(x,y,'or');
set(h, 'Color', cmap(3,:));
hold on
xx = [1.00223];
yy = [0.573476];
h = plot(xx,yy,'+r');
set(h, 'Color', cmap(3,:));
hold on
