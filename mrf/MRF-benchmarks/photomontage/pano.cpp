// pano.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string>
#include <assert.h>
#include <vector>

// arguments
// n file1.png ... filen.png 
// n       : number of source images
// source images

#include "pngio.h"
#include "image.h"
#include "seamFinder.h"
//#include "blend.h"

// Read globals.h comment to explain this ugliness of global variables
int g_width, g_height;
std::vector<Image*> g_sources;

int main(int argc, char* argv[])
{
    if (argc < 3) exit(1);

	// Load everything up

	int n = atoi(argv[1]);
	assert(n > 1 && n<500);
	char** sourceNames = new char*[n];
	//std::vector<Image*> sources; // = new Image[n];
	std::vector<Image*>& sources = g_sources; 
	//CType** sources = new unsigned char*[n];
	int w=-1, h=-1, i;
	for (i=0; i<n; ++i) {
	  CType *ptr=NULL;
	  sourceNames[i] = new char[100];
	  strcpy(sourceNames[i], argv[2+i]);
	  int neww, newh, numc;
	  read_png(sourceNames[i], &neww, &newh, &ptr, &numc);
	  if (w==-1) {
	    w = neww; h = newh;
	    g_width = w; g_height = h;
	  }
	  else 
	    assert(neww == w && newh == h);
	  assert(numc==_NUMC_); 
	  sources.push_back(new Image(ptr,neww,newh));
	  //sources[i].allocate(ptr, newh, newh);
	}

	printf("%d sources\n",n);
	printf("w: %d, h: %d\n",w,h);

	// Create necessary data	
	//for (i=0; i<n; ++i) {
	//  sources[i]->createAuxData();
	//}

	// Create, run seam-finder
	SeamFinder sf(sources);
	printf("Running study\n");
	sf.runStudy("pano", argc, argv);

	/*Image* outIm = new Image(w,h);
	sf.comp(outIm);
	//outIm->write(outname);
	write_png(outname, w, h, outIm->data(), _NUMC_);
	*/

	// Create, run blender
	//CType* final3 = new CType[3*w*h];

	/*std::vector<bool> trans(w*h);
	int ind=0;
	for (j=0; j<h; ++j)
	  for (i=0; i<w; ++i,++ind)
	    trans[ind] = outIm->trans(i,j);
	
	GlobalBlender gb(w,h,sources, sf.labels(), outIm, trans);
	gb.compute();
	write_png(outname, w, h, outIm->data(), _NUMC_);
	*/
	return 0;
}


