function features_sp2_segments(imdir, outdir, ext)
%
% compute superpixel over-segmentations
%

imnames = dir([imdir '*' ext]);

doDisplay = 0;
numEvecs = 8;
numSp = 30;
N_sp2 = 25;

for i=1:length(imnames)
    imname = [imdir imnames(i).name];
    
    fprintf('reading %s to get segments...\n', imname);
    
    [Sp, Sp2, Seg] = superpixelRegions(imname, numSp, numEvecs, doDisplay, N_sp2);
    outname = [outdir imnames(i).name '.mat'];
    fprintf('saving to %s\n', outname);
    
%     save('-v7', outname, 'Sp', 'Sp2', 'Seg', 'numEvecs', 'numSp');
    save('-v7', outname, 'Sp2', 'numEvecs', 'numSp');
end



