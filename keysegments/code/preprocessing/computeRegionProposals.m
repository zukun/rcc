clear;

addpath(genpath('./code/util/external/proposals/'))
videoDir = ''; % path to the video

d = dir([videoDir '*.jpg']);
for i=1:length(d)
    imname = d(i).name;
    
    [proposals superpixels image_data unary] = generate_proposals([videoDir imname]);
    save('-v7',[outDir  imname '.mat'], 'proposals', 'superpixels', 'unary');
end

