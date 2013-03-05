function [fgpim,bgpim,Data] = computePosteriors(im,nr,nc,param,fg,bg)

Data = double(im(:));
Data = reshape(Data, [nr*nc 3]);
[nbVar,nbData] = size(Data');

clear fgPxi bgPxi
for j=1:param.fgNbStates
    %Compute the probability p(x|i)
    fgPxi(:,j) = gaussPDF(Data', fg.Mu(:,j), fg.Sigma(:,:,j));            
end
for j=1:param.bgNbStates
    %Compute the probability p(x|i)
    bgPxi(:,j) = gaussPDF(Data', bg.Mu(:,j), bg.Sigma(:,:,j));            
end

%Compute posterior probability p(i|x)
fgPix_tmp = repmat(fg.Priors,[nbData 1]).*fgPxi;
bgPix_tmp = repmat(bg.Priors,[nbData 1]).*bgPxi;    

fgPix = fgPix_tmp ./ repmat(sum([sum(fgPix_tmp,2),sum(bgPix_tmp,2)],2),[1 param.fgNbStates]);
bgPix = bgPix_tmp ./ repmat(sum([sum(fgPix_tmp,2),sum(bgPix_tmp,2)],2),[1 param.bgNbStates]);

[fgpim,fgcomp] = max(fgPix,[],2);
fgpim = reshape(fgpim,[nr nc]);        

[bgpim,bgcomp] = max(bgPix,[],2);
bgpim = reshape(bgpim,[nr nc]); 