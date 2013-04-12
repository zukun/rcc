// binary.cpp
// Binary segmentation problems contributed by Carsten
// modified to use MRF API and imageLib by Daniel Scharstein

static char *usage = "usage: %s datafile outfilenamestem\n";

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <vector>
#include <fstream>
#include <iostream>

#include "imageLib.h"

#include "mrf.h"
#include "ICM.h"
#include "GCoptimization.h"
#include "MaxProdBP.h"
#include "TRW-S.h"
#include "BP-S.h"

#include "runalgs.h"

int verbose = 1;

void WTA(MRF::CostVal *dsi, int width, int height, int nD, CByteImage &disp);
void setDisparities(CByteImage disp, MRF *mrf);
void writeDisparities(CByteImage disp, int outscale, char *dispname, FILE *debugfile);
void writeDataCosts(MRF::CostVal *dataCostArray, int width, int height, int nLabels);

void LoadDataFile(char *filename, int &width, int &height, int &nLabels, 
		  MRF::CostVal *&dataCostArray,
		  MRF::CostVal *&hCue, 
		  MRF::CostVal *&vCue)
{
    // Load energies

    std::ifstream in;
    in.open(filename);

    // Load size and num labels
    in >> width >> height >> nLabels;

    //if (nLabels != 2)
    //	throw CError("Only binary problem supported");

    int i, n, x, y;

    // ignore gt labels
    int gt;
    for(i = 0; i < width * height; i++)
	in >> gt;

    // Load unary potentials (data costs)
    dataCostArray = new MRF::CostVal[width * height * nLabels];
    for(int c=0; c < nLabels; c++) {
	n = c;
	for(i = 0; i < width * height; i++) {
	    in >> dataCostArray[n];
	    n += nLabels;
	}
    }

    writeDataCosts(dataCostArray, width, height, nLabels);

    // Load pairwise potentials
    hCue = new MRF::CostVal[width * height];
    vCue = new MRF::CostVal[width * height];

    // L-R
    n = 0;
    for(y = 0; y < height; y++) {
	for(x = 0; x < width-1; x++) {
	    in >> hCue[n++];
	}
	// last column:
	hCue[n++] = 0;
    }

    // U-D
    n = 0;
    for(y = 0; y < height-1; y++) {
	for(x = 0; x < width; x++) {
            in >> vCue[n++];
	}
    }
    // last row:
    for(x = 0; x < width; x++) {
	vCue[n++] = 0;
    }

    in.close();
}

