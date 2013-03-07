#ifndef BLEND_H
#define BLEND_H

#include <vector>
#include <string>
#include <stdio.h>

#include "../ImageAbs.h"
#include "../coord.h"
#include "../linearSolver.h"
#include "blendFuncs.h"

#include <wx/wx.h>
#include <wx/thread.h>

namespace Adrien {

  
  enum LinkType { L_NORMAL, L_BLEND, L_IGNORE};
  struct PixLinkTypes {
    LinkType r,b; // for right, bottom
  };

  class GlobalBlender : public implicitMatrix {
  public:
    GlobalBlender(int w, int h, std::vector<ImageAbs*> * imgs, unsigned char* final, 
		  Areal edgeThres, FILE* fp);

    virtual ~GlobalBlender();

    //thread stuff
    //ExitCode Entry () { compute(); return 0;}
    void myStop() { _myStop = true;}
    void takeComputeData(bool reInit, const ushort* newLabels, Areal edgeThres);

    //void compute(bool reInit, const ushort* newLabels, Areal edgeThres);
    void compute();

    void matVecMult(const Areal x[], Areal r[]) const;

    void pinPixel (const int x, const int y) { _pixelPin = y*_w+x; }

    void updateFinal(); // puts current result in final
    void copyOutFinal(unsigned char *out); // puts result(final) int out 
    bool doneComputing() { return (_newLabels == NULL); }
  private:
    //wxMutex _xMutex, _resultMutex; 

    //////// Variable abstractions, pixels ignore color
    
    int numPixels() const { return _wh; }
    int numVariables() const { return _wh-1; }
    //    int pixelToVar(const int index) const { if (index==0) return -1; else return index-1;}
    int pixelToVar (const int index) const { 
      if (index > _pixelPin) 
	return index-1; 
      else if (index == _pixelPin) 
	return -1; 
      else 
	return index; 
    }
    int pixelToVar(const int x, const int y) const { 
      return pixelToVar(y*_w + x); }
    //    int varToPixelIndex(const int v) const { return v+1; }
    int varToPixelIndex (const int v) const { 
      if (v == -1)
	return _pixelPin;
      else if (v >= _pixelPin)
	return v+1;
      else
	return v;
    }
    //    Coord varToPixelCoord(const int v) const { return Coord((v+1) % _w, (v+1) / _w); }      
    Coord varToPixelCoord (const int v) const {
      int pix = v;
      if (v == -1)
	pix = _pixelPin;
      else if (v > _pixelPin)
	pix = v+1;
      return Coord((v+1) % _w, (v+1) / _w);
    }
    //    int nonVarPixel() const { return 0; }
    int nonVarPixel () const { return _pixelPin; }

    int _pixelPin;
    //////// PROCEDURES

    void computeEdgeMap(const ushort* labels);

    // labels only relevant for non-variable pixels
    //void pasteResultIntoFinal(int c, const ushort* labels);
    void pasteResultIntoFinal(int c, Areal* x, const ushort* labels);

    //void computeRhs(const int c, const ushort* labels);
    void computeRhs(const int c, Areal* b, const ushort* labels);

    // given a new labeling, compares to old labeling and re-initializes _x where different
    //void modifyInitial(const int c, const ushort* newLabels);
    //void buildInitial(const int c, const ushort* newLabels); // completely initializes _x
    void modifyInitial(const int c, Areal *x, const ushort* newLabels);
    void buildInitial(const int c, Areal *x, const ushort* newLabels); // 

    void updateOffsetImgs(const ushort* newLabels);
    void buildOffsetImage(const int i);

    // Gets c channel color at out pixel imIndex according to _labels
    unsigned char getImgValue(const int imIndex, const int c) const;
    // Gets c channel color at out pixel imIndex according to passed in labels
    unsigned char getImgValue(const int imIndex, const int c, const ushort* labels) const; 
    // Gets c channel color at out pixel imIndex  from image im (_offsetImgs index)
    unsigned char getImgValue(const int imIndex, const int c, const int im) const;
    // projects out of bounds points to image region
    //unsigned char safeGetImgValue(const int imIndex, const int c) const;
    
