// denoise.cpp
// De-noising with quadratic data and smoothness terms
// added support for truncated smoothness term

static char *usage = "usage: %s lambda trunc in.png outfilenamestem [mask.png]\n";

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "imageLib.h"

#include "mrf.h"
#include "ICM.h"
#include "GCoptimization.h"
#include "MaxProdBP.h"
#include "TRW-S.h"
#include "BP-S.h"

#include "runalgs.h"

// count energy truncations in energy.h
#ifdef COUNT_TRUNCATIONS
int truncCnt, totalCnt;
#endif

int lambda = 1;
int width = 0;
int height = 0;
CByteImage img, maskimg;
int mask = 0;
int verbose = 1;
int trunc = 0; // no truncation


// data cost is squared distance to original value
MRF::CostVal dCostFn(int pix, int value)
{
    int x = pix % width;
    int y = pix / width;
    if (mask && maskimg.Pixel(x, y, 0) == 0)
	return 0;
    int diff = img.Pixel(x, y, 0) - value;
    return diff * diff;
}

MRF::CostVal *dataArray(int nLabels)
{
    MRF::CostVal *D = (MRF::CostVal *)malloc(width*height*nLabels*sizeof(MRF::CostVal));
    int n = 0;
    // cost is the array of type CostVal. The cost of pixel p and label l is
    // stored at cost[p*nLabels+l] where nLabels is the number of labels
    for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
	    for (int v = 0; v < nLabels; v++) {
		if (mask && maskimg.Pixel(x, y, 0) == 0)
		    D[n] = 0;
		else {
		    int diff = img.Pixel(x, y, 0) - v;
		    D[n] = diff * diff;
		}
		n++;
	    }
	}
    }
    return D;
}

int main(int argc, char **argv)
{
    try {
        // parse command line
        if (argc < 5 || argc > 6)
            throw CError(usage, argv[0]);
        int argn = 1;
	lambda = atoi(argv[argn++]);
	trunc = atoi(argv[argn++]);
        char *inFileName = argv[argn++];
	char *outstem = argv[argn++];
	char *maskFileName = NULL;
	if (argn < argc)
	    maskFileName = argv[argn++];

	int writeParams = 1;
	int writeTimings = 1;

	FILE *debugfile = createDebugFile(writeParams, outstem, verbose, argc, argv);

        ReadImageVerb(img, inFileName, verbose);
	CShape sh = img.Shape();
	width = sh.width;
	height = sh.height;
	if (sh.nBands != 1)
	    throw CError("Only gray images are supported");
	
	CByteImage outimg(sh);

	if (maskFileName) {
	    mask = 1;
	    ReadImageVerb(maskimg, maskFileName, verbose);
	}
	
	if (debugfile) {
	    fprintf(debugfile, "running denoising with lambda = %d, ", lambda);
	    if (trunc == 0)
		fprintf(debugfile, "quadratic smoothness (no truncation)\n");
	    else
		fprintf(debugfile, "quadratic smoothness truncated at %d\n", trunc);
	}

	int nLabels = 256;
	if (trunc == 0)
	    trunc = nLabels * nLabels; // no truncation

	MRF::CostVal* D = dataArray(nLabels);
	EnergyFunction *energy = new EnergyFunction(new DataCost(D),
						    new SmoothnessCost(2, trunc, lambda));

	// can also use general data functions, but BP-M can't handle this, plus much slower
	//EnergyFunction *energy = new EnergyFunction(new DataCost(dCostFn),
	//	    				      new SmoothnessCost(2, trunc, lambda));

	int MRFalg = aRunAll;

	int outerIter, innerIter;
	MRF *mrf = NULL;
	for (int numAlg = aICM; numAlg <= aBPM; numAlg++) {
	    outerIter = MAXITER;
	    innerIter = 1;
	    if (MRFalg < aRunAll && numAlg != MRFalg) continue;

#ifdef COUNT_TRUNCATIONS
	// only re-run Expansion and Swap
	    if (!(numAlg == aExpansion || numAlg == aSwap)) continue;
#endif

	    startAlgInitTimer();

	    switch (numAlg) {
	    case aICM:       mrf = new ICM(width, height, nLabels, energy); innerIter = 5; break;
	    case aExpansion: mrf = new Expansion(width, height, nLabels, energy); break;
	    case aSwap:      mrf = new Swap(width, height, nLabels, energy); break;
	    case aTRWS:      mrf = new TRWS(width, height, nLabels, energy); break;
	    case aBPS:       mrf = new BPS(width, height, nLabels, energy); break;
	    case aBPM:       mrf = new MaxProdBP(width, height, nLabels, energy); 
		//outerIter = MAXITER/2; 
		break;
	    default: throw new CError("unknown algorithm number");
	    }
	    if (debugfile)
		fprintf(debugfile, "******* Running %s for up to %d x %d iterations\n",
			algs[numAlg], outerIter, innerIter);

	    mrf->initialize();
	    mrf->clearAnswer();

	    // initialize labels to original intensity values
	    int n = 0;
	    for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
		    int label = (mask && maskimg.Pixel(x, y, 0) == 0) ? 0 // in holes
			: img.Pixel(x, y, 0);
		    mrf->setLabel(n, label);
		    outimg.Pixel(x, y, 0) = label;
		    n++;
		}
	    }

	    float initTime = getAlgInitTime();
	    
	    FILE *timefile = createTimeFile(writeTimings, outstem, algs[numAlg], debugfile);

#ifdef COUNT_TRUNCATIONS
    truncCnt = totalCnt = 0;
#endif

	    runAlg(mrf, numAlg, debugfile, timefile, outerIter, innerIter, initTime);

#ifdef COUNT_TRUNCATIONS
	if (truncCnt > 0)
	    if (debugfile)
		fprintf(debugfile, "***WARNING: %d terms (%.4f%%) were truncated to ensure regularity\n", 
			truncCnt, (float)(100.0 * truncCnt / totalCnt));
#endif

	    // save resulting labels as image
	    n = 0;
	    for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
		    outimg.Pixel(x, y, 0) = mrf->getLabel(n);
		    n++;
		}
	    }

	    char fname[500];
	    sprintf(fname, "%s-%s.png", outstem, algs[numAlg]);
	    WriteImageVerb(outimg, fname, 1);
	    delete mrf;
	}
	if (writeParams)
	    fclose(debugfile);
	delete D;
	delete energy;
    }
    catch (CError &err) {
        fprintf(stderr, err.message);
        fprintf(stderr, "\n");
        return -1;
    }
    catch (bad_alloc) {
	fprintf(stderr, "*** Error: not enough memory\n");
	exit(1);
    }

    return 0;
}
