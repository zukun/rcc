function fg_cost = computeColorHistDataCost(fg_color_hist, bg_color_hist, color_index_map)

fg_color_score = fg_color_hist(color_index_map) - bg_color_hist(color_index_map);
mu = mean2(abs(fg_color_score));
fg_cost = 1./(1+exp(fg_color_score./mu));


