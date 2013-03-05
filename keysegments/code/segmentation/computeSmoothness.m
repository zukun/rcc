function [Hc,Vc] = computeSmoothness(im,param)

[dx dy] = gradient(im2double(im));
hL2 = sum(dx.*dx, 3);
vL2 = sum(dy.*dy, 3);

beta_ = 1/(2*mean([hL2(:); vL2(:)]));

Hc = param.gamma_*exp(-beta_*hL2);
Vc = param.gamma_*exp(-beta_*vL2);  