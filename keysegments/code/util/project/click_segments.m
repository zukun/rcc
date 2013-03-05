DO_SEG = 1;
DO_BINARY = 1;
DO_TRAIN = 1;


NUM_SEG_QUERIES = 12;
NUM_BINARY_QUERIES = 24;
NUM_SHOW = 4;

outputname = input('Give name for this data: ', 's');
%outputname = ['/projects/vision/0/sival/timedata/' outputname '.mat'];
outputname = ['/projects/vision/5/svnaras/timedata/' outputname '.mat'];
fprintf('\nwill be saving to %s\n', outputname);

d = dir('/projects/vision/0/sival/sival_images/');


if(DO_TRAIN)
   
    for i=3:length(d)
        files = dir(['/projects/vision/0/sival/sival_images/' d(i).name '/*.jpg']);
        fileind = randperm(length(files));
        for j=1:NUM_SHOW
            subplot(2,2,j);
            fname = ['/projects/vision/0/sival/sival_images/' d(i).name '/' files(fileind(j)).name];
            im = imread(fname);
            im = imresize(im, round([size(im,1)/3, size(im,2)/3]));
            imshow(im);
            title(d(i).name);
        end
        fprintf('This is %s\n', d(i).name);
        fprintf('press key to continue\n\n');
        pause;
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%

close all;
if(DO_BINARY)
    for j=1:NUM_BINARY_QUERIES

        % choose a random name
        randind = randperm(length(d)-2);
        randomname = d(randind(1)+2).name;

        % choose a random image
        n = rand(1);
        if(n <= 0.5)
            randind = randperm(length(d)-2); % make it the wrong class about 50% of time
            trueanswers(j) = 0;
        else
            trueanswers(j) = 1;
        end

        files = dir(['/projects/vision/0/sival/sival_images/' d(randind(1)+2).name '/*.jpg']);
        randfile = randperm(length(files));
        fname = ['/projects/vision/0/sival/sival_images/' d(randind(1)+2).name '/' files(randfile(1)).name];

        im = imread(fname);
        im = imresize(im, round([size(im,1)/3, size(im,2)/3]));
        imshow(im);
        title(randomname);
        
        tic,
        answer = input(['Does this image contain a ' randomname '? (1 for yes, 0 for no)' ]);,
        timeAnswer = toc;
        fprintf('\n%f to respond\n', timeAnswer);
        if(trueanswers(j) == answer)
            fprintf('That is right\n');
        else
            fprintf('That was wrong\n');
        end
        
        
        answers(j) = answer;
        binaryResponseTimes(j) = timeAnswer;
    end
    fprintf('Average binary response time is %f\n', mean(binaryResponseTimes));
end
close all;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


if(DO_SEG)
    
    d = dir('/projects/vision/0/sival/segmented/*data.mat');

    numsegs = [];
    timetolabel = [];
    randinds = randperm(length(d));

    for i=1:min([length(d), NUM_SEG_QUERIES])

        basein = ['/projects/vision/0/sival/segmented/' d(randinds(i)).name(1:end-9)];

        load([basein '_data.mat']);
        fprintf('segcounts: %d %d %d\n', seg_count, sp1_count, sp2_count);
        segim = imread([basein '_seg.jpg']);
        sp1im = imread([basein '_sp1.jpg']);
        sp2im = imread([basein '_sp2.jpg']);


        subplot(1,2,1);
        imshow(segim);
        
        f = d(randinds(i)).name(1:end-9);
        title(['SEG ' f]);

        fprintf('Click on the segments belonging to the %s\n', d(randinds(i)).name(1:end-9));
        tic,
        [x, y] = ginput;,
        timeClicks = toc;

        numsegclicks = length(x);
        for j=1:length(x)
            segnum = Seg(round(y(j)),round(x(j)));
            segim(find(Seg==segnum)) = 0;
        end
        subplot(1,2,2);
        imshow(segim);
        fprintf('time on seg clicks %f\n', timeClicks);
        pause(2);
        clf;


        subplot(1,2,1);
        imshow(sp1im);
        title(['SP1 ' f]);
        fprintf('Click on the segments belonging to the %s\n', d(randinds(i)).name(1:end-9));

        tic,
        [x, y] = ginput;,
        timeSp1Clicks = toc;

        numsp1clicks = length(x);
        for j=1:length(x)
            segnum = Sp(round(y(j)),round(x(j)));
            sp1im(find(Sp==segnum)) = 0;
        end
        subplot(1,2,2);
        imshow(sp1im);
        fprintf('time on sp1 clicks %f\n', timeSp1Clicks);
        pause(2);

        clf;
        subplot(1,2,1);
        imshow(sp2im);
        title(['SP2 ' f]);
        fprintf('Click on the segments belonging to the %s\n', d(randinds(i)).name(1:end-9));
        tic,
        [x, y] = ginput;,
        timeSp2Clicks = toc;

        numsp2clicks = length(x);
        for j=1:length(x)
            segnum = Sp2(round(y(j)),round(x(j)));
            sp2im(find(Sp2==segnum)) = 0;
        end
        subplot(1,2,2);
        imshow(sp2im);
        fprintf('time on sp2 clicks %f\n', timeSp2Clicks);
        pause(2);

        numsegs = [numsegs, seg_count, sp1_count, sp2_count];
        timetolabel = [timetolabel, timeClicks, timeSp1Clicks, timeSp2Clicks];
        numclicks = [numsegclicks, numsp1clicks, numsp2clicks];
    end
    
    close all;
    [sortedvals, sortedinds] = sort(numsegs);
    plot(numsegs(sortedinds), timetolabel(sortedinds), 'ko');

end

fprintf('\nAverage binary response time is %f\n', mean(binaryResponseTimes));
fprintf('Average segmentation time over all seg levels is %f\n', mean(timetolabel));

fprintf('saving %s\n', outputname);
save(outputname, 'numsegs', 'timetolabel', 'numclicks', 'answers', 'binaryResponseTimes', 'trueanswers');



