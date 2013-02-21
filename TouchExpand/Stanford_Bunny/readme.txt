This folder contains the test dataset for the shape fitting model.

***************************************************
POINTS.DAT

points.dat contain the oriented points obtained from the range scans for the well-known Stanford Bunny model.
The original range scans were obtained from Stanford 3D scanning repository. Range scan dataset description:
-----------
Stanford Bunny
Source: Stanford University Computer Graphics Laboratory
Scanner: Cyberware 3030 MS
Number of scans: 10
Total size of scans: 362,272 points (about 725,000 triangles) 
------------

While reading the scans we assigned the same orientation for all points in the scan. Thus the points are only weakly-oriented.

Points.dat has an array of 7*N 32-bit floats in a binary format, where N is the number of points. 
Each 7-tuple corresponds to x,y,z=coordinates, px,py,pz=orientation vector, w=weight.

***************************************************
BBOX.DAT

Approximate bounding box of the model in (minx, miny, minz; maxx, maxy, maxz) format.

***************************************************
PARAMS.INI

Contains the parameters for our algorithm. Should be suitable for a large variety of the scenes.
Below is the description along with the recommended parameters.

1) RESOLUTION
Defines the resolution of the discrete grid, indicates the number of cubic voxels along of the x-side of the bounding box.

2) FLUX_STRENGTH = 3
Strength of the data term.

3) FLUX_DIFFUSE_RAD = 1
   FLUX_DIFFUSE_ITER = 3
These parameters define the spread of each point sample. Each flux vector is filtered with FLUX_DIFFUSE_RAD box filter FLUX_DIFFUSE_ITER iter times.

4) DENSITY_AMBIENT = 30
This parameter defines the strength of the uniform (Euclidean) regularization. 

5) DENSITY_STRENGTH = 0
   DENSITY_DIFFUSE_RAD = 4
   DENSITY_DIFFUSE_ITER = 3
These parameters allow to add spacially-varying regularization on top of the uniform regularization. May be useful if you belief that the density of your points is very non-uniform. Otherwise, may be switched off (DENSITY_STRENGTH = 0).
DENSITY_STRENGTH controls the strength of this additional regularization. DENSITY_DIFFUSE_RAD and DENSITY_DIFFUSE_ITER control the spread of each point sample contributing to the spatially-variant regularization term.

6) TOPOLOGY = {0,1,2}
Enforces the topological properties of the output model.
0 = FREE. Nothing is enforced.
1 = CLOSED model. The outer boundary is enforced to be outside the model
2 = "model on the pediment". Same as previous but the lower face of the bounding box (z = zmin) is not enforced to be outside.

7) NEIGHB_SYSTEM = {6, 14, 26}
Neighborhood system for each voxel, that affects the accuracy of the approximation of the Euclidean regularization term. 6-neighborhood should be fine for dense data without very large holes. 14-,18-or 26-neighborhood should give better result for the case of large holes at the expense of much higher memory and time consumption.

**************************************************
RESULT.DAT

The output file that is produced with our algorithm. The shape is output in a form of 3D binarized voxel grid (1=inside, 0=outside). Any isosurface extraction software can be used to turn this into surface representation. We provide a simple MATLAB script to do that (this is not the algorithm used to extract the surfaces for visualizations in the paper).
Result.dat has the following binary format:

3 32-bit integers: nx, ny, nz - the dimensions of the voxel grid.
nx*ny*nz 8-bit unsigned chars: the occupancy result - 1 = inside, 0 = outside.
3 32-bit floats: minx,miny,minz - coordinates of the corner of the bounding box
1 32-bit float: step - the side length of a single voxel (voxels are always cubic in our implementation)
