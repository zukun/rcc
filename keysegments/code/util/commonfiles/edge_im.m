function e = edge_im(im,th)

eim = imfilter(im,[1 -1]);
idx = find(eim~=0);
eim = imfilter(im,[1;-1]);
idx = [idx;find(eim~=0);];
eim = imfilter(im,[-1 1]);
idx = [idx;find(eim~=0);];
eim = imfilter(im,[-1;1]);
idx = [idx;find(eim~=0);];
e = zeros(size(im,1), size(im,2));
e(idx) = 1;
e = imfilter(e,ones(1,th));
e = imfilter(e,ones(th,1));
