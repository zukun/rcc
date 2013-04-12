This directory contains all the code to run all the MRF benchmarks.

The code is provided "as is" as a service to the community.
It comes with NO WARRANTIES WHATSOEVER, and we cannot provide
any help on how to use it beyond the documentation included.

Subdirectories:

MRF2.1/ - the MRF library

imageLib/ - the image library

binary/ - the binary image segmentation benchmarks, with results in:
binary/results_flower/
binary/results_person/
binary/results_sponge/

denoise/ - the denoising and inpainting benchmarks, with results in:
denoise/results_house/
denoise/results_penguin/

mrfstereo/ - the stereo matching benchmarks, with results in:
mrfstereo/results_teddy/
mrfstereo/results_tsukuba/
mrfstereo/results_venus/

photomontage/ - the Photomontage benchmarks, with results in:
photomontage/results_family/
photomontage/results_pano/


To compile everything, type "make".
Note: the current version of the code does not compile on 64-bit machines.

To run all the benchmarks, type "make run".

See the Makefile for other commands.

Note: Running the benchmarks will take a long time (many hours).  It
will populate the results_xxx subdirectories, which are initially
empty.

The code that runs all the different minimization algorithms is in
runalgs.cpp and runalgs.h, which are in mrfstereo/ (but the other
three benchmarks use these files too).  In particular, runalgs.h
contains the line
   #define MAXITER 500
which controls the maximum number of iterations.  Note that for the
results shown on the webpage we used higher numbers of iterations
(usually 900, but in some cases, like photomontage, up to 2000).

