function features_filters(imdir, outdir, vocoutdir, ext, NUM_RESPONSES_PER_IMAGE, NUM_TEXTONS)

imnames = dir([imdir '*' ext]);

conv2_option = 'same';
F = makeRFSfilters;
% Returns the RFS filter bank of size 49x49x38 in F. The MR8, MR4 and
% MRS4 sets are all derived from this filter bank. To convolve an
% image I with the filter bank you can either use the matlab function
% conv2, i.e. responses(:,:,i)=conv2(I,F(:,:,i),'valid'), or use the
% Fourier transform.

% first 36 filters are bars and edges in all orientations and scales (18
% bars, 18 edges)
% 37 is Gaussian
% 38 is LoG

numfilters = size(F,3);

for i=1:length(imnames)    
    imname = [imdir imnames(i).name];
    fprintf('reading %s...\n', imname);    
    im = imread(imname);
    
    % convert image to Luv
    luv_im = colorspace('Luv<-RGB', im);
    L = luv_im(:,:,1);
    u = luv_im(:,:,2);
    v = luv_im(:,:,3);
    
    % check for nans
    nanind = find(isnan(L));
    L(nanind) = 0;
    nanind = find(isnan(u));
    u(nanind) = 0;
    nanind = find(isnan(v));
    v(nanind) = 0;

    % convolve Gaussians with all three channels
    % convolve LoG and edge/bar to L only
    %
    % for rotation invariance:
    % take the max per the bar filters
    % take the max per the edge filters
    %
    % in 1:18: these are grouped by scale - 6 diff orientations at scale 1, then
    % 6 diff orientations at scale 2, then 6 diff orientations at scale 3
    for j=1:18 
        L_edge_responses(:,:,j) = conv2(L, F(:,:,j), conv2_option);
        L_bar_responses(:,:,j) = conv2(L, F(:,:,j+18), conv2_option);
    end
    L_log_response = conv2(L, F(:,:,38), conv2_option);
    L_gaussian_response = conv2(L, F(:,:,37), conv2_option);
    u_gaussian_response = conv2(u, F(:,:,37), conv2_option);
    v_gaussian_response = conv2(v, F(:,:,37), conv2_option);    
    
    % for the bars and edges:
    % for each scale, select max response over all orientations    
    startind = 1;
    for scale=1:3        
        endind = startind + 5;
        edge_responses_this_scale = L_edge_responses(:,:,startind:endind);
        bar_responses_this_scale = L_bar_responses(:,:,startind:endind);        
        
        L_max_edge_responses(:,:,scale) = max(edge_responses_this_scale, [], 3);
        L_max_bar_responses(:,:,scale) = max(bar_responses_this_scale, [], 3); 
        startind = endind + 1;
    end
    
    h = size(im,1);
    w = size(im,2);
    
    % full, rotation variant filter bank output:
    % L log, L gauss, u gauss, v gauss, L edge at 6 orient*3 scales, L bar
    % at 6 orient*3 scales = 4 + 18 + 18 = 40 total dimensions
    fullFeats = zeros(h, w, 40);
    fullFeats(:,:,1) = L_log_response;
    fullFeats(:,:,2) = L_gaussian_response;
    fullFeats(:,:,3) = u_gaussian_response;
    fullFeats(:,:,4) = v_gaussian_response;
    for ii=1:18
        fullFeats(:,:,4+ii) = L_edge_responses(:,:,ii);
        fullFeats(:,:,22+ii) = L_bar_responses(:,:,ii);
    end   
    
    % rotation invariant filter bank output:
    % L log, L gauss, u gauss, v gauss, L max oriented edge at 3 scales,
    % L max oriented bar at 3 scales = 4 + 3 + 3 = 10 dimensions
    rotInvarFeats = zeros(h, w, 10);
    rotInvarFeats(:,:,1:4) = fullFeats(:,:,1:4);
    rotInvarFeats(:,:,5:7) = L_max_edge_responses;
    rotInvarFeats(:,:,8:10) = L_max_bar_responses;    
    
    outname = [outdir imnames(i).name '.mat'];
    fprintf('saving to %s\n', outname);
    save('-v7',outname, 'rotInvarFeats', 'fullFeats', 'luv_im');
    
    clear fullFeats rotInvarFeats L_* u_* v_*
end

% form texton vocab for the full feats 
F = [];
for i=1:length(imnames)
    inname = [outdir imnames(i).name '.mat'];
    fprintf('loading %s\n', inname);
    
    load(inname, 'fullFeats', 'luv_im');
    h = size(luv_im,1);
    w = size(luv_im,2);
    
%     f = fullFeats(:);
    f = reshape(fullFeats(:), w*h, 40)'; % makes filter responses the columns
    randinds = randperm(w*h);
    randinds = randinds(1:NUM_RESPONSES_PER_IMAGE);
    
    F = [F, f(:, randinds)];
    
    fprintf('size F is %d x %d\n', size(F,1), size(F,2));    
    clear fullFeats luv_im
end

outname = [vocoutdir '/fullfeatsVocabData.mat'];
fprintf('saving F to %s\n', outname);
save('-v7',outname, 'F', 'NUM_RESPONSES_PER_IMAGE');

fprintf('running k-means on full feature responses...\n');
[fullmemb, fullmeans, fullrms] = kmeansML(NUM_TEXTONS, F);
fprintf('saving results to %s\n', outname);
save('-v7',outname, 'fullmemb', 'fullmeans', 'fullrms', 'NUM_TEXTONS', '-APPEND');

