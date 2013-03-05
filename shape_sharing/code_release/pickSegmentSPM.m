function sel = pickSegmentSPM(segments, seg_score_for_bin, img_size, n_grid, sample_rate, N_MAX_SEG, do_strict)

x_grid_size = img_size(2)/n_grid;
y_grid_size = img_size(1)/n_grid;
n_seg = size(segments,2);
segments = reshape(segments,[img_size,n_seg]);
[y x] = arrayfun(@(idx)(find(segments(:,:,idx))), 1:n_seg, 'uniformoutput', false);
cx = cellfun(@mean,x);
cy = cellfun(@mean,y);

x_bin = ceil(cx./x_grid_size);
y_bin = ceil(cy./y_grid_size);
bin_ind = sub2ind([n_grid,n_grid], y_bin, x_bin);

n_bin = n_grid*n_grid;

% bin score
bin_score = zeros(n_bin,1);
for i = 1 : n_bin
    inds = bin_ind == i;
    if  ( sum(inds) > 0 )
        bin_score(i) = mean(seg_score_for_bin(inds));
    end
end
bin_score = bin_score./mean(bin_score(bin_score>0));
bin_score = bin_score./mean(bin_score);

% selection
sel = [];    
if (do_strict)
    mag =1;
    for i = 1 : n_bin
        inds = find(bin_ind == i);
        n = ceil(mag*numel(inds)*sample_rate);
        n = min(n,numel(inds));
        sel = [sel inds(1:n)];
    end
else
    mag = 1;    
    while (1)
        bin_gain = mag*(bin_score - 1);

        % selection
        sel = [];
        for i = 1 : n_bin
            inds = find(bin_ind == i);
            n = ceil((1+bin_gain(i))*numel(inds)*sample_rate);
            n = min(n,numel(inds));
            sel = [sel inds(1:n)];
        end

        % stop
%         if ( numel(sel) > N_MAX_SEG || mag > 10 )
%             break;
%         else
%             mag = mag + 1;
%         end
        if ( numel(sel) > N_MAX_SEG )
            damp = N_MAX_SEG/numel(sel);
            % selection
            sel = [];
            for i = 1 : n_bin
                inds = find(bin_ind == i);
                n = ceil(damp*(1+bin_gain(i))*numel(inds)*sample_rate);
                n = min(n,numel(inds));
                sel = [sel inds(1:n)];
            end
            break;
        elseif ( mag > 10 )
            break;
        else
            mag = mag + 1;
        end
    end
    
end



