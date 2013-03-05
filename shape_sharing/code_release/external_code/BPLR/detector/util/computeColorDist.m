function color_dist = computeColorDist(im, ptrs, scales, min_scale)

color_hist = computeColorHist_(im,ptrs,scales,min_scale);
color_hist = single(color_hist);


try
    color_dist = slmetric_pw(color_hist, color_hist, 'eucdist');
catch ME
    if ( strmatch('Out of memory', ME.message) )
        len = size(color_hist,2);
        color_dist = zeros(len,len,'single');
        l = floor(len/2);
        color_dist(1:l,1:l) = slmetric_pw(color_hist(:,1:l), color_hist(:,1:l), 'eucdist');
        color_dist(l+1:len,1:l) = slmetric_pw(color_hist(:,l+1:len), color_hist(:,1:l), 'eucdist');
        color_dist(1:l,l+1:len) = slmetric_pw(color_hist(:,1:l), color_hist(:,l+1:len), 'eucdist');
        color_dist(l+1:len,l+1:len) = slmetric_pw(color_hist(:,l+1:len), color_hist(:,l+1:len), 'eucdist');
    end
end
clear color_hist
color_dist = replaceDiagonal(color_dist,inf);
