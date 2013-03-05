function [segments, graph_cut_params, cost_type] =...
    graphCutSegmentationWrapper(merge_segments, im, gPb_thin, gPb_orient)

% parameter
img_size = size(gPb_thin);


%% clustering of shape prior
n_target = 400;
min_overlap = 0.5;
[cluster_id unused merge_seg_score] =...
    groupSegmentWrapper(merge_segments, gPb_thin, n_target, min_overlap);


%% graph-cut
do_clustering = true;
[graph_cut_segments graph_cut_params cost_type segment_cluster_id] =...
    graphCutSegmentation(merge_segments, cluster_id,...
    gPb_thin, gPb_orient, im, do_clustering);
segments = graph_cut_segments;


% add top merge segments
[unused r] = sort(merge_seg_score, 'descend');
cut_off_idx = ceil(numel(r)*0.025); 
merge_segments = merge_segments(:, r(1:cut_off_idx));
cost_type = [cost_type nan(1, size(merge_segments,2))];
graph_cut_params = [graph_cut_params nan(1, size(merge_segments,2))];
segments = [segments merge_segments];
for i = 1 : cut_off_idx
    cluster_id{end+1} = r(i);
    segment_cluster_id = [segment_cluster_id numel(cluster_id)];
end


%% clustering
sel_tracker = 1:size(segments,2);
if ( do_clustering )

    seg_size = sum(segments,1)./prod(img_size);
    [unused size_r] = sort(seg_size,'descend');
    big_seg_inds = size_r(1:10);
    big_segments = segments(:, big_seg_inds);
    big_graph_cut_params = graph_cut_params(big_seg_inds);
    big_cost_type = cost_type(big_seg_inds);
    big_sel_tracker = big_seg_inds;
    
    N_TARGET = 600;
    n_super_save =30;
    n_seg = numel(graph_cut_params);
    overlap_thresh = 0.95;
    disp(['clustering:       ' num2str(overlap_thresh)]);
    n_before = n_seg;
    
    % cut score
    e = gPb_thin > 0.0;
    img_size = size(e);
    [im_dt im_dtL] = bwdist(e);
    [cut_score_] =...
        computeCutScore2(segments, im_dt, im_dtL, gPb_thin, img_size);
    bdry_tf = pickBdrySegments(segments, img_size);
    n_seg = size(segments,2);
    cut_score = zeros(n_seg,1);
    cut_score(bdry_tf) = cut_score_(bdry_tf,2);
    cut_score(~bdry_tf) = cut_score_(~bdry_tf,1);    
       
     % super-save
    seg_score = cut_score;
    [unused r] = sort(seg_score,'descend');
    super_save_sel = r(1:n_super_save);
        
    % clustering
    sel = clusterGraphCutSegments(segments, overlap_thresh, gPb_thin, img_size);
    sel = unique([sel(:);super_save_sel(:)]);
       
    
    cost_type = cost_type(sel);
    graph_cut_params = graph_cut_params(sel);
    segments = segments(:, sel);
    cut_score = cut_score(sel);
    sel_tracker = sel_tracker(sel);
    segment_cluster_id = segment_cluster_id(sel);

    % add big
    segments = [segments big_segments];
    graph_cut_params = [graph_cut_params big_graph_cut_params];
    cost_type = [cost_type big_cost_type];  
    sel_tracker = [sel_tracker(:); big_sel_tracker(:)];    
    
    n_after = size(segments,2);
    disp(['nseg: ' num2str(n_before) '--> ' num2str(n_after)]); 
end


    