function [quantIm color_index_dist color_index_map nBin] = RGBQuantization(im, nbin, coverage_thresh, is_disp)

if ( ndims(im) == 2 )
    im = repmat(im,[1,1,3]);
end

% binning each channel (0-nbin-1)
imR = double(im(:,:,1));
imG = double(im(:,:,2));
imB = double(im(:,:,3));

interval = 255/nbin;
rBin = ceil(imR./interval);
gBin = ceil(imG./interval);
bBin = ceil(imB./interval);
rBin(rBin==0) = 1;
gBin(gBin==0) = 1;
bBin(bBin==0) = 1;

% binIndex: 0-nbin^3-1, 2D image
binIndex = (rBin-1) + (gBin-1).*nbin + (bBin-1).*nbin^2; 

% histogram
h = histc(binIndex(:), 0:nbin^3-1);
[sort_h inds] = sort(h, 'descend');
inds = inds - 1;
coverage = cumsum(sort_h)./sum(h);
cutoff_idx = find(coverage > coverage_thresh, 1);
cutoff_idx2 = find(coverage == 1.0, 1);
selected_inds = inds(1:cutoff_idx);
outlier_inds = inds(cutoff_idx+1:cutoff_idx2);
nBin = numel(selected_inds);

% selected inds
bBin_inds = floor(selected_inds./nbin^2) + 1;
gBin_inds = floor((selected_inds - (bBin_inds-1).*nbin^2)./nbin) + 1;
rBin_inds = floor((selected_inds - (bBin_inds-1).*nbin^2 - (gBin_inds-1).*nbin)) + 1;
rCenters = (rBin_inds-1).*interval + ceil(interval/2);
gCenters = (gBin_inds-1).*interval + ceil(interval/2);
bCenters = (bBin_inds-1).*interval + ceil(interval/2);
[LC aC bC] = RGB2Lab(rCenters, gCenters, bCenters);

% unselected inds
bOutBin_inds = floor(outlier_inds./nbin^2) + 1;
gOutBin_inds = floor((outlier_inds - (bOutBin_inds-1).*nbin^2)./nbin) + 1;
rOutBin_inds = floor((outlier_inds - (bOutBin_inds-1).*nbin^2 - (gOutBin_inds-1).*nbin)) + 1;
rOutCenters = (rOutBin_inds-1).*interval + ceil(interval/2);
gOutCenters = (gOutBin_inds-1).*interval + ceil(interval/2);
bOutCenters = (bOutBin_inds-1).*interval + ceil(interval/2);
[LOutC aOutC bOutC] = RGB2Lab(rOutCenters, gOutCenters, bOutCenters);

% Lab dist
selected_Lab = [LC(:)'; aC(:)'; bC(:)'];
out_Lab = [LOutC(:)'; aOutC(:)'; bOutC(:)'];
Lab_dist = slmetric_pw(out_Lab, selected_Lab, 'eucdist');
[unused, assigned_inds] = min(Lab_dist,[],2);

% RGB dist
% selected_RGB = [rCenters(:)'; gCenters(:)'; bCenters(:)'];
% out_RGB = [rOutCenters(:)'; gOutCenters(:)'; bOutCenters(:)'];
% RGB_dist = slmetric_pw(out_RGB, selected_RGB, 'eucdist');
% [unused assigned_inds] = min(RGB_dist,[],2);

% quantized image and index map
color_index_map = zeros(size(imR));
for i = 1 : numel(selected_inds)
    bw = binIndex == selected_inds(i);
 
    imR(bw) = rCenters(i);
    imG(bw) = gCenters(i);
    imB(bw) = bCenters(i);
    
    color_index_map(bw) = i;
end
for i = 1 : numel(outlier_inds)
    bw = binIndex == outlier_inds(i);
    aidx = assigned_inds(i);
    
    imR(bw) = rCenters(aidx);
    imG(bw) = gCenters(aidx);
    imB(bw) = bCenters(aidx);
    
    color_index_map(bw) = aidx;
end
quantIm(:,:,1) = imR;
quantIm(:,:,2) = imG;
quantIm(:,:,3) = imB;

% color distance between bins
Lab = [LC(:)';aC(:)';bC(:)'];
color_index_dist = slmetric_pw(Lab,Lab, 'eucdist');

if ( is_disp )
    subplot(1,2,1);
    imshow(im);
    subplot(1,2,2);
    imshow(uint8(quantIm));
    title(num2str(nBin));
end