int main(int argc, char **argv)
{
    try {
        // parse command line
        if (argc != 3)
            throw CError(usage, argv[0]);
        int argn = 1;
        char *dataFileName = argv[argn++];
	char *outstem = argv[argn++];

	int writeParams = 1;
	int writeTimings = 1;

	FILE *debugfile = createDebugFile(writeParams, outstem, verbose, argc, argv);

	// Load datafile 
	int width, height, nLabels;
	std::vector<int> gt, data, lrPairwise, udPairwise;
	MRF::CostVal *dataCostArray, *hCue, *vCue;

	if (verbose)
	    fprintf(stderr, "Loading datafile...\n");
	
	LoadDataFile(dataFileName, width, height, nLabels, dataCostArray, hCue, vCue);

	DataCost *dcost = new DataCost(dataCostArray);
	SmoothnessCost *scost = new SmoothnessCost(1, 1, 1, hCue, vCue);
	EnergyFunction *energy = new EnergyFunction(dcost, scost);

	if (verbose)
	    fprintf(stderr, "Running optimization...\n");
	fflush(stderr);

	int MRFalg = aRunAll;

	int outerIter, innerIter;
	MRF *mrf = NULL;
	for (int numAlg = aICM; numAlg <= aBPM; numAlg++) {
	    outerIter = MAXITER;
	    innerIter = 1;
	    if (MRFalg < aRunAll && numAlg != MRFalg) continue;

	    startAlgInitTimer();

	    switch (numAlg) {
	    case aICM:       mrf = new ICM(width, height, nLabels, energy); innerIter = 5; break;
	    case aExpansion: mrf = new Expansion(width, height, nLabels, energy); break;
	    case aSwap:      mrf = new Swap(width, height, nLabels, energy); break;
	    case aTRWS:      mrf = new TRWS(width, height, nLabels, energy); break;
	    case aBPS:       mrf = new BPS(width, height, nLabels, energy);  
		//innerIter = 5; 
		break;
	    case aBPM:       mrf = new MaxProdBP(width, height, nLabels, energy);
		//innerIter = 2; 
		break;
	    default: throw new CError("unknown algorithm number");
	    }
	    if (debugfile)
		fprintf(debugfile, "******* Running %s for up to %d x %d iterations\n",
			algs[numAlg], outerIter, innerIter);

	    mrf->initialize();
	    mrf->clearAnswer();

	    bool initializeToWTA = false;
	    if (initializeToWTA) {
		if (debugfile)
		    fprintf(debugfile, "performing WTA\n");
		CByteImage disp;
		WTA(dataCostArray, width, height, nLabels, disp);
		writeDisparities(disp, 255, "WTA.png", debugfile);
		setDisparities(disp, mrf);
	    } else {
		mrf->clearAnswer();
	    }

	    float initTime = getAlgInitTime();
	    
	    FILE *timefile = createTimeFile(writeTimings, outstem, algs[numAlg], debugfile);

	    runAlg(mrf, numAlg, debugfile, timefile, outerIter, innerIter, initTime);

	    // save resulting labels as image
	    CShape sh(width, height, 1);
	    CByteImage outimg(sh);
	    int n = 0;
	    for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
		    outimg.Pixel(x, y, 0) = 255* mrf->getLabel(n);
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

	delete energy;
	delete scost;
	delete dcost;
	delete [] dataCostArray;
	delete [] hCue;
	delete [] vCue;
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

void WTA(MRF::CostVal *dsi, int width, int height, int nD, CByteImage &disp)
{
    CShape sh(width, height, 1);
    disp.ReAllocate(sh);
    int n = 0;
    for (int y = 0; y < height; y++) {
	uchar *row = &disp.Pixel(0, y, 0);
	for (int x = 0; x < width; x++) {
	    int minval = dsi[n++]; // dsi(x,y,0)
	    int mind = 0;
	    for (int d = 1; d < nD; d++) {
		int val = dsi[n++]; // dsi(x,y,d)
		if (val < minval) {
		    minval = val;
		    mind = d;
		}
	    }
	    row[x] = mind;
	}
    }
}

void setDisparities(CByteImage disp, MRF *mrf)
{
    CShape sh = disp.Shape();
    int width = sh.width, height = sh.height;

    int n = 0;
    for (int y = 0; y < height; y++) {
	uchar *row = &disp.Pixel(0, y, 0);
	for (int x = 0; x < width; x++) {
	    mrf->setLabel(n++, row[x]);
	}
    }
}

void writeDisparities(CByteImage disp, int outscale, char *dispname, FILE *debugfile)
{
    if (debugfile)
	fprintf(debugfile, "scaling disparities by %d\n", outscale);

    CByteImage disp2;
    ScaleAndOffset(disp, disp2, (float)outscale, 0);
    
    if (debugfile)
	fprintf(debugfile, "writing file %s\n", dispname);

    WriteImageVerb(disp2, dispname, verbose);
}

void writeDataCosts(MRF::CostVal *dataCostArray, int width, int height, int nLabels)
{
    CShape sh(width, height, 1);
    CIntImage im(sh);
    CByteImage im2(sh);

    for (int l = 0; l < nLabels; l++) {
	int n = l;
	int vmax = 0;
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		int v = dataCostArray[n];
		n += nLabels;
		if (v > vmax)
		    vmax = v;
		im.Pixel(x, y, 0) = v;
	    }
	}
	ScaleAndOffset(im, im2, (float)(255.0/vmax), 0);
	char fname[1000];
	sprintf(fname, "datacost%d.png", l);
	WriteImageVerb(im2, fname, 1);
    }
}
