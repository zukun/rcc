%%%% image, feature, classifier directories
imgdir = 'images/';
featdir = 'features/';
outdir = 'classifier/';

categories = {'aeroplane','bird','boat','bus'};

for c=1:length(categories)

  cat = categories{c};

  %%% 
  signofclass = 1;

  %%% SVM bias parameter
  rho = load([outdir '/' cat '-rho']);

  %%% load SVM weights for each dimension (visual word)
  tmp = load([outdir '/' cat '-centre-out']);
  centre_scores = signofclass*(tmp(:,2) + rho);


  files = dir([imgdir '/' cat '*.jpg']);

  for f=1:length(files)

    [tmp,filename,tmp,tmp] = fileparts(files(f).name);

    im = imread([imgdir '/' filename '.jpg']);

    %%% superpixel segmentation
    Sp = load([featdir '/' filename '.seg.txt']);

    %%% bag of words features file
    bag = load([featdir '/' filename '.soup.txt']);

    %%% superpixel adjacency graph (columns 2,3 denote adjacent superpixel node
    %%% indices 
    g = load([featdir '/' filename '.graph.txt']); 

    %%% compute node weights for each superpixel
    seg_scores = bag * centre_scores;

    %%% construct adjacency matrix
    adjmat = zeros(g(1),g(1));
    idx = sub2ind(size(adjmat), g(:,2)+1, g(:,3)+1);
    adjmat(idx) = 1;

    %%% alternatively use get_graph function to directly obtain
    %%% adjacency matrix
    %% adjmat = get_graph(Sp);

    %%% write the input for solving the MWCS instance using heinze code
    %%% See Edges.txt, scores0001.txt for the format of these files
    %%% The solution will be stored in result****.txt
    randfile = floor(rand(1)*100000);
    scorefile = sprintf('score%d.txt',randfile);
    edgefile = sprintf('edges%d.txt',randfile);
    resfile = sprintf('result%d.txt',randfile);

    fid = fopen(scorefile,'w');
    fprintf(fid,'label score\n');
    nodes = [1:length(seg_scores)];
    fprintf(fid,'%d(%d) %f\n',[nodes' nodes' seg_scores]');
    fclose(fid);

    [I, J] = find(adjmat);
    fid = fopen(edgefile,'w');
    fprintf(fid,'%d(%d) %d(%d)\n',[I I J J]');
    fclose(fid);

    %%% run the MWCS solver
    tic
    [r,s] = system(['heinz.py -e ' edgefile ' -n ' scorefile]);
    time_ms = toc;
    times = load([scorefile '.time']);
    fprintf('matlab time - %f, dhea time %f\n', time_ms, sum(times));
    system(['sed -e "/.*n\/a/d" ' scorefile '.0.hnz | sed 1d | sed' ...
	    ' ''$d'' | sed -e "s/\([0-9]*\)\([^ \t]*\)\(.*' ...
	    '\)/\1\3/g" > ' resfile]);

    %%% read the result
    [nodes, scores] = textread(resfile,'%d %f');
    res_nodes = nodes;

    fprintf('%s\n',['rm *' num2str(randfile) '.txt']);
    system(['rm *' num2str(randfile) '.txt*']);

    %%% compute the segmentation mask result
    bcmask = zeros(size(Sp));
    for n=1:length(nodes)
      idx = find(Sp == nodes(n));
      bcmask(idx) = 1;
    end
    
    %%% segmented image
    res_segim = zeros(size(im));
    for n=1:3
      tmpim = im(:,:,n);
      res_segim(:,:,n) = double(tmpim) .* bcmask;
    end

    %%% plot results
    subplot(2,2,1), imshow(im);
    subplot(2,2,2), imagesc(Sp), axis image;
    subplot(2,2,3), imshow(bcmask);
    subplot(2,2,4), imshow(uint8(res_segim));
    pause
  end

end