% assign each pixel to a texton in every image
for i=1:length(imnames)
    
    inname = [outdir imnames(i).name '.mat'];
    fprintf('loading %s to get full feat texton map\n', inname);
    
    load(inname, 'fullFeats');
    h = size(fullFeats,1);
    w = size(fullFeats,2);
    
    f = reshape(fullFeats(:), w*h, 40)'; % makes filter responses the columns

    D = dist2(f', fullmeans');
    if(size(D,1) ~= w*h | size(D,2)~= NUM_TEXTONS)
        fprintf('error in D size\n');
        keyboard;
    end
    
    [mindists, mininds] = min(D, [], 2);    
    fullFeats_textonMap = reshape(mininds, h, w);
    
    %imname = [imdir imnames(i).name];
    %subplot(1,2,1);
    %imshow(imread(imname));
    %subplot(1,2,2);
    %imagesc(fullFeats_textonMap);
    %fprintf('showing texton map\n');
    %pause;
    
    save('-v7',inname, 'fullFeats_textonMap', '-APPEND');
end

clear F fullmeans fullmemb fullrms;

% form texton vocab for rotinvar feats
R = [];
for i=1:length(imnames)
    inname = [outdir imnames(i).name '.mat'];
    fprintf('loading %s\n', inname);

    load(inname, 'rotInvarFeats');
    h = size(rotInvarFeats,1);
    w = size(rotInvarFeats,2);
    
    f = reshape(rotInvarFeats(:), w*h, 10)'; % makes filter responses the columns
    randinds = randperm(w*h);
    randinds = randinds(1:NUM_RESPONSES_PER_IMAGE);

    R = [R, f(:, randinds)];

    fprintf('size R is %d x %d\n', size(R,1), size(R,2));
    clear rotInvarFeats
end

outname = [vocoutdir '/rotInvarFeatsVocabData.mat'];
fprintf('saving R to %s\n', outname);
save('-v7',outname, 'R', 'NUM_RESPONSES_PER_IMAGE');

fprintf('running k-means on rot invar feature responses...\n');
[rot_invar_memb, rot_invar_means, rot_invar_rms] = kmeansML(NUM_TEXTONS, R);
fprintf('saving results to %s\n', outname);
save('-v7',outname, 'rot_invar_memb', 'rot_invar_means', 'rot_invar_rms', 'NUM_TEXTONS', '-APPEND');

% assign each pixel to a rotinvar texton in every image
for i=1:length(imnames)
    
    inname = [outdir imnames(i).name '.mat'];
    fprintf('loading %s to get rot invar texton map\n', inname);
    
    load(inname, 'rotInvarFeats');
    h = size(rotInvarFeats,1);
    w = size(rotInvarFeats,2);
   
    f = reshape(rotInvarFeats(:), w*h, 10)'; % makes filter responses the columns

    D = dist2(f', rot_invar_means');
    if(size(D,1) ~= w*h | size(D,2)~= NUM_TEXTONS)
        fprintf('error in D size\n');
        keyboard;
    end
    
    [mindists, mininds] = min(D, [], 2);    
    rotInvarFeats_textonMap = reshape(mininds, h, w);
    
    save('-v7',inname, 'rotInvarFeats_textonMap', '-APPEND');
    
    clear rotInvarFeats_textonMap rotInvarFeats    
end

% segment the image into superpixels
% color hists per superpixel, texton hists per superpixel

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% to see the outputs of the filters:
% 
% for scale=1:3
% 
%     endind = startind + 5;
% 
%     edge_responses_this_scale = L_edge_responses(:,:,startind:endind);
%     bar_responses_this_scale = L_bar_responses(:,:,startind:endind);
% 
%     startind = endind + 1;
%     L_max_edge_responses(:,:,scale) = max(edge_responses_this_scale, [], 3);
%     L_max_bar_responses(:,:,scale) = max(bar_responses_this_scale, [], 3);
% 
%     %%%%%%%%%%%%%%%%%%% temp dbg
%     figure(1);
%     clf;
%     for ii=1:6
%         subplot(2,3,ii);
%         imagesc(edge_responses_this_scale(:,:,ii));
%     end
%     title('edge responses 6 orientations, single scale\n');
%     colormap gray;
% 
%     figure(3);
%     clf;
%     for ii=1:6
%         subplot(2,3,ii);
%         imagesc(bar_responses_this_scale(:,:,ii));
%     end
%     title('bar responses 6 orientations, single scale\n');
%     colormap gray;
% 
%     figure(2);
%     clf;
%     subplot(1,2,1);
%     imshow(im);
%     subplot(1,2,2);
%     imagesc(L_max_edge_responses(:,:,scale));
%     title(['max response, scale' int2str(scale)]);
%     colormap gray;
%     fprintf('pausing\n');
% 
%     pause;
%     %%%%%%%%%%%%%%%%%%%%%%
% end
% 
% figure(1); clf;
% subplot(2,2,1);
% imagesc(L_log_response), title('L log');
% subplot(2,2,2);
% imagesc(L_gaussian_response), title('L gauss');
% subplot(2,2,3);
% imagesc(u_gaussian_response), title('u gauss');
% subplot(2,2,4);
% imagesc(v_gaussian_response), title('v gauss');
% 
% fprintf('showing log and Gauss\n');
% pause;