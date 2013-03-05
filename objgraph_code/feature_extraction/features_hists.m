function features_hists(imdir, segdir, featdir, histoutdir, ext)
%
% save multiple-segmentations' features (textons, color histograms, pHOG)
%

imnames = dir([imdir '*' ext]);

NUM_TEXTONS = 400; 
numColorHistBins = 22;
maxLValue = 100;

LBinSize = maxLValue / numColorHistBins;
abBinSize = 256 / numColorHistBins;
LBinEdges = [0:LBinSize:100];
abBinEdges = [-128:abBinSize:128];

nBins = 8;
angle_ = 360;
levels_ = 3;

% for each blob, make two texton hists and a color hist, and record
% centroid

for i=1:length(imnames)
    imname = [imdir imnames(i).name];
    featname = [featdir imnames(i).name '.mat'];
    segname = [segdir imnames(i).name '.seg.mat'];
   
    fprintf('reading %s, im %d of %d\n', imname, i, length(imnames));
    
    im = imread(imname);
    load(segname, 'Segs');
    load(featname, 'rotInvarFeats_textonMap', 'fullFeats_textonMap');    
   
    [L,a,b] = RGB2Lab(double(im(:,:,1)), double(im(:,:,2)), double(im(:,:,3)));
   
    if(max(L(:)) > 100 | min(L(:)) < 0)
        fprintf('error in L range\n');
        keyboard;
    end
    if(max(a(:)) > 128 | min(a(:)) < -128)
        fprintf('error in a range\n');
        keyboard;
    end
    if(max(b(:)) > 128 | min(b(:)) < -128)
        fprintf('error in b range\n');
        keyboard;
    end
    
    clear multiSegs
   
    G = rgb2gray(im);
    % cycle through the Sp superpixels in this imgae
    for segmentationind = 1:size(Segs,3)
        Sp = Segs(:,:,segmentationind);
        
        uniqueSegs = unique(Sp(:));
        numSpSegs = length(uniqueSegs);
        
        for j=1:numSpSegs
            segnum = uniqueSegs(j);
            inds = find(Sp==segnum);
            [yy,xx] = find(Sp == segnum);      
            
            multiSegs(segmentationind).seg_blob(j).centroid = [mean(xx), mean(yy)]; % center position of this blob
            multiSegs(segmentationind).seg_blob(j).color_hist = [histc(L(inds), LBinEdges); histc(a(inds), abBinEdges); histc(b(inds), abBinEdges)];
            multiSegs(segmentationind).seg_blob(j).full_texton_hist = histc(fullFeats_textonMap(inds), [1:1:NUM_TEXTONS]);
            multiSegs(segmentationind).seg_blob(j).rotInvar_texton_hist = histc(rotInvarFeats_textonMap(inds), [1:1:NUM_TEXTONS]);
            multiSegs(segmentationind).seg_blob(j).segnum = segnum; % id into original Seg image

            thisG = zeros(size(G));
            thisG(inds) = G(inds);
            
            roi = [min(yy); max(yy); min(xx); max(xx)];
            if max(yy)-min(yy)<8 || max(xx)-min(xx)<8
                multiSegs(segmentationind).seg_blob(j).phog = 1/680*ones(1,680);
            else
                phogfeat = anna_phog(thisG, nBins, angle_, levels_, roi);
                multiSegs(segmentationind).seg_blob(j).phog = phogfeat(1:680)';
            end
        end        
    end    
    
    outname = [histoutdir imnames(i).name '.hists.mat'];
    fprintf('saving to %s\n\n', outname);
    save('-v7', outname, 'multiSegs', 'featname', 'segname', 'imname', 'NUM_TEXTONS');
    
    clear Segs rotInvarFeats_textonMap fullFeats_textonMap uniqueSegs L a b im
end