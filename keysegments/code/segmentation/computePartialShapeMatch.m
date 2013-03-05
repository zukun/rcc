function [shapeMask,pred_fg_bplrs] = computePartialShapeMatch(bplr_file,fgFeats,bgFeats,Segs,selind,nr,nc)

h = fspecial('gaussian', 75, 10);

load(bplr_file, 'BPLR', 'pb_phog', 'color_hist');
color_hist.feat_vecs = single(reshape(color_hist.feat_vecs,[69 numel(BPLR.feats)]));

% compute distances to fg model
fg_phog_dist = slmetric_pw(pb_phog.feat_vecs, single(fgFeats.pb_phog), 'eucdist');
fg_color_dist = slmetric_pw(color_hist.feat_vecs, single(fgFeats.color_hist), 'eucdist');
fg_dist = fg_phog_dist+0.5*fg_color_dist;
[fg_bestMatchDist, fg_bestMatchInd] = min(fg_dist,[],2);

% compute distances to bg model
bg_phog_dist = slmetric_pw(pb_phog.feat_vecs, single(bgFeats.pb_phog), 'eucdist');
bg_color_dist = slmetric_pw(color_hist.feat_vecs, single(bgFeats.color_hist), 'eucdist');  
bg_dist = bg_phog_dist+0.5*bg_color_dist;
[bg_bestMatchDist, bg_bestMatchInd] = min(bg_dist,[],2);

pred_fg_bplrs = find((fg_bestMatchDist - bg_bestMatchDist) < 0);

topMatchInd = fg_bestMatchInd(pred_fg_bplrs);
topMatchScore = exp(-fg_bestMatchDist(pred_fg_bplrs)/mean(mean(fg_dist)));

shapeMask = zeros(nr,nc);
for j=1:length(topMatchInd)
    matchedFrameNum = fgFeats.frame_num(topMatchInd(j));
    matchedFrameLocs = fgFeats.locs(:,topMatchInd(j));

%         matchedFrameScale = fgFeats.scales(topMatchInd(j));
%         scaleX = sqrt(pb_phog.feat_scales(pred_fg_bplrs(j))/matchedFrameScale);
%         scaledMatchMask = imresize(Segs(selind(matchedFrameNum)).proposal, scaleX, 'nearest');

    scaleX = 1;
    scaledMatchMask = Segs(selind(matchedFrameNum)).proposal;
    scaledMatchedFrameLocs = round(matchedFrameLocs*scaleX);

    [mask_y mask_x] = find(scaledMatchMask==1);
    thisLocs = round(double(pb_phog.feat_centers(:,pred_fg_bplrs(j))));
    translatedMaskLocs = [mask_x mask_y]' + repmat((thisLocs - scaledMatchedFrameLocs), [1 length(mask_x)]);
    invalidInd = ((translatedMaskLocs(1,:)<=0) | (translatedMaskLocs(1,:)>nc) | (translatedMaskLocs(2,:)<=0) | (translatedMaskLocs(2,:)>nr));
    translatedMaskLocs(:,invalidInd) = [];

    maskInd = sub2ind([nr nc], translatedMaskLocs(2,:), translatedMaskLocs(1,:));
    newMask = zeros(nr,nc);
    newMask(maskInd) = 1;

    % weight vote according to match similarity
    shapeMask = shapeMask + newMask*topMatchScore(j);
end    
shapeMask = shapeMask/max(max(shapeMask));
shapeMask = shapeMask+0.1;
shapeMask = shapeMask/max(max(shapeMask));
shapeMask = imfilter(shapeMask,h); 

