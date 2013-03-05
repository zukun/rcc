function deformationCost = CheckGeometry(ptrs1, ptrs2, deformation_range)

n = size(ptrs1, 2);
disparity1x = zeros(n,n);
disparity1y = zeros(n,n);
disparity2x = zeros(n,n);
disparity2y = zeros(n,n);

distance = zeros(n,n);
% distance2 = zeros(n,n);

for i = 1 : n
    disparity1x(i,:) = ptrs1(1,:) - repmat(ptrs1(1,i),1,n);
    disparity1y(i,:) = ptrs1(2,:) - repmat(ptrs1(2,i),1,n);
    disparity2x(i,:) = ptrs2(1,:) - repmat(ptrs2(1,i),1,n);
    disparity2y(i,:) = ptrs2(2,:) - repmat(ptrs2(2,i),1,n);
    
    distance(i,:) = sqrt(sum((ptrs1 - repmat(ptrs1(:,i),1,n)).^2, 1));
%   distance2(i,:) = sqrt(sum((ptrs2 - repmat(ptrs2(:,i),1,n)).^2, 1));
end

deformationCost = sqrt((disparity1x - disparity2x).^2 + (disparity1y - disparity2y).^2);
largedistInds = distance > deformation_range;
magnitudes = distance(largedistInds)./deformation_range;
min_magnitude = 1.0;
max_magnitude = sqrt(2)/deformation_range;
%med_magnitude = (min_magnitude + max_magnitude)/2;
damping_factors = (-0.2/(max_magnitude-min_magnitude))*(magnitudes-min_magnitude) + 1.0;
%damping_factors(damping_factors < 0.75 ) = 0.75;
deformationCost(largedistInds) =...
    deformationCost(largedistInds).*damping_factors;

    