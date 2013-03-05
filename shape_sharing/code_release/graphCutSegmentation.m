function [graph_cut_segments graph_cut_params cost_type segment_cluster_id] =...
    graphCutSegmentation(shape_prior_segments, cluster_id,...
    gPb_thin, gPb_orient, im, do_clustering)

% parameter
img_size = size(im);
img_size = img_size(1:2);
% graph-cut default params
params.small_obj_size_thresh = 0.05;
params.small_obj_lambda = [-0.165, linspace(0, 0.20 ,7)];
params.large_obj_lambda = [-0.165, 0, 0.0625, 0.125, 0.20];
params.smoothness_const = 5.0;
params.min_obj_size = 250;
params.pick_rate = 0.75;
params.min_overlap = 0.5;
params.fix_seed = false;

% output 
graph_cut_segments = logical([]);
graph_cut_params = [];
cost_type = [];
segment_cluster_id = [];

method_cnt = 0;
%% region-type color -- weak shape prior
bbox_margin = [0.0,0.25,0.5];
for i = 1 : numel(bbox_margin)
    method_cnt = method_cnt + 1;
    params.shape_lambda = 0.0;
    params.bbox_margin = bbox_margin(i);
    params.min_overlap_initguess = 0.5;
    params.min_overlap = 0.5;
    params.fix_seed = false;
     params.cluster_overlap_thresh = 0.95;    
    overlap_thresh = params.cluster_overlap_thresh;    
    
    [rgn_type_segments rgn_type_params seg_cluster_id] =...
    graphCutSegmentationRegionTypeColorWrapper(shape_prior_segments,...
        cluster_id, gPb_thin, im, params, img_size, do_clustering, overlap_thresh);
    n = numel(rgn_type_params);
    cost_type = [cost_type method_cnt*ones(1,n)];
    graph_cut_segments = [graph_cut_segments rgn_type_segments];
    graph_cut_params = [graph_cut_params rgn_type_params];
    segment_cluster_id = [segment_cluster_id seg_cluster_id];
end

%% region-type color + shape -- strong shape prior
bbox_margin = 0.25;
for i = 1 : numel(bbox_margin)
    method_cnt = method_cnt + 1;
    params.shape_lambda = 0.25;
    params.bbox_margin = bbox_margin(i);
    params.min_overlap_initguess = 0.50;
    params.min_overlap = 0.50;
    params.fix_seed = false;
%     params.small_obj_lambda = [-0.165, 0, 0.0625, 0.125];
%     params.large_obj_lambda = [-0.165, 0, 0.0625, 0.125];
    params.cluster_overlap_thresh = 0.95;    
    overlap_thresh = params.cluster_overlap_thresh;    
    
    [rgn_type_segments rgn_type_params seg_cluster_id] =...
    graphCutSegmentationRegionTypeColorWrapper(shape_prior_segments,...
        cluster_id, gPb_thin, im, params, img_size, do_clustering, overlap_thresh);
    n = numel(rgn_type_params);
    cost_type = [cost_type method_cnt*ones(1,n)];
    graph_cut_segments = [graph_cut_segments rgn_type_segments];
    graph_cut_params = [graph_cut_params rgn_type_params];
    segment_cluster_id = [segment_cluster_id seg_cluster_id];
end

% %% contour-type color + shape -- weak shape prior
% bbox_margin = [0.05,0.25,0.5];
% for i = 1 : numel(bbox_margin)
%     method_cnt = method_cnt + 1;
%     params.shape_lambda = 0.0;
%     params.bbox_margin = bbox_margin(i);
%     params.min_overlap_initguess = 0.50;
%     params.min_overlap = 0.50;
%     params.fix_seed = false;
%     params.cluster_overlap_thresh = 0.95;
%     overlap_thresh = params.cluster_overlap_thresh;    
%     
%     [ctr_type_segments ctr_type_params seg_cluster_id] =...
%         graphCutSegmentationContourTypeColorWrapper(shape_prior_segments,...
%         cluster_id, gPb_thin, gPb_orient, im, params, img_size, do_clustering, overlap_thresh);
%     n = numel(ctr_type_params);
%     cost_type = [cost_type method_cnt*ones(1,n)];
%     graph_cut_segments = [graph_cut_segments ctr_type_segments];
%     graph_cut_params = [graph_cut_params ctr_type_params];
%     segment_cluster_id = [segment_cluster_id seg_cluster_id];
% end



%% contour-type color + shape -- strong shape prior
bbox_margin = 0.25;
for i = 1 : numel(bbox_margin)
    method_cnt = method_cnt + 1;
    params.shape_lambda = 0.25;
    params.bbox_margin = bbox_margin(i);
    params.min_overlap_initguess = 0.50;
    params.min_overlap = 0.50;
    params.fix_seed = false;
