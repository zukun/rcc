function [fgFeats bgFeats] = getKeySegmentBPLRFeatures(Segs, bplrdir)

bplr_file = [bplrdir Segs(1).imname '_bplr.mat'];
load(bplr_file, 'pb_phog');
        
fg_pb_phog = zeros(size(pb_phog.feat_vecs,1),10000);
fg_color_hist = zeros(69,10000);
fg_scales = zeros(1,10000);
fg_locs = zeros(2,10000);
fg_frame_num = zeros(1,10000);

bg_pb_phog = zeros(size(pb_phog.feat_vecs,1),100000);
bg_color_hist = zeros(69,100000);
bg_scales = zeros(1,100000);
bg_locs = zeros(2,100000);
bg_frame_num = zeros(1,10000);

fgnum = 1;
bgnum = 1;
for i=1:length(Segs)
        bplr_file = [bplrdir Segs(i).imname '_bplr.mat'];
        load(bplr_file, 'BPLR', 'pb_phog', 'color_hist');

        ind = zeros(1,length(BPLR.feats));
        for j=1:length(BPLR.feats)
            ind(j) = Segs(i).proposal(round(pb_phog.feat_centers(2,j)), round(pb_phog.feat_centers(1,j)));
        end
        color_hist.feat_vecs = reshape(color_hist.feat_vecs,[69 numel(BPLR.feats)]);
        
        fgNdx = find(ind==1);
        numfgfeats = length(fgNdx);         
        fg_pb_phog(:,fgnum:fgnum+numfgfeats-1) = double(pb_phog.feat_vecs(:,fgNdx));
        fg_color_hist(:,fgnum:fgnum+numfgfeats-1) = double(color_hist.feat_vecs(:,fgNdx));
        fg_scales(:,fgnum:fgnum+numfgfeats-1) = double(pb_phog.feat_scales(fgNdx));        
        fg_locs(:,fgnum:fgnum+numfgfeats-1) = round(double(pb_phog.feat_centers(:,fgNdx)));
        fg_frame_num(:,fgnum:fgnum+numfgfeats-1) = i;
        fgnum = fgnum + numfgfeats;

        bgNdx = find(ind==0);
        numbgfeats = length(bgNdx);         
        bg_pb_phog(:,bgnum:bgnum+numbgfeats-1) = double(pb_phog.feat_vecs(:,bgNdx));
        bg_color_hist(:,bgnum:bgnum+numbgfeats-1) = double(color_hist.feat_vecs(:,bgNdx));
        bg_scales(:,bgnum:bgnum+numbgfeats-1) = double(pb_phog.feat_scales(bgNdx));        
        bg_locs(:,bgnum:bgnum+numbgfeats-1) = round(double(pb_phog.feat_centers(:,bgNdx)));
        bg_frame_num(:,bgnum:bgnum+numbgfeats) = i;
        bgnum = bgnum + numbgfeats;
end

fgFeats.pb_phog = fg_pb_phog(:,1:fgnum-1);
fgFeats.color_hist = fg_color_hist(:,1:fgnum-1);
fgFeats.scales = fg_scales(:,1:fgnum-1);
fgFeats.locs = fg_locs(:,1:fgnum-1);
fgFeats.frame_num = fg_frame_num(:,1:fgnum-1);

bgFeats.pb_phog = bg_pb_phog(:,1:bgnum-1);
bgFeats.color_hist = bg_color_hist(:,1:bgnum-1);
bgFeats.scales = bg_scales(:,1:bgnum-1);
bgFeats.locs = bg_locs(:,1:bgnum-1);
bgFeats.frame_num = bg_frame_num(:,1:bgnum-1);
