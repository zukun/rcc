function features_sp2_hists(imdir, segdir, featdir, histoutdir, ext)
%
% save superpixel segmentation features (textons, color histograms, pHOG)
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
    segname = [segdir imnames(i).name '.mat'];
   
    fprintf('reading %s, im %d of %d\n', imname, i, length(imnames));
    
    im = imread(imname);
%     load(segname, 'Sp', 'Sp2', 'Seg');
    load(segname, 'Sp2');
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
    
    clear sp_blob sp2_blob seg_blob
   
    G = rgb2gray(im);
    
%     % cycle through the Sp superpixels in this imgae
%     uniqueSegs = unique(Sp(:));
%     numSpSegs = length(uniqueSegs);
%     for j=1:numSpSegs
% 
%         segnum = uniqueSegs(j);
%         inds = find(Sp==segnum);
%         [yy,xx] = find(Sp == segnum);
%       
%         sp_blob(j).centroid = [mean(xx), mean(yy)]; % center position of this blob        
%         sp_blob(j).color_hist = [histc(L(inds), LBinEdges); histc(a(inds), abBinEdges); histc(b(inds), abBinEdges)];       
%         sp_blob(j).full_texton_hist = histc(fullFeats_textonMap(inds), [1:1:NUM_TEXTONS]);
%         sp_blob(j).rotInvar_texton_hist = histc(rotInvarFeats_textonMap(inds), [1:1:NUM_TEXTONS]);        
%         sp_blob(j).segnum = segnum; % id into original Seg image
%             
%         thisG = zeros(size(G));
%         thisG(inds) = G(inds);
% 
%         roi = [min(yy); max(yy); min(xx); max(xx)];
%         if max(yy)-min(yy)<8 || max(xx)-min(xx)<8
%             sp_blob(j).phog = 1/680*ones(1,680);
%         else
%             phogfeat = anna_phog(thisG, nBins, angle_, levels_, roi);
%             sp_blob(j).phog = phogfeat(1:680)';
%         end
%     end
    
    % cycle through the Sp2 superpixels
    uniqueSegs = unique(Sp2(:));
    numSpSegs = length(uniqueSegs);
    for j=1:numSpSegs

        segnum = uniqueSegs(j);
        inds = find(Sp2 == segnum);
        [yy,xx] = find(Sp2 == segnum);
      
        sp2_blob(j).centroid = [mean(xx), mean(yy)]; % center position of this blob        
        sp2_blob(j).color_hist = [histc(L(inds), LBinEdges); histc(a(inds), abBinEdges); histc(b(inds), abBinEdges)];       
        sp2_blob(j).full_texton_hist = histc(fullFeats_textonMap(inds), [1:1:NUM_TEXTONS]);
        sp2_blob(j).rotInvar_texton_hist = histc(rotInvarFeats_textonMap(inds), [1:1:NUM_TEXTONS]);        
        sp2_blob(j).segnum = segnum; % id into original Seg image
          
        thisG = zeros(size(G));
        thisG(inds) = G(inds);

        roi = [min(yy); max(yy); min(xx); max(xx)];
        if max(yy)-min(yy)<8 || max(xx)-min(xx)<8
            sp2_blob(j).phog = 1/680*ones(1,680);
        else
            phogfeat = anna_phog(thisG, nBins, angle_, levels_, roi);
            sp2_blob(j).phog = phogfeat(1:680)';
        end
    end
       
%     % cycle through the Seg regions
%     uniqueSegs = unique(Seg(:));
%     numSpSegs = length(uniqueSegs);
%     for j=1:numSpSegs
% 
%         segnum = uniqueSegs(j);
%         inds = find(Seg==segnum);
%         [yy,xx] = find(Seg == segnum);
%       
%         seg_blob(j).centroid = [mean(xx), mean(yy)]; % center position of this blob        
%         seg_blob(j).color_hist = [histc(L(inds), LBinEdges); histc(a(inds), abBinEdges); histc(b(inds), abBinEdges)];       
%         seg_blob(j).full_texton_hist = histc(fullFeats_textonMap(inds), [1:1:NUM_TEXTONS]);
%         seg_blob(j).rotInvar_texton_hist = histc(rotInvarFeats_textonMap(inds), [1:1:NUM_TEXTONS]);        
%         seg_blob(j).segnum = segnum; % id into original Seg image
%         
%         thisG = zeros(size(G));
%         thisG(inds) = G(inds);
% 
%         roi = [min(yy); max(yy); min(xx); max(xx)];
%         if max(yy)-min(yy)<8 || max(xx)-min(xx)<8
%             seg_blob(j).phog = 1/680*ones(1,680);
%         else
%             phogfeat = anna_phog(thisG, nBins, angle_, levels_, roi);
%             seg_blob(j).phog = phogfeat(1:680)';
%         end            
%     end       
    
    outname = [histoutdir imnames(i).name '.hists.mat'];
    fprintf('saving to %s\n\n', outname);
%     save('-v7',outname, 'seg_blob', 'sp2_blob', 'sp_blob', 'featname', 'segname', 'imname', 'NUM_TEXTONS');
    save('-v7',outname, 'sp2_blob', 'featname', 'segname', 'imname', 'NUM_TEXTONS');
    
    clear Seg Sp Sp2 rotInvarFeats_textonMap fullFeats_textonMap uniqueSegs L a b im
end