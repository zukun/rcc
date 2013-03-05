function [ucm, ucm2, gpb, gpb_orient, t] = ComputeGPB(img_file)

disp('Compute gPb...');
[gpb_orient, gpb, ~, t] = globalPb(img_file, '', 0.5);

ucm2 = contours2ucm(gpb_orient, 'doubleSize');
ucm = ucm2(3:2:end, 3:2:end);