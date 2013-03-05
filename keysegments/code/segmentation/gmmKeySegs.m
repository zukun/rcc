function [fg,bg] = gmmKeySegs(imdir,Segs,selind,param)

fgmodel = [];
bgmodel = [];
for i=1:length(selind)
    im = imread([imdir Segs(selind(i)).imname]);
    [nr, nc, z] = size(im);
    
    Seg = Segs(selind(i)).proposal;
    fgind = find(Seg==1);    
    bgindOrig = find(Seg==0);
    
    [y,x] = find(Seg==1);
    miny = max(1,min(y)-50); minx = max(1,min(x)-50); 
    maxy = min(nr,max(y)+50); maxx = min(nc,max(x)+50);
    Seg(miny:maxy, minx:maxx) = 1;
    bgind = find(Seg==0); 
    
    if isempty(bgind)
        bgind = bgindOrig;
    end
    fgmodel = [fgmodel; im(fgind) im(fgind+nr*nc) im(fgind+2*nr*nc)];
    bgmodel = [bgmodel; im(bgind) im(bgind+nr*nc) im(bgind+2*nr*nc)];
end
fgmodel = double(fgmodel);
bgmodel = double(bgmodel);

[Priors, Mu, Sigma] = EM_init_kmeans(fgmodel', param.fgNbStates);
[fg.Priors, fg.Mu, fg.Sigma, Pix] = EM(fgmodel', Priors, Mu, Sigma);
   
[Priors, Mu, Sigma] = EM_init_kmeans(bgmodel', param.bgNbStates);
[bg.Priors, bg.Mu, bg.Sigma, Pix] = EM(bgmodel', Priors, Mu, Sigma);