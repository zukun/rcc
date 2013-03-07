#include "blendFuncs.h"
using namespace std;


namespace Adrien {

  void blend(int w, int h, ushort* labeling, std::vector<ImageAbs*> * imgs, unsigned char* final, Areal edgeThres, FILE* fp) {
    // Make sure the labeling is zero (backround) along border.
    ushort *lab = new ushort[w*h];
    enforceLabelingContraint(w, h, labeling, lab);
  
    // Allocate some space into which we'll do our computations.
    unsigned char *finlChannels[] = {(unsigned char *) malloc(w * h),   // starts as base, gets new features blended in
				     (unsigned char *) malloc(w * h),
				     (unsigned char *) malloc(w * h)};
    unsigned char *imagChannels[] = {(unsigned char *) malloc(w * h),   // takes offset new feature image
				     (unsigned char *) malloc(w * h),
				     (unsigned char *) malloc(w * h)};
    unsigned char *offsetImg = (unsigned char *) malloc(w* h * 3);

    // Offset & Decompose the background image into its color channels.
    // This forms basis for our result.
    offsetImageRGB(w,h, (*imgs)[0], offsetImg);
    channelDecompose(w,h,offsetImg, finlChannels[0], finlChannels[1], finlChannels[2]); 

    // now blend in each feature, in order
    for (unsigned int feature = 1 ; feature < imgs->size() ; feature++) {

      // Create the matrix for this blend;
      PoissonBlender blendMatrix(w, h, feature, lab, edgeThres);
      if (blendMatrix.getNumPixels() == 0) {
	cout << "WARNING: Feature " << feature << " has no values in labeling." << endl;
	//fprintf(fp, "Feature %d has no values in labeling.\n",feature);
	continue;
      }

      // offset the image
      offsetImageRGB(w, h, (*imgs)[feature], offsetImg);

      // Decompose this feature image into its color channels
      channelDecompose(w, h, offsetImg, imagChannels[0], imagChannels[1], imagChannels[2]);


      // now blend in, one channel at a time
      //for (int chnl = 0 ; chnl < 3 ; chnl++)
      blendMatrix.blendChannels(imagChannels, finlChannels, fp);
    }

    // now put the whole thing back together
    channelCombine(w, h, finlChannels[0], finlChannels[1], finlChannels[2], final);

    // free all allocated memory
    delete[] lab;
    free(finlChannels[0]);
    free(finlChannels[1]);
    free(finlChannels[2]);
    free(imagChannels[0]);
    free(imagChannels[1]);
    free(imagChannels[2]);
    free(offsetImg);
  }


  int imgIndex(int x, int y, int w) {
    return y * w + x;
  }

  int imgIndexRGB(int x, int y, int w) {
    return imgIndex(x, y, w) * 3;
  }


  void channelDecompose(int w, int h, unsigned char *img, unsigned char *r, unsigned char *g, unsigned char *b) {
    int n = w * h;
    for (int i = 0 ; i < n ; i++) {
      r[i] = img[3 * i + 0];
      g[i] = img[3 * i + 1];
      b[i] = img[3 * i + 2];
    }
  }

  /*
    void channelDecompose(ImageAbs* im, unsigned char *r, unsigned char *g, unsigned char *b) {
    int n = im->_size.x * im->_size.y;
    for (int i = 0 ; i < n ; i++) {
    r[i] = im->_data[3 * i + 0];
    g[i] = im->_data[3 * i + 1];
    b[i] = im->_data[3 * i + 2];
    }
    }
  */
  void channelCombine(int w, int h, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *img) {
    int n = w * h;
    for (int i = 0 ; i < n ; i++) {
      img[3 * i + 0] = r[i]; 
      img[3 * i + 1] = g[i]; 
      img[3 * i + 2] = b[i]; 
    }
  }

  // Make sure the labeling is zero (backround) along border.
  void enforceLabelingContraint(int w, int h, ushort *oldLabeling, ushort *result) {
    memcpy(result, oldLabeling, sizeof(ushort)*w * h);
    int i;
    for (i = 0 ; i < w ; i++)
      result[imgIndex(i, 0, w)] = result[imgIndex(i, h - 1, w)] = 0;
    for (i = 0 ; i < h ; i++)
      result[imgIndex(0, i, w)] = result[imgIndex(w - 1, i, w)] = 0;
  }

  // pushes the image by coord, and fills black
  void offsetImageRGB(int w, int h, ImageAbs* in, unsigned char *out) {
    for (int x = 0 ; x < w ; x++) {
      for (int y = 0 ; y < h ; y++) {
	Coord prime(x,y);
	in->displace(prime);
	int xPrime = prime.x, yPrime = prime.y;
	//int xPrime = x - in->displacement().x;
	//int yPrime = y - in->displacement().y;
	if ((xPrime < 0) || (xPrime >= in->_size.x) ||
	    (yPrime < 0) || (yPrime >= in->_size.y)) {
	  out[imgIndexRGB(x, y, w) + 0] = 0;
	  out[imgIndexRGB(x, y, w) + 1] = 0;
	  out[imgIndexRGB(x, y, w) + 2] = 0;
	} else {
	  out[imgIndexRGB(x, y, w) + 0] = in->data()[imgIndexRGB(xPrime, yPrime, in->_size.x) + 0];
	  out[imgIndexRGB(x, y, w) + 1] = in->data()[imgIndexRGB(xPrime, yPrime, in->_size.x) + 1];
	  out[imgIndexRGB(x, y, w) + 2] = in->data()[imgIndexRGB(xPrime, yPrime, in->_size.x) + 2];
	}
      }
    }
  }






}
