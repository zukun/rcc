function sift = ExtractSIFT(imgname, nstep, nsize)

%fixed parameters
MAX_SCALE = 12;
maxbound = MAX_SCALE + 0.5*MAX_SCALE + 1;

if ( max(nsize) > MAX_SCALE )
    disp(['nsize should be less or equal to ' num2str(MAX_SCALE) '.']);
    return;
end
nsize(mod(nsize,2) == 1) = nsize(mod(nsize,2) ==1) + 1;


im = imread(imgname);
[h,w,~] = size(im);
scale = 1.0;
if (w > 320 || h > 320)
    if ( w > h )
        scale = 320/w;
    else
        scale = 320/h;
    end
end 

if ( ndims(im) == 3 )
    im = single(rgb2gray(im));
else
    im = single(im);
end
if ( scale ~= 1.0 )
    im = imresize(im, scale);
end

w = scale*w;
h = scale*h;

scaler = max([w,h])/320; 
adaptive_step = floor(scaler*nstep);

for j = 1 : numel(nsize)
    bound = maxbound - (nsize(j) + 0.5*nsize(j));
    bounds = [bound, bound, floor(w-bound), floor(h-bound)];
    [f, d] = vl_dsift(im, 'Step', adaptive_step, 'Size', nsize(j), 'Bounds', bounds, 'Norm');

    sift(j).ptrs = f(1:2,:);
    sift(j).descriptors = d;
    sift(j).strength = f(3,:);
    sift(j).imwidth = w;
    sift(j).imheight = h;
    sift(j).step = adaptive_step;
    sift(j).size = nsize(j);
end

 