%     params.small_obj_lambda = [-0.165, 0, 0.0625, 0.125];
%     params.large_obj_lambda = [-0.165, 0, 0.0625, 0.125];    
    params.cluster_overlap_thresh = 0.95;
    overlap_thresh = params.cluster_overlap_thresh;    
    
    [ctr_type_segments ctr_type_params seg_cluster_id] =...
        graphCutSegmentationContourTypeColorWrapper(shape_prior_segments,...
        cluster_id, gPb_thin, gPb_orient, im, params, img_size, do_clustering, overlap_thresh);
    n = numel(ctr_type_params);
    cost_type = [cost_type method_cnt*ones(1,n)];
    graph_cut_segments = [graph_cut_segments ctr_type_segments];
    graph_cut_params = [graph_cut_params ctr_type_params];
    segment_cluster_id = [segment_cluster_id seg_cluster_id];
end

% %% fit to the indivudal shape priors
% bbox_margin = 0.10;
% n_prior = size(shape_prior_segments,2);
% cluster_id_each = cell(1,n_prior);
% for i = 1 : n_prior
%     cluster_id_each{i} = i;
% end
% for i = 1 : numel(bbox_margin)
%     method_cnt = method_cnt + 1;
%     params.shape_lambda = 0.25;
%     params.bbox_margin = bbox_margin(i);
%     params.min_overlap_initguess = 0.5;
%     params.min_overlap = 0.5;
%     params.fix_seed = false;
%     params.small_obj_lambda = 0.0625;
%     params.large_obj_lambda = 0.0625;    
%     params.cluster_overlap_thresh = 0.95;
%     overlap_thresh = params.cluster_overlap_thresh;    
%     
%     
%     [ctr_type_segments ctr_type_params seg_cluster_id] =...
%         graphCutSegmentationContourTypeColorWrapper(shape_prior_segments,...
%         cluster_id_each, gPb_thin, gPb_orient, im, params, img_size, do_clustering, overlap_thresh);
%     n = numel(ctr_type_params);
%     cost_type = [cost_type method_cnt*ones(1,n)];
%     graph_cut_segments = [graph_cut_segments ctr_type_segments];
%     graph_cut_params = [graph_cut_params ctr_type_params];
%     segment_cluster_id = [segment_cluster_id nan*seg_cluster_id];
% end

end


function [ctr_type_segments ctr_type_params segment_cluster_id] =...
    graphCutSegmentationContourTypeColorWrapper(shape_prior_segments,...
    cluster_id, gPb_thin, gPb_orient, im, params, img_size, do_clustering, overlap_thresh)
% tic;
disp('processing: contour type');
disp(params);
[ctr_type_segments ctr_type_params segment_cluster_id] =...
    graphCutSegmentationContourTypeColor(shape_prior_segments,...
    cluster_id, gPb_thin, gPb_orient, im, params);
n_before = numel(ctr_type_params);
% clustering
if ( do_clustering )
%     overlap_thresh = 0.95;
    if ( n_before > 0 )
        sel = clusterGraphCutSegments(ctr_type_segments, overlap_thresh, gPb_thin, img_size);
        ctr_type_segments = ctr_type_segments(:, sel);
        ctr_type_params = ctr_type_params(sel);
        segment_cluster_id = segment_cluster_id(sel);
    end
end
n_after = numel(ctr_type_params);
% disp(['nseg: ' num2str(n_before) '-->' num2str(n_after)]);
% toc;
end

function [rgn_type_segments rgn_type_params segment_cluster_id] =...
    graphCutSegmentationRegionTypeColorWrapper(shape_prior_segments,...
    cluster_id, gPb_thin, im, params, img_size, do_clustering, overlap_thresh)

% tic;
disp('processing: region type');
disp(params);
[rgn_type_segments rgn_type_params segment_cluster_id] =...
    graphCutSegmentationRegionTypeColor(shape_prior_segments,...
    cluster_id, gPb_thin, im, params);
n_before = numel(rgn_type_params);
% clustering
if ( do_clustering )
%     overlap_thresh = 0.95;    
    if ( n_before > 0 )
        sel = clusterGraphCutSegments(rgn_type_segments, overlap_thresh, gPb_thin, img_size);
        rgn_type_segments = rgn_type_segments(:, sel);
        rgn_type_params = rgn_type_params(sel);
        segment_cluster_id = segment_cluster_id(sel);
    end
end
n_after = numel(rgn_type_params);
% disp(['nseg: ' num2str(n_before) '-->' num2str(n_after)]);
% toc;
end

