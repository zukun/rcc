function [fgpim,bgpim,bgind] = adjustPosteriors(selind,nr,nc,Segs,imname,fgpim,bgpim)

keyFrameNdx = [];
for i=1:length(selind)
    if strcmp(Segs(selind(i)).imname,imname)
        keyFrameNdx = [keyFrameNdx; i];
    end
end

Seg = zeros(nr,nc);
for i=1:length(keyFrameNdx)
    Seg = Seg + Segs(selind(keyFrameNdx(i))).proposal;
end
Seg(Seg>=1) = 1;

[y,x] = find(Seg==1);
miny = max(1,min(y)-50); minx = max(1,min(x)-50); 
maxy = min(nr,max(y)+50); maxx = min(nc,max(x)+50);
Seg(miny:maxy, minx:maxx) = 1;
bgind = find(Seg==0);

bgpim(bgind) = 1;
fgpim(bgind) = 0;
