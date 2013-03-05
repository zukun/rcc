function n_borders = get_border(contour, max_margin)

% top row
n_ones = sum(contour(1:max_margin, :), 2);
n_bcup = max(n_ones);

% down row
n_ones = sum(contour(end - max_margin + 1 : end, :), 2);
n_bcdown = max(n_ones);

% left column
n_ones = sum(contour(:, 1 :max_margin), 1);
n_bcleft = max(n_ones);

% right column
n_ones = sum(contour(:, end - max_margin + 1: end), 1);
n_bcright = max(n_ones);

n_borders = n_bcup + n_bcdown + n_bcleft + n_bcright;