function [Length, Edges] = computeEMSTofPoints(rr, show_flag, gamma)

% function [Length, Edges] = computeEMSTofPoints(rr, show_flag = 0, gamma = 1)
% computes the Euclidean Minimum Spanning Tree of a bunch of input
% two-dimensional points [rr] of size N (number of points) x 2.
% if show_flag is greater than zero, it will plot the EMST in a new figure
% gamma = exponential weight on the edges when summing them up (default is
% 1)
% Length: the total exponentially weigthed length of the EMST: sum_{i}
% \sum_{j \in N_i} \|e_{ij}\|^{\gamma}, where e_{ij} is the Euclidean
% distance between the connected samples i and j and N_i is the
% neighborhood of sample i, i.e., the set of points connected with i.

% Edges: the edges of the EMST (Matlab-style index) [at most, if all points are unique] numOfEdges (N - 1) x 2

% ************************************
% NEEDS: computeEMST.dll (mex) file!!!
% ************************************

% AUTHOR: Mert R. Sabuncu (c) 2004-2008

if (nargin < 2)
    show_flag = 0;
end

if (nargin < 3)
    gamma = 1;
end

[rr_unique, I, J] = unique(rr, 'rows');

try
    TRI = delaunay(rr_unique(:,1)', rr_unique(:,2)');
catch
    disp('delaunay option changed: "OJ" added');
    TRI = delaunay(rr_unique(:,1)', rr_unique(:,2)', {'Qt','Qbb','Qc','QJ'});
end

EDGE = [TRI(:,1) TRI(:,2); TRI(:,1) TRI(:,3); TRI(:,2) TRI(:,3)];

EDGE = unique(EDGE,'rows');

[Length, Edges] = computeEMST(rr_unique', EDGE',gamma); %dll file written by Mert Rory Sabuncu

% switch to original indexing

Edges(1,:) = I(Edges(1,:));
Edges(2,:) = I(Edges(2,:));

Edges = Edges';

if (show_flag)
    
    figure,
    hold on
    line(rr(Edges(1,:),1),rr(Edges(1,:),2))
    for i=1:size(Edges,1)

        line(rr(Edges(i,:),1),rr(Edges(i,:),2))

    end
    plot(rr(:,1),rr(:,2),'r*');
    hold off;
end

