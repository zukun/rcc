#ifndef IMAGEABS_H
#define IMAGEABS_H
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <vector>
#define _USE_MATH_DEFINES
#include "math.h"
#include "coord.h"
#include "jl_vectors.h"

class ImageAbs {
 public:
  ImageAbs(int w, int h, bool allocate=true);	
  ImageAbs();

  ImageAbs(const ImageAbs& other);

  ImageAbs(int w, int h, const unsigned char* data);

  ~ImageAbs();

  ImageAbs& operator=(const ImageAbs& o);

  void init(const int x, const int y);

  bool allocated() const { return _data != NULL; }
  void deallocate();

  void takeData(unsigned char* d);

  void setSize(const int x, const int y) { _size.x=x; _size.y=y; }
  int getWidth() const { return _size.x; }
  int getHeight() const { return _size.y; }

  void setBrightness (float val);

  unsigned char redData(const int x, const int y) const;

  const unsigned char* data(const int x, const int y) const;
  const unsigned char* data() const { return _data; }
  unsigned char* mutableData() { return _data; };

  Vec3i getBlurColor(const int x, const int y);

  bool valid() const;

  //inline bool rotated() const;

  void writePPM(char* fname);

  void readPPM(char* fname);

  //const Coord& displacement() const { return _displacement; }
  //float scale() const { return _scale; }
  //float angle() const { return _rot; }

  void translate(const int x, const int y); // relative
  void scale(float s); // relative
  void rotate(float s); // relative

  void setTrans(const float* M); // absolute

  void setScale(const Vec2f& s);   // absolute
  void setRotate(const float a);  // absolute

  void getGLTrans(float* M) const;

  void displace(Coord& p) const;
  
  bool transformed() const { return _transformed; }

  unsigned char* transformedPixel(Coord p);
  const unsigned char* transformedPixel(Coord p) const;


  static void initBlack() {
    __black[0] =   __black[1] =   __black[2] = 0;
  }

  int _num;
  Coord _size;
  //float _brightness;


  // Made these private so _transformed stays consistent other stuff

 private:

  void setTransformIdentity();
  void matMult(float* A, const float* B);
  void printTrans();


  unsigned char* _data;
  bool _transformed;
  float _trans[6];
  float _invtrans[6];
  //   [ s_x cos(theta),   sin(theta),      d_x ]
  //   [ -sin(theta),      s_y cos(theta),  d_y ]


  static unsigned char __black[3];
};

//inline bool ImageAbs::rotated() const {
//return (_rot!=0);
//}



#endif