    // given label image number, returns index to _offsetImgs
    int labelImToOffsetIm(const int i) const;

    // gets c channel color at out pixel imIndex in result array
    unsigned char getResultVal(const int imIndex, const int c) const {
      return _result[imIndex*3 + c]; }

    // given pixel indii of two neighboring pixels, looks up linktype
    LinkType lookupLink(const int index, const int lindex, const int l) const;

    // Given pixel index and link direction, returns pixel index
    // assumes 0<=l<4, returns -1 of out of bounds, look at proc body for l codes
    int linkResolve(const int index, const int l) const;

    // returns sum of magnitude of edge between two images in labels
    // (gI, fI are two pixel indii), vertCut is true if vertical cut, false otherwise
    Areal getE(const int gI, const int fI, const ushort* labels, const bool vertCut);

    //void saveX();

    //////// VARIABLES 

    int _w, _h, _wh, _numImages;
    Areal _edgeThres;
    FILE* _fp;
    std::vector<ImageAbs*> _imgs;

    unsigned char* _result;
    //set<int> _imMap; // set of relevant images (have labeled pixels)
    PixLinkTypes* _linkTypeLookup;
    int* _imgMap;  // for label image i, gives index into _offsetImgs
    std::vector<unsigned char*> _offsetImgs; // vector of images of 3*_wh uchar
    //Areal* _x;  // solution vector 
    //Areal* _b; // right-hand side
    ushort* _labels; // THese are the labels that led to our current result -- used to compare
    const ushort* _newLabels;
    // against new labels

    Areal *_xR, *_xG, *_xB, *_bR, *_bG, *_bB;
    bool _myStop;
  };


  ////////  INLINES

  inline unsigned char GlobalBlender::getImgValue(const int imIndex, const int c, const int im) const {
    assert(c>=0 && c<=3); 
    assert(imIndex < numPixels() && imIndex>=0);
    return _offsetImgs[im][3*imIndex+c];
  }

  inline unsigned char GlobalBlender::getImgValue(const int imIndex, const int c, const ushort* labels) const {
    assert(c>=0 && c<=3); 
    assert(imIndex < numPixels() && imIndex>=0);

    int whichIm = labelImToOffsetIm(labels[imIndex]);
    assert(whichIm != -1);
    return getImgValue(imIndex, c, whichIm);
  }

  inline unsigned char GlobalBlender::getImgValue(const int imIndex, const int c) const {
    return getImgValue(imIndex, c, _labels);
  }

  //inline unsigned char GlobalBlender::safeGetImgValue(const int imIndex, const int c) const {
  // 
  //}

  inline int GlobalBlender::labelImToOffsetIm(const int i) const {
    assert(i>=0 && i<_numImages);
    return _imgMap[i];
  }

  // assumes 0<=l<4, returns -1 of out of bounds
  inline int GlobalBlender::linkResolve(const int index, const int l) const { 
    //    0
    // 1  i  2
    //    3
    
    switch(l) {
    case 0:
      if (index >= _w) return index-_w;
      else return -1;
      break;
    case 1:
      if (index%_w == 0) return -1;
      else return index-1;
      break;
    case 2:
      if (index%_w == _w-1) return -1;
      else return index+1;
      break;
    case 3:
      if (index+_w > numPixels()) return -1;
      else return index+_w;
      break;
    }

    assert(0);
    return -1; // should never be reached
  }


  inline LinkType GlobalBlender::lookupLink(const int index, const int lindex, const int l) const { 
    //    0
    // 1  i  2
    //    3
    if (lindex==-1)      // out of bounds
      return L_IGNORE;
    switch(l) {
    case 0:
      return _linkTypeLookup[lindex].b;
      break;
    case 1:
      return _linkTypeLookup[lindex].r;
      break;
    case 2:
      return _linkTypeLookup[index].r;
      break;
    case 3:
      return _linkTypeLookup[index].b;
    }
    
    assert(0);
    return L_IGNORE; // should never reach here
  }


}



#endif
