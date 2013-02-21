%a simple script that can be used to visualize the binary segmentation
%produced with Shape Fitting code
function ShowSegmentation(path)

if nargin == 0
    path = 'Stanford_Bunny\result.dat';
end

resultFile = fopen(path, 'rb');

dims = fread(resultFile, 3, 'int32')';
result = zeros(prod(dims),1, 'uint8');

PORTION = 30000000;
for i = 1:floor(prod(dims)/PORTION)
    result((i-1)*PORTION+1:i*PORTION) = fread(resultFile, PORTION, 'uint8');
end
result(floor(prod(dims)/PORTION)*PORTION+1:prod(dims)) = fread(resultFile, prod(dims)-floor(prod(dims)/PORTION)*PORTION, 'uint8');
result = reshape(result, dims);

disp('Result read. Now, extracting isosurface (may take a while)...');
[faces, vertices] = isosurface(result,0.5);


disp('Isosurface extracted. Displaying the result');

patch('Faces', faces,'Vertices', vertices,'FaceColor', [0.5 0.5 0.5], 'FaceLighting', 'flat',...
    'EdgeColor', 'none', 'SpecularStrength', 0, 'AmbientStrength', 0.4, 'DiffuseStrength', 0.6);

set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1],...
    'PlotBoxAspectRatioMode', 'manual');
lighting flat
rotate3d on;
camlight;

