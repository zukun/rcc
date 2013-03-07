#ifndef _pngio_h
#define _pngio_h

#include "png.h"

#define ERROR 0
#define OK 1

int read_png(char *file_name, int *w, int *h, unsigned char *pixout[], 
	     int *numchannel);
int read_png_gl(char *file_name, int *w, int *h, int *rw, int *rh, unsigned char *pixout[]);
int write_png(char *file_name, int width, int height, unsigned char pixels[], 
	      int numchannel);

int read_png_(char *file_name, int *w, int *h,  int *rw, int *rh, unsigned char *pixout[], bool forgl, int *numchannel);


#endif
