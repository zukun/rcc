function features_segments(imdir, outdir, ext)
%
% compute multiple-segmentations with Ncuts
%

imnames = dir([imdir '*' ext]);

for i=1:length(imnames)
    imname = [imdir imnames(i).name];
    
    fprintf('reading %s to get segments...\n', imname);
    img = imread(imname);
    
    [Segs numsegs res] = multipleSegmentations(img);
    outname = [outdir imnames(i).name '.seg.mat'];
    fprintf('saving to %s\n', outname);
    
    save('-v7', outname, 'Segs', 'numsegs', 'res');
end


