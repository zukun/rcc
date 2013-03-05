function shape_score = SCMatchScore(n_angle_bin, n_length_bin, bw_inds, img_size, rsz, ref_shape_context, use_all_shape)

angle_bin = linspace(0,2*pi+1e-12,n_angle_bin+1);
l_bin = logspace(-1,log10(3),n_length_bin+1);
length_bin = zeros(1, numel(l_bin)+2);
length_bin(1) = -1;
length_bin(2:end-1) = l_bin;
length_bin(end) = 1e6;
proj_sc = Region2ShapeContextMex(bw_inds, img_size, angle_bin, length_bin, rsz);
s = sum(proj_sc,1);
s(s==0) = 1e-12;
proj_sc = proj_sc./repmat(s,size(proj_sc,1),1);

d = slmetric_pw(proj_sc, ref_shape_context, 'chisq');
if ( use_all_shape)
    shape_score = min(d,[],2);
else
    shape_score = diag(d);
end
shape_score(isnan(shape_score)) = inf;
