% Demo to Show sliding segments
% Tomasz Malisiewicz (tomasz@cmu.edu)

%Load up the segmentations of this image
load('12_2_s.segfile.mat');
I = imread('12_2_s.bmp');

%Choose a segmentation from the multiple segmentations
curseg = double(Seg{11});

%create superpixel image
superpixels = segImage(I,curseg);

%get adjacency matrix and merge adjacent segments
N = 3;
adj = segadjgraph(curseg);
groups = enumerateGroups(adj,3);

for i = 1:N
  fprintf(1,'Group %d, Number of segments %d\n',i,length(groups{i}));
end

for i = 1:length(groups)
  for j = 1:size(groups{i},1)
    I2 = segImage2(superpixels,ismember(curseg,groups{i}(j,:)),3);
    imagesc(I2)
    axis image
    axis off
    title(sprintf('MERGES=%d ID=%d/%d',i,j,size(groups{i},1)));
    pause(.1)
  end
end



