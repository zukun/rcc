cur_dir = pwd;

try
base_dir = which('compile_proposals.m');
base_dir = base_dir(1:end-length('compile_proposals.m'));

% Geometric context
cd(fullfile(base_dir, 'src/GeometricContext/tools/misc'));
mex treevalc.cc

% Proposal code
cd(fullfile(base_dir, 'src/objectProposals/utils'));
mex get_region_overlap_mex.cc


% External code
cd(fullfile(base_dir, 'external', 'bresenham_mex'));
mex LineTwoPnts.cpp bresenham.cpp
mex SegInMat.cpp bresenham.cpp

cd(fullfile(base_dir, 'external', 'lightspeed'));
install_lightspeed

cd(fullfile(base_dir, 'external', 'maxflow-v3.01'));
mex mex_maxflow.cpp maxflow.cpp graph.cpp

cd(fullfile(base_dir, 'external', 'pwmetric'));
slmetric_pw_compile

cd(fullfile(base_dir, 'external', 'segment'));
mex segment_fh_mex.cc -I.

cd(fullfile(base_dir, 'external', 'textons'));
mex anigauss.c anigauss_mex.c

fprintf('\n\n****Compilation Complete!****\n');
fprintf('If you''re having trouble with PB computation, see external/segbench for compiling instructions\n');

catch me
fprintf('Error compiling: %s\n', me.message);
end

cd(cur_dir);
