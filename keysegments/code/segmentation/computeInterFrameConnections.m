function [coloraff,opticalflow_map] = computeInterFrameConnections(opticalflowdir,Data,nextIm,imname1,imname2,param)

% get squared color dist of connected nodes
[nr, nc, z] = size(nextIm);
nextData = double(nextIm(:));
nextData = reshape(nextData, [nr*nc 3]);

% get connections between adjacent frames
opticalflowname = [opticalflowdir imname1 '_to_' imname2 '.opticalflow.mat'];
load(opticalflowname,'vx','vy');      

xposMat = round(repmat([1:nc],[nr,1]) + vx);
yposMat = round(repmat([1:nr]',[1 nc]) + vy);

invalidNDx = (xposMat<=0)|(xposMat>nc)|(yposMat<=0)|(yposMat>nr);
xposMat(invalidNDx) = 1;
yposMat(invalidNDx) = 1;

opticalflow_map = sub2ind([nr,nc], yposMat, xposMat);

colordist = zeros(size(opticalflow_map));
for m=1:size(opticalflow_map,1)
    for n=1:size(opticalflow_map,2)
        thisNdx = sub2ind([nr,nc], m, n);
        colordist(m,n) = sum((Data(thisNdx,:)-nextData(opticalflow_map(m,n),:)).^2);
%         colordist(m,n) = slmetric_pw(Data(thisNdx,:)', nextData(opticalflow_map(m,n),:)', 'sqdist');
    end
end

beta_ = 1/(2*mean(colordist(:)));

% opticalflow_map(invalidNDx) = -1;
coloraff = param.gamma_*exp(-beta_*colordist);
coloraff(invalidNDx) = 0;