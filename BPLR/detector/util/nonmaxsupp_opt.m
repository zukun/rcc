function supp_im = nonmaxsupp_opt(im, supp_radius, supp_thresh, opt)
% opt = 'sliding', 'distinct'

if ( strcmp(opt, 'sliding') )
    im = double(im);
    nonmax = inline('max(x)');
    max_im = colfilt(im, [supp_radius supp_radius], opt, nonmax);
    supp_im = im.*(im == max_im);
    supp_im(supp_im < supp_thresh) = 0;
elseif ( strcmp(opt, 'distinct') )
    im = double(im);
    w = size(im,2);
    h = size(im,1);
    w_margin = supp_radius - mod(w, supp_radius);
    h_margin = supp_radius - mod(h, supp_radius);
    im = padarray(im,[h_margin, w_margin], 0, 'post');
    [xx yy] = meshgrid(1:supp_radius:size(im,2)-supp_radius+1, 1:supp_radius:size(im,1)-supp_radius+1);
    [max_vals, locs] = arrayfun(@(i,j)(max2(im(i:i+supp_radius-1, j:j+supp_radius-1))), yy,xx);
    [sample_yy sample_xx] = ind2sub([supp_radius, supp_radius], locs);
    sample_x = (sample_xx-1) + xx;
    sample_y = (sample_yy-1) + yy;
    nz_inds = max_vals > supp_thresh;
    sample_x = sample_x(nz_inds);
    sample_y = sample_y(nz_inds);
    %max_vals = max_vals(nz_inds);    
    
    sample_inds = sub2ind(size(im), sample_y, sample_x);
    supp_im = zeros(size(im));
    supp_im(sample_inds) = im(sample_inds);
    supp_im = supp_im(1:h,1:w);
else
    disp('option must be either sliding or distinct');
    supp_im = [];
end
    
    