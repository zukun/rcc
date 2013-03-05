function feats = normalizeFeats(feats)

N = sum(feats,2);
Nmat = repmat(N,[1 size(feats,2)]);
feats =feats./Nmat;

[r1,c] = find(isnan(feats)); 
[r2,c] = find(isinf(feats));
r = unique([r1 r2]); 
for i=1:length(r)
    feats(r(i),:) = 1/size(feats,2)*ones(1,size(feats,2));
end