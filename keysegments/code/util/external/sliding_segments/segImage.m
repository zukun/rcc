function I_s = segImage(I,S,channel,dilationfactor)
%function I_s = segImage(I,S,channel,dilationfactor)
%return an image which shows the segment boundaries
%dilationfactor makes fatter lines
%Tomasz Malisiewicz (tomasz@cmu.edu)

I = im2double(I);
if ~exist('channel','var')
  channel = 1;
end
if ~exist('dilationfactor','var')
  dilationfactor = 0;
end

[cx,cy] = gradient(double(S));
ccc = (abs(cx)+abs(cy))~=0;
if dilationfactor > 0 
  ccc = bwmorph(ccc,'dilate',dilationfactor);
elseif  dilationfactor < 0
  ccc = bwmorph(ccc,'thin');
end


I_s = I;

if channel == 1
  I_s(:,:,1) = max(I_s(:,:,1),ccc);
  I_s(:,:,2) = min(I_s(:,:,2),~ccc);
  I_s(:,:,3) = min(I_s(:,:,3),~ccc);
elseif channel == 2
  I_s(:,:,1) = min(I_s(:,:,1),~ccc);
  I_s(:,:,2) = max(I_s(:,:,2),ccc);
  I_s(:,:,3) = min(I_s(:,:,3),~ccc);
elseif channel == 3
  I_s(:,:,1) = min(I_s(:,:,1),~ccc);
  I_s(:,:,2) = min(I_s(:,:,2),~ccc);
  I_s(:,:,3) = max(I_s(:,:,3),ccc);
elseif channel == 4
  I_s(:,:,1) = max(I_s(:,:,1),ccc);
  I_s(:,:,2) = min(I_s(:,:,2),~ccc);
  I_s(:,:,3) = max(I_s(:,:,3),ccc);
else
  fprintf('wrong channel type, must be 1,2,or 3\n');
end
