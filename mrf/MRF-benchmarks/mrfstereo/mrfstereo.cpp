/* mrfstereo.cpp
 *
 * simple MRF stereo matcher
 * uses MRF library
 *
 * Daniel Scharstein
 * 9/20/2006 initial version
 *
 * 10/4/2006 version 1.0 - reasonable complete version used to generate
 * first set of results at http://vision.middlebury.edu/MRF/new/.
 *
 * 10/11/2006 moved the code that runs the algorithms and creates the files to runalgs.h/.cpp
 * 3/19/2007 added code to count energy truncations (see ../MRF2.0/energy.h)
 */

static char usage[] = "\n mrfstereo version 1.0 \n\
\n\
 usage: %s [options] imL imR dispL \n\
\n\
  reads imL and imR (in png or pgm/ppm format)\n\
  runs MRF stereo\n\
  writes dispL (in png or pgm/ppm format), disparities corresponding to imL\n\
\n\
  options:\n\
    -n nD          disparity levels, by default 16 (i.e. disparites 0..15)\n\
    -b             use Birchfield/Tomasi costs\n\
    -s             use squared differences (absolute differences by default)\n\
    -t trunc       truncate differences to <= 'trunc'\n\
    -a MRFalg      0-ICM, 1-Expansion (default), 2-Swap, 3-TRWS, 4-BPS, 5-BPM, 9-all\n\
    -e smoothexp   smoothness exponent, 1 (default) or 2, i.e. L1 or L2 norm\n\
    -m smoothmax   maximum value of smoothness term (2 by default)\n\
    -l lambda      weight of smoothness term (20 by default)\n\
    -g gradThresh  intensity gradient cue threshold\n\
    -p gradPenalty if grad < gradThresh, multiply smoothness (2 by default)\n\
    -o outscale    scale factor for disparities (full range by default)\n\
    -w             write parameter settings to dispL.txt\n\
    -x             write timings to dispL.csv\n\
    -q             quiet (turn off debugging output)\n\n";

#include <stdio.h>
#ifdef _MSC_VER        // Visual C++
# include "XGetopt.h"  // replacement for getopt()
#else
# include <getopt.h>
#endif
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

// global variables for debugging output
int verbose;

// prototypes
void computeDSI(CByteImage im1, CByteImage im2, MRF::CostVal *&dsi, int nD, 
		int birchfield, int squaredDiffs, int truncDiffs);
void computeCues(CByteImage im1, MRF::CostVal *&hCue, MRF::CostVal *&vCue,
		 int gradThresh, int gradPenalty);
void WTA(MRF::CostVal *dsi, int width, int height, int nD, CByteImage &disp);
void getDisparities(MRF *mrf, int width, int height, CByteImage &disp);
void setDisparities(CByteImage disp, MRF *mrf);
void writeDisparities(CByteImage disp, int outscale, char *dispname, FILE *debugfile);
    
