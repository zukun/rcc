function res=draw_seg_bound(im,seg,varargin)
th = 3;
if(nargin == 3)
  th = varargin{1};
end
e = edge_im(seg,th);
idx = find(e);
tmpim = im(:,:,1);
tmpim(idx) = 255;
res(:,:,1) = tmpim;
tmpim = im(:,:,2);
tmpim(idx) = 255;
res(:,:,2) = tmpim;
tmpim = im(:,:,3);
tmpim(idx) = 255;
res(:,:,3) = tmpim;