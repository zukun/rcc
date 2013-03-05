Shape Sharing for Object Segmentation

Jaechul Kim (jaechul@cs.utexas.edu) and Kristen Grauman (grauman@cs.utexas.edu) 

---------------------------------------------------------------------------
Install
---------------------------------------------------------------------------
This code was tested in MATLAB R2011a under Linux 64-bit.

---------------------------------------------------------------------------
Example
---------------------------------------------------------------------------
When you start the codes, you first should set up the path by SetupPath.m.
Then, 

(1) For segmenting an (arbitrary) image,
[masks, timing] = ComputeSegment(img_file)

(2) For the experiment in PASCAL2010 dataset
SegmentationPASCAL(img_idx)

(3) For the experiment in BSD300 dataset
SegmentationBSD(img_idx)

---------------------------------------------------------------------------
License
---------------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by 
the Free Software Foundation

---------------------------------------------------------------------------
References
---------------------------------------------------------------------------
Jaechul Kim and Kristen Grauman. Shape Sharing for Object Segmentation, ECCV2012

Bibtex

@INPROCEEDINGS{JKimShapeSharing,
TITLE = {{Shape Sharing for Object Segmentation}},
AUTHOR = {J. Kim and K. Grauman},
BOOKTITLE = eccv,
YEAR = {2012},
}