int main(int argc, char **argv)
{
    // parameters controlled via command-line options:
    int nD = 16;           // disparity levels (d = 0 .. nD-1)
    int birchfield = 0;    // use Birchfield/Tomasi costs
    int squaredDiffs = 0;  // use squared differences (absolute differences by default)
    int truncDiffs = 255;  // truncated differences (before squaring), by default not
    int MRFalg = 1;        // 0-ICM, 1-GC/expansion (default), 2-GC/swap, 3-TRWS, 4-BPS, 5-BPM, 6-all
    int smoothexp = 1;     // exponent of smoothness term: 1 (default) or 2, i.e. L1 or L2 norm
    int smoothmax = 2;     // maximum value of smoothness term (2 by default)
    int lambda = 20;       // weight of smoothness term (20 by default)
    int gradThresh = -1;   // intensity gradient cue threshold, by default none
    int gradPenalty = 2;   // if grad < gradThresh, multiply smoothness cost by this
    int outscale = -1;     // scale factor for disparities; -1 means full range 255.0/(nD-1)
    int writeParams = 0;       // write parameter settings to dispL.txt
    int writeTimings = 0;  // write timings to dispL.csv
    verbose = 1;           // print messages to stderr

    // parse command-line arguments using the "getopt" utility
    int o;
    while ((o = getopt(argc, argv, "n:bst:a:e:m:l:g:p:o:wxq")) != -1)
	switch (o) {
	case 'n': nD = atoi(optarg); break;
	case 'b': birchfield = 1; break;
	case 's': squaredDiffs = 1; break;
	case 't': truncDiffs = atoi(optarg); break;
	case 'a': MRFalg = atoi(optarg); break;
	case 'e': smoothexp = atoi(optarg); break;
	case 'm': smoothmax = atoi(optarg); break;
	case 'l': lambda = atoi(optarg); break;
	case 'g': gradThresh = atoi(optarg); break;
	case 'p': gradPenalty = atoi(optarg); break;
	case 'o': outscale = atoi(optarg); break;
	case 'w': writeParams = 1; break;
	case 'x': writeTimings = 1; break;
	case 'q': verbose = 0; break;
	default: 
	    fprintf(stderr, "Ignoring unrecognized option %s\n", argv[optind-1]);
	}

    if (optind != argc-3) {
	fprintf(stderr, usage, argv[0]);
	exit(1);
    }

    char *im1name = argv[optind++];
    char *im2name = argv[optind++];
    char *dispname = argv[optind++];

    if (outscale < 0)
	outscale = 255 / (nD-1);

    try {
	char outstem[1000];
	strcpy(outstem, dispname);
	char *dot = strrchr(outstem, '.');
	if (dot)
	    *dot = '\0'; // terminate string at dot; if no dot use whole name

	FILE *debugfile = createDebugFile(writeParams, outstem, verbose, argc, argv);

	CByteImage im1, im2;      // input images (gray or color)
	CByteImage disp;          // output disparities
	ReadImageVerb(im1, im1name, verbose);
	ReadImageVerb(im2, im2name, verbose);
  
	CShape sh = im1.Shape();
	CShape sh2 = im2.Shape();
	if (sh != sh2)
	    throw CError("image shapes don't match");
	int width = sh.width, height = sh.height;

	if (debugfile) {
	    fprintf(debugfile, "Image size: %d x %d, nDisp = %d\n", width, height, nD);
	    fprintf(debugfile, "Data term: %s differences ", squaredDiffs ? "squared" : "absolute");
	    if (truncDiffs < 255)
		fprintf(debugfile, "truncated at %d", truncDiffs);
	    fprintf(debugfile, "%s\n", birchfield ? " (Birchfield/Tomasi)" : " ");
	}

	MRF::CostVal *dsi = NULL;
	computeDSI(im1, im2, dsi, nD, birchfield, squaredDiffs, truncDiffs);
	DataCost *dcost = new DataCost(dsi);

	if (debugfile) {
	    fprintf(debugfile, "Smoothness term: L%d norm, truncated at %d, lambda=%d\n",
		    smoothexp, smoothmax, lambda);
	    if (gradThresh > 0)
		fprintf(debugfile, "using intensity cues: Penalty %d if gradient < %d\n",
			gradPenalty, gradThresh);
	}

	SmoothnessCost *scost;
	MRF::CostVal *hCue = NULL, *vCue = NULL;
	if (gradThresh > 0) {
	    computeCues(im1, hCue, vCue, gradThresh, gradPenalty);
	    scost = new SmoothnessCost(smoothexp, smoothmax, lambda, hCue, vCue);
	} else {
	    scost = new SmoothnessCost(smoothexp, smoothmax, lambda);
	}
	EnergyFunction *energy = new EnergyFunction(dcost, scost);

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
	    case aICM:       mrf = new ICM(width, height, nD, energy); innerIter = 5; break;
	    case aExpansion: mrf = new Expansion(width, height, nD, energy); break;
	    case aSwap:      mrf = new Swap(width, height, nD, energy); break;
	    case aTRWS:      mrf = new TRWS(width, height, nD, energy); break;
	    case aBPS:       mrf = new BPS(width, height, nD, energy); break;
	    case aBPM:       mrf = new MaxProdBP(width, height, nD, energy); 
		//outerIter = MAXITER/2; 
		break;
	    default: throw new CError("unknown algorithm number");
	    }
	    if (debugfile)
		fprintf(debugfile, "******* Running %s for up to %d x %d iterations\n",
			algs[numAlg], outerIter, innerIter);

	    mrf->initialize();

	    bool initializeToWTA = (numAlg == aICM); 
	    if (initializeToWTA) {
		if (debugfile)
		    fprintf(debugfile, "performing WTA\n");
		WTA(dsi, width, height, nD, disp);
		//writeDisparities(disp, outscale, "WTA.png", debugfile);
		setDisparities(disp, mrf);
	    } else {
		mrf->clearAnswer();
	    }

	    if (numAlg == aExpansion || numAlg == aSwap) {
		// don't randomize label order (so that we get the same result each time)
		bool b[1] = {0};
		mrf->setParameters(1, &b);
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

	    getDisparities(mrf, width, height, disp);
	    
	    if (MRFalg < aRunAll) { // run single algorithm, use file name specified
	    	writeDisparities(disp, outscale, dispname, debugfile);
	    } else { // run all algorithms, so add alg name to filename
		char dispalgname[1000];
		sprintf(dispalgname, "%s-%s.png", outstem, algs[numAlg]);
	    	writeDisparities(disp, outscale, dispalgname, debugfile);
	    }

	    delete mrf;
	}

	if (writeParams)
	    fclose(debugfile);

	delete energy;
	delete scost;
	delete dcost;
	delete [] dsi;
	delete [] hCue;
	delete [] vCue;
    }
    catch (CError &err) {
	fprintf(stderr, err.message);
	fprintf(stderr, "\n");
	exit(1);
    }
    catch (bad_alloc) {
	fprintf(stderr, "*** Error: not enough memory\n");
	exit(1);
    }

    return 0;
}


// DSI - cost volume (absolute color diffs)

