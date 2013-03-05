clear;

addpath('./code/util/external/brox-opticalflow/');

videoDir = ''; % path to the video
outDir = ''; % output directory

d = dir([videoDir '*.jpg']);

for i=1:length(d)-1
    
    imname1 = d(i).name;
    imname2 = d(i+1).name;
    
    im1 = double(imread([videoDir imname1]));
    im2 = double(imread([videoDir imname2]));

    tic;
    flow = mex_LDOF(im1,im2);
    toc;
    
%     figure(1); clf;
%     subplot(2,2,1); imshow([videoDir imname1]);
%     subplot(2,2,2); imshow([videoDir imname2]);
%     subplot(2,2,3); imagesc(flow(:,:,1));
%     subplot(2,2,4); imagesc(flow(:,:,2));
%     pause;

    vx = flow(:,:,1);
    vy = flow(:,:,2);
    
    outFile = [outDir imname1 '_to_' imname2 '.opticalflow.mat'];
    save('-v7',outFile,'vx','vy');
end


