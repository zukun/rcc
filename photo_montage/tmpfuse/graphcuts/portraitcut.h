#ifndef PORTRAITCUT_H
#define PORTRAITCUT_H

//#pragma unmanaged

#include <vector>
#include <queue>
#include <stdio.h>
#include "../coord.h"
#include "../ImageAbs.h"
#include "image.h"  
#include "imageData.h"
#include <wx/wx.h>
#include <wx/thread.h>
#include "defs.h"

#define SINGLE_THREAD 1

using namespace std;


// C_GRAD is gradmag divide, C_GRAD2 is comparing actual grads
enum CutType {C_NORMAL, C_GRAD, C_GRAD2, C_BOTH};

#define _ITER_MAX_ 100

static inline double SQR(const float a) { return a*a; }

//extern wxMutex* finishedMutex;

class PortraitCut  {

  // width, height: image dimensions
  // images: vector of char* images, where images are rgb for pixel 0, rgb for pixel 1, etc...
  // displacements: vector, better be same length as images, contains x-y displacements relative to 0th image.  0th image better have 0,0 displacement!  We can change how we encode this, if you prefer.
  // required: width*height image.  Pixel is -1 if unknown, otherwise has index of image requiring it
  // result: you allocate.  width*height.  Will set each pixel to an index of an image.

 public:

  //thread stuff
  //ExitCode Entry () { /*finishedMutex->Lock ();*/ _myStop = false; printf("entry\n"); compute (); return 0;}
  void myStop() { _myStop = true;}

  PortraitCut(int oWidth, int oHeight, vector<ImageAbs*>* images, short* required, ushort* result, FILE* fp);

  PortraitCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result, FILE* fp);

  virtual ~PortraitCut();  

  void saveLabels(const char* name);  

  void saveComp(char* name);   

  virtual void loadLabeling(const char* name);

  virtual void compute();

  virtual void addImage(ImageAbs* im);
  virtual void removeLastImage();

  ushort* getLabels() {
    return _labels; }
  
  ushort getLabel(const int i, const int j) { assert(i>=0 && i<_w && j>=0 && j<_h); return _labels[j*_w + i]; }

  void allocateAuxData(const ushort i);
  void allocateAllAuxData();

  unsigned char* comp();

  void setCuttype(const CutType c) { _cuttype = c; }
  CutType getCuttype() const { return _cuttype; }

  //short* getRequired() { return _required; }

  float getCurrDataTerm(const int i, const int j);
  float getCurrMaxInterTerm(const int i, const int j);

  virtual float BVZ_data_penalty(Coord p, ushort d);
  virtual float BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl);

  virtual void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) = 0;
  virtual float getDataCost (const struct StrokeData* s, Coord p, ushort d) = 0;

  const ImageData* getImageData() const { return _idata; }

  wxFrame* _th_frame;

  float _potts, _regular_inter, _bright;

  //----------------------------------- PRIVATE

 protected:
 
  void mutualInit();
 
  unsigned char* _imptr(short l, Coord p) {
    return _idata->images(l)->transformedPixel(p);
  }


  void _displace(Coord& p, ushort l) {
    _idata->images(l)->displace(p);
  }


  double BVZ_ComputeEnergy();
  double BVZ_ComputeSpatialOnly();
  double BVZ_ComputeDataOnly();
  double BVZ_Expand(ushort a, double E_old);
  void initializeResult();


  int _n,_w,_h,_wh;
  Coord _size;
  //short* _required;
  ushort* _labels;
  //bool _ownRequired;

  ImageData* _idata;
  bool _myStop;

  FILE* _fp;
  CutType _cuttype;

  PtrImage		indeces_a;		/* node indeces for assignments originated from pixels in the left image */
  DoubleImage		D_a;					/* penalty for making an assignment active */


};



//#define IMPTR(l, p) ( ((p)>=Coord(0,0) && (p)<_im_size) ? (*_images)[(int)l] + 3*((p.y)*_w + (p.x)) : __black  )

// FROM THEIR UGLY CODE------------------------

/* (half of) the neighborhood system
   the full neighborhood system is edges in NEIGHBORS
   plus reversed edges in NEIGHBORS */
const struct Coord NEIGHBORS[] = { Coord(1, 0), Coord(0, -1) };
#define NEIGHBOR_NUM (sizeof(NEIGHBORS) / sizeof(Coord))
#define CINDEX(p) ((p.y)*_w+(p.x))




#define INDEX_NONPRESENT ((void*)1)
#define INDEX_ACTIVE     ((void*)0)
#define IS_NODE_ID(index) ((unsigned)index>1)

//#pragma managed


inline float CGrad::val(int c) const {
  if (c<3)
    return _x[c];
  c -=3;
  return _y[c];
}
inline void CGrad::setVal(int c, const float d) {
  if (c<3)
    _x[c] = d;
  else {
    c -=3;
    _y[c] = d;
  }
}

BEGIN_DECLARE_EVENT_TYPES()
     DECLARE_EVENT_TYPE(TH_REFRESH_VIEWER, 7777)
     DECLARE_EVENT_TYPE(END_GLOBAL_COMPUTE, 8888)
END_DECLARE_EVENT_TYPES()

#endif
 

