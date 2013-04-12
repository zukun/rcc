mrfstereo version 1.0

(c) Daniel Scharstein 10/4/2006

Stereo matcher front-end to MRF library

Requires imageLib and MRF

The three directories should be within one directory.
The MRF directory (e.g., MRF2.1) should be called 'MRF':
rename it ("mv MRF2.1 MRF") or create a symbolic link ("ln -s MRF2.1 MRF").

To compile:

$ cd MRF
$ make
$ cd ../imageLib
$ make
$ cd ../mrfstereo
$ make

To run:

$ ./mrfstereo

 mrfstereo version 1.0

 usage: ./mrfstereo [options] imL imR dispL

  reads imL and imR (in png or pgm/ppm format)
  runs MRF stereo
  writes dispL (in png or pgm/ppm format), disparities corresponding to imL

  options:
    -n nD          disparity levels, by default 16 (i.e. disparites 0..15)
    -b             use Birchfield/Tomasi costs
    -s             use squared differences (absolute differences by default)
    -t trunc       truncate differences to <= 'trunc'
    -a MRFalg      0-ICM, 1-Expansion (default), 2-Swap, 3-TRWS, 4-BPS, 5-BPM, 9-all
    -e smoothexp   smoothness exponent, 1 (default) or 2, i.e. L1 or L2 norm
    -m smoothmax   maximum value of smoothness term (2 by default)
    -l lambda      weight of smoothness term (20 by default)
    -g gradThresh  intensity gradient cue threshold
    -p gradPenalty if grad < gradThresh, multiply smoothness (2 by default)
    -o outscale    scale factor for disparities (full range by default)
    -w             write parameter settings to dispL.txt
    -x             write timings to dispL.csv
    -q             quiet (turn off debugging output)

Exit 1

$ ./mrfstereo tsukuba/tsukuba-im* tsukuba/tsukuba.png
Reading image tsukuba/tsukuba-imL.png
Reading image tsukuba/tsukuba-imR.png
Image size: 384 x 288, nDisp = 16
Data term: absolute differences
Smoothness term: L1 norm, truncated at 2, lambda=20
******* Running Expansion for up to 500 x 1 iterations
Energy = 6913378 (Ed=6913378, Es=0) at start
Energy = 1134339 (Ed=930659, Es=203680), 3.1 secs
Energy = 1128470 (Ed=927190, Es=201280), 5.5 secs
Energy = 1127957 (Ed=926877, Es=201080), 8.0 secs
Energy = 1127957 (Ed=926877, Es=201080), 10.5 secs
scaling disparities by 17
Writing image tsukuba/tsukuba.png
