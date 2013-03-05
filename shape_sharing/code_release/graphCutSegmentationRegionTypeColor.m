function [graph_cut_segments graph_cut_params segment_cluster_id] =...
    graphCutSegmentationRegionTypeColor(shape_prior_segments,...
    cluster_id, gPb_thin, im, params)

% parameters
n_cluster = numel(cluster_id);
img_size = size(gPb_thin);
% shape_lambda = 0.20;
% % shape_lambda = 0.50;
% small_obj_size_thresh = 0.05;
% small_obj_lambda = [-0.125, linspace(0, 0.20 ,7)];
% large_obj_lambda = [-0.125, 0, 0.125, 0.20];
% smoothness_const = 5.0;
% min_overlap = 0.5;
% min_obj_size = 250;
shape_lambda = params.shape_lambda;
small_obj_size_thresh = params.small_obj_size_thresh;
small_obj_lambda = params.small_obj_lambda;
large_obj_lambda = params.large_obj_lambda;
smoothness_const = params.smoothness_const;
min_overlap_initguess = params.min_overlap_initguess;
min_overlap = params.min_overlap;
min_obj_size = params.min_obj_size;
fix_seed = params.fix_seed;
bbox_margin = params.bbox_margin;
% cohesion_thresh = params.cohesion_thresh;
n_max_params = numel(small_obj_lambda);

% color quantize
color_bin = 12;
while (color_bin <= 255)
    [unused color_index_dist color_index_map nBin] =...
        RGBQuantization(im, color_bin, 0.95, false);
    if ( nBin < 100 )
        color_bin = color_bin + 12;
    else
        break;
    end
end

%% compuate cost
% region-type 
n_color_bin = size(color_index_dist,1);
[fg_color_hist, bg_color_hist, bboxs] =...
    buildRegionTypeColorModel(shape_prior_segments, cluster_id,...
    color_index_map, n_color_bin, bbox_margin);

% shape prior
[fg_shape_prior_cost shape_priors] =...
    computeShapePriorCost(shape_prior_segments,...
    cluster_id, shape_lambda, bbox_margin, img_size);

% smoothness
mu = mean(gPb_thin(gPb_thin>0));
smoothness = exp(-gPb_thin./mu);
    

%% graph cut
% region-color + shape model
graph_cut_segments = false(prod(img_size), n_cluster*n_max_params);
graph_cut_params = zeros(1, n_cluster*n_max_params);
segment_cluster_id = zeros(1, n_cluster*n_max_params);
seg_cnt = 0;
for i = 1 : n_cluster
    
    % graph-cut parameters
    bbox = bboxs(:,i);
    shape_prior = shape_priors(:,i);
    problem_size = sum(shape_prior)/prod(img_size);

%     w = max(bbox(2)-bbox(1),0);
%     h = max(bbox(4)-bbox(3),0);
%     problem_size = (w*h)/prod(img_size);

    if ( problem_size == 0 )
        continue;
    end
    if ( problem_size < small_obj_size_thresh )
        lambda = small_obj_lambda;
    else 
        lambda = large_obj_lambda;
    end
    
    % cost
    fg_color_cost =...
        computeColorHistDataCost(fg_color_hist{i}, bg_color_hist{i}, color_index_map);
    fg_shape_cost = fg_shape_prior_cost{i};
    fg_cost = (fg_color_cost + fg_shape_cost)./2;
    
   
    % graph-cut
    [segments params n_comp] =...
        computeGraphCut(fg_cost, lambda, shape_prior,...
        smoothness, smoothness_const, img_size, bbox, min_overlap_initguess, min_obj_size, fix_seed);
    % remove unreliable solutions
    sel = pruneGraphCutSegments(segments, n_comp, shape_prior, bbox,...
        img_size, min_overlap, min_obj_size);
    segments = segments(:,sel);
    params = params(:,sel);
    n = size(segments,2);
    if ( n > 0 )
        graph_cut_segments(:, seg_cnt+1:seg_cnt+n) = segments;
        graph_cut_params(seg_cnt+1:seg_cnt+n) = params;
        segment_cluster_id(seg_cnt+1:seg_cnt+n) = i;
        seg_cnt = seg_cnt + n;
    end  
end

graph_cut_segments = graph_cut_segments(:,1:seg_cnt);
graph_cut_params = graph_cut_params(1:seg_cnt);
segment_cluster_id = segment_cluster_id(1:seg_cnt);
