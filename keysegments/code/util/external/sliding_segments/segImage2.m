function finalI = segImage2(I,mask,a,b)
%function finalI = segImage2(I,mask,a,b)
%create an image of the segment by making the outside slightly
%darker
%I is the RGB input image
%mask is the input mask (1 is region, 0 is outside)
%a and b are the optional options (color and thickness)
%Tomasz Malisiewicz (tomasz@cmu.edu)

masky = .3*repmat(~mask,[1 1 3]) + repmat(mask,[1 1 3]);
I2 = I .* masky;

if exist('b','var')
  finalI = segImage(I2,mask,a,b);  
elseif exist('a','var')
  finalI = segImage(I2,mask,a);
else
  finalI = segImage(I2,mask);
end