void computeDSI(CByteImage im1,       // source (reference) image
		CByteImage im2,       // destination (match) image
		MRF::CostVal *&dsi,   // computed cost volume
		int nD,               // number of disparities
		int birchfield,       // use Birchfield/Tomasi costs
		int squaredDiffs,     // use squared differences
		int truncDiffs)       // truncated differences
{
    CShape sh = im1.Shape();
    int width = sh.width, height = sh.height, nB = sh.nBands;
    dsi = new MRF::CostVal[width * height * nD];

    int nColors = __min(3, nB);

    // worst value for sumdiff below 
    int worst_match = nColors * (squaredDiffs ? 255 * 255 : 255);
    // truncation threshold - NOTE: if squared, don't multiply by nColors (Eucl. dist.)
    int maxsumdiff = squaredDiffs ? truncDiffs * truncDiffs : nColors * abs(truncDiffs);
    // value for out-of-bounds matches
    int badcost = __min(worst_match, maxsumdiff);

    int dsiIndex = 0;
    for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
	    uchar *pix1 = &im1.Pixel(x, y, 0);
	    for (int d = 0; d < nD; d++) {
		int x2 = x-d;
		int dsiValue;
                if (x2 >= 0 && d < nD) { // in bounds
		    uchar *pix2 = &im2.Pixel(x2, y, 0);
                    int sumdiff = 0;
                    for (int b = 0; b < nColors; b++) {
			int diff = 0;
			if (birchfield) {
			    // Birchfield/Tomasi cost
			    int im1c = pix1[b];
			    int im1l = x == 0?   im1c : (im1c + pix1[b - nB]) / 2;
			    int im1r = x == width-1? im1c : (im1c + pix1[b + nB]) / 2;
			    int im2c = pix2[b];
			    int im2l = x2 == 0?   im2c : (im2c + pix2[b - nB]) / 2;
			    int im2r = x2 == width-1? im2c : (im2c + pix2[b + nB]) / 2;
			    int min1 = __min(im1c, __min(im1l, im1r));
			    int max1 = __max(im1c, __max(im1l, im1r));
			    int min2 = __min(im2c, __min(im2l, im2r));
			    int max2 = __max(im2c, __max(im2l, im2r));
			    int di1 = __max(0, __max(im1c - max2, min2 - im1c));
			    int di2 = __max(0, __max(im2c - max1, min1 - im2c));
			    diff = __min(di1, di2);
			} else {
			    // simple absolute difference
			    int di = pix1[b] - pix2[b];
			    diff = abs(di);
			}
			// square diffs if requested (Birchfield too...)
			sumdiff += (squaredDiffs ? diff * diff : diff);
                    }
		    // truncate diffs
		    dsiValue = __min(sumdiff, maxsumdiff);
                } else { // out of bounds: use maximum truncated cost
		    dsiValue = badcost;
		}
		//int x0=-140, y0=-150;
		//if (x==x0 && y==y0)
		//    printf("dsi(%d,%d,%2d)=%3d\n", x, y, d, dsiValue); 

		// The cost of pixel p and label l is stored at dsi[p*nLabels+l]
		dsi[dsiIndex++] = dsiValue;
	    }
	}
    }
    //exit(1);
}

void computeCues(CByteImage im1, MRF::CostVal *&hCue, MRF::CostVal *&vCue,
		 int gradThresh, int gradPenalty) {
    CShape sh = im1.Shape();
    int width = sh.width, height = sh.height, nB = sh.nBands;
    hCue = new MRF::CostVal[width * height];
    vCue = new MRF::CostVal[width * height];

    int nColors = __min(3, nB);

    // below we compute sum of squared colordiffs, so need to adjust threshold accordingly (results in RMS)
    gradThresh *= nColors * gradThresh;

    //sh.nBands=1;
    //CByteImage hc(sh), vc(sh);
    int n = 0;
    for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
	    uchar *pix   = &im1.Pixel(x, y, 0);
	    uchar *pix1x = &im1.Pixel(x + (x < width-1), y, 0);
	    uchar *pix1y = &im1.Pixel(x, y + (y < height-1), 0);
	    int sx = 0, sy = 0;
	    for (int b = 0; b < nColors; b++) {
		int dx = pix[b] - pix1x[b];
		int dy = pix[b] - pix1y[b];
		sx += dx * dx;
		sy += dy * dy;
	    }
	    hCue[n] = (sx < gradThresh ? gradPenalty : 1);
	    vCue[n] = (sy < gradThresh ? gradPenalty : 1);
	    //hc.Pixel(x, y, 0) = 100*hCue[n];
	    //vc.Pixel(x, y, 0) = 100*vCue[n];
	    n++;
	}
    }
    //WriteImageVerb(hc, "hcue.png", true);
    //WriteImageVerb(vc, "vcue.png", true);
    //exit(1);
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

void getDisparities(MRF *mrf, int width, int height, CByteImage &disp)
{
    CShape sh(width, height, 1);
    disp.ReAllocate(sh);

    int n = 0;
    for (int y = 0; y < height; y++) {
	uchar *row = &disp.Pixel(0, y, 0);
	for (int x = 0; x < width; x++) {
	    row[x] = mrf->getLabel(n++);
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
