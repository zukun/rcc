function evaluateSegTrackResults(imdir,our_mask,Segs,selind)

gtdir = [imdir 'ground-truth/'];

figure(1);
N = ceil(sqrt(length(selind)));
for i=1:length(selind)
    im = imread([imdir Segs(selind(i)).imname]);
    I2 = segImage3(im2double(im),double(Segs(selind(i)).proposal),2,2);
    subplot(N,N,i); imagesc(I2); title(ceil(selind(i)/10));
end

d = dir([imdir '/*.jpg']);
if isempty(d)
    d = dir([imdir '/*.png']);
end
if isempty(d)
    d = dir([imdir '/*.bmp']);
end

gtd = dir([gtdir]);
err_pixels = 0;
err_pixelsMat = zeros(length(d),1);
for i=1:length(d)
    ours = our_mask(:,:,i);
    
    try
        gt = imread([gtdir num2str(i-1) '.bmp']);        
    catch
        gt = imread([gtdir gtd(i+2).name]);   
    end
    gt = rgb2gray(gt);
    gt = (gt>128);
    
    im = imread([imdir d(i).name]); 
    [nr, nc, z] = size(im);
    
    figure(2); 
    I2 = segImage3(im2double(im),double(ours),2,1);
    subplot(2,2,1); imagesc(I2); title(['Ours, Error: ' num2str(sum(sum(xor(ours,gt))))]);
    I2 = segImage3(im2double(im),double(gt),2,1);
    subplot(2,2,2); imagesc(I2); title('Ground Truth');

    ind = find(ours==1);
    im_ours = uint8(255*ones(size(im)));
    im_ours([ind; ind+nr*nc; ind+2*nr*nc]) = im([ind; ind+nr*nc; ind+2*nr*nc]);
    subplot(2,2,3); imagesc(im_ours); title(['Ours, Error: ' num2str(sum(sum(xor(ours,gt))))]);
    ind = find(gt==1);
    im_gt = uint8(255*ones(size(im))); 
    im_gt([ind; ind+nr*nc; ind+2*nr*nc]) = im([ind; ind+nr*nc; ind+2*nr*nc]);
    subplot(2,2,4); imagesc(im_gt); title('Ground Truth'); title(i);
    if i==1
        pause;
    else
%         pause(0.3);
%         pause;
    end
    
    err_pixels = err_pixels + sum(sum(xor(ours,gt)));
    err_pixelsMat(i) = sum(sum(xor(ours,gt)));
end
disp(['Avg # of incorrect pixels: ' num2str(mean(err_pixelsMat))]);

% err_pixels/length(d)
% mean(err_pixelsMat)
