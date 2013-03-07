#include "ImageAbs.h"

unsigned char ImageAbs::__black[3];

ImageAbs::ImageAbs(int w, int h, bool allocate) {
  if (allocate) {
    _data = new unsigned char[3*w*h];
    memset(_data,0,3*w*h*sizeof(unsigned char));
  }
  else
    _data = NULL;
  _size.x = w;
  _size.y = h;
  //_displacement.x=_displacement.y=0;
  _num=-1;
  //_brightness = 1.f;
  
  setTransformIdentity();
}

void ImageAbs::setTransformIdentity() {
  _transformed = false;
  memset(_trans, 0, 6*sizeof(float));
  _trans[0] = _trans[4] = 1.;
  memcpy(_invtrans, _trans, 6*sizeof(float));
}
	
ImageAbs::ImageAbs() {
  _data = NULL; 
  _size.x = _size.y = 0;
  //_displacement.x=_displacement.y=0;
  _num=-1;

   setTransformIdentity();
}

ImageAbs::ImageAbs(const ImageAbs& other) {
  _size = other._size;
  if (other._data) {
    _data = new unsigned char[3*_size.x*_size.y];
    memcpy(_data, other._data, 3*_size.x*_size.y*sizeof(unsigned char));
  }
  else
    _data = NULL;
  //_displacement.x=_displacement.y=0;
  _num=-1;

  _transformed = other._transformed;
  memcpy(_trans, other._trans, 6*sizeof(float));
  memcpy(_invtrans, other._invtrans, 6*sizeof(float));
}

ImageAbs::ImageAbs(int w, int h, const unsigned char* data) {
  _size.x = w; _size.y = h;
  //_displacement.x=_displacement.y=0;
  _num=-1;
  _data = new unsigned char[3*_size.x*_size.y];
  memcpy(_data, data, 3*_size.x*_size.y*sizeof(unsigned char));
  //_brightness = 1.f;

  setTransformIdentity();
  //_parentData = NULL;

}

ImageAbs::~ImageAbs() { 
  if (_data)
    delete[] _data; 
  //if (_parentData)
  //delete[] _parentData;
}

ImageAbs& ImageAbs::operator=(const ImageAbs& o) {
  _num = o._num;

  if (!o._data) {
    if (_data) delete[] _data;
    _data = NULL;
    return *this;
  }

  if (_size != o._size ||  !_data) {
    if (_data) delete[] _data;
    _size = o._size;
    _data = new unsigned char[3*_size.x*_size.y];
  }

  memcpy(_data, o._data, 3*_size.x*_size.y*sizeof(unsigned char));
  _num = o._num;

  memcpy(_trans, o._trans, 6*sizeof(float));
  memcpy(_invtrans, o._invtrans, 6*sizeof(float));
  _transformed = o._transformed;

  return *this;
}

void ImageAbs::deallocate() {
  delete _data; _data = NULL;
  _num = -1;
  setTransformIdentity();
}


unsigned char ImageAbs::redData(const int x, const int y) const {  
  assert(x>=0 && x<_size.x && y>=0 && y<_size.y);
  return _data[3*(y*_size.x+x)];}

const unsigned char* ImageAbs::data(const int x, const int y) const {  
  assert(x>=0 && x<_size.x && y>=0 && y<_size.y);
  return _data + 3*(y*_size.x+x);}




void ImageAbs::writePPM(char* fname) {
  FILE* fp;
  if ( (fp = fopen(fname, "wb")) == NULL) {
    assert(0);
    std::exit(0);
  }
	  
  /* Write header */
  fprintf(fp, "P6\n");
  fprintf(fp, "%d %d\n", _size.x, _size.y);
  fprintf(fp, "255\n");
	  
  fwrite(_data, sizeof(unsigned char), 3*_size.x*_size.y, fp); 

}

void ImageAbs::readPPM(char* fname) {
  FILE* fp;

  if ( (fp = fopen(fname, "rb")) == NULL) {
    assert(0);
    std::exit(0);
  }
  int width, height, maxComp;
  char cookie[3];
  fscanf(fp, "%2s", &cookie);
  if (strcmp("P6", cookie)) std::exit(0); /* not right file type */
  //skipComment(fin);
  fscanf(fp, "%d", &width);
  //skipComment(fin);	
  fscanf(fp, "%d", &height);
  //skipComment(fin);		
  fscanf(fp, "%d", &maxComp);
  fread(cookie, 1, 1, fp); /* Read newline which follows maxval */  

  if (maxComp != 255) std::exit(0); /* data error */

  _size.x = width; _size.y = height;
  _data = new unsigned char[3*width*height];
  size_t res = fread(_data, sizeof(unsigned char),  3*width*height, fp);
  assert((int)res == 3*_size.x*_size.y);
}


void ImageAbs::init(const int x, const int y) {
  if (_data) delete[] _data;
  setSize(x,y);
  int w = _size.x, h = _size.y;
  _data = new unsigned char[3*w*h];
  memset(_data,0,3*w*h*sizeof(unsigned char));
  setTransformIdentity();
  _num=-1;
}

void ImageAbs::takeData(unsigned char* d) {
  if (_data) delete[] _data;  
  _data = d;
}

Vec3i ImageAbs::getBlurColor(const int x, const int y) {
  Vec3i sums;
  int kernel[9] = {1,2,1, 2,3,2, 1,2,1};
  int index=0;
  for (int j=-1; j<=1; ++j)
    for (int i=-1; i<=1; ++i, ++index) {
      const unsigned char* c = _data + 3*( (y+j)*_size.x + x+i );
      sums.Inc(kernel[index]*c[0], kernel[index]*c[1], kernel[index]*c[2]);
    }
  sums /= 15.f;
  return sums;
}



/*
void ImageAbs::setBrightness (float val) {

  //  float lum = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];

  float red_scale = 1.f+ val*(0.50);//*.3086;
  float green_scale = 1.f+val*(0.50);//*.6094;
  float blue_scale = 1.f+val*(0.50);//*.082;

  //	float red_scale = val*(5);
  //	float green_scale = val*(5);
  //	float blue_scale = val*(5);

  for (int i=0; i < _size.x; i++) {
    for (int j=0; j < _size.y; j++) {

      int red = int(_parentData[(j*_size.x + i)*3]*red_scale);
      if (red < 256 && red > -1)
	_data[(j*_size.x + i)*3] = red;
      else if (red > 255)
	_data[(j*_size.x + i)*3] = 255;
      else
	_data[(j*_size.x + i)*3] = 0;

      int green = int(_parentData[(j*_size.x + i)*3+1]*green_scale);
      if (green < 256 && green > -1)
	_data[(j*_size.x + i)*3+1] = green;
      else if (green > 255)
	_data[(j*_size.x + i)*3+1] = 255;
      else
	_data[(j*_size.x + i)*3+1] = 0;
			
      int blue = int(_parentData[(j*_size.x + i)*3+2]*blue_scale);
      if (blue < 256 && blue > -1)
	_data[(j*_size.x + i)*3+2] = blue;
      else if (blue > 255)
	_data[(j*_size.x + i)*3+2] = 255;
      else
	_data[(j*_size.x + i)*3+2] = 0;
    }
  }
}
*/

std::istream& operator>>(std::istream& s, Coord& c) {
  s >> c.x >> c.y;
  return s;
}

std::ostream& operator<<(std::ostream& s, Coord& c) {
  s << c.x << c.y;
  return s;
}

bool ImageAbs::valid() const {
  if (_data && (this->_data[0] || !this->_data[0]))
    return true;
  else
    return false;
}



// inverse transform
void ImageAbs::displace(Coord& p) const {

  if (_transformed) {
    float x = p.x, y = p.y;
    x -= _size.x*.5; y -= _size.y*.5;
    x += _invtrans[2]; y +=  _invtrans[5];
    p.x = (int) (_invtrans[0]*x + _invtrans[1]*y + _size.x*.5);
    p.y = (int) (_invtrans[3]*x + _invtrans[4]*y + _size.y*.5);
    
  }
  /*
  if (rotated()) {
    float x = p.x, y = p.y, nx, ny;
    x -= _size.x/2;
    y -= _size.y/2;
    nx = _rotcos*x  + _rotsin*y;
    ny = -_rotsin*x + _rotcos*y;
    nx += _size.x/2;
    ny += _size.y/2;
    p.x = (int)nx;
    p.y = (int)ny;
  }
  p -= displacement();
  if (scale() != 1.f) {
    p.x = (int) (float(p.x) / scale());
    p.y = (int) (float(p.y) / scale());
  }
  */
}

void ImageAbs::translate(const int x, const int y) {
  _transformed = true;
  _trans[2] += (float)x;
  _trans[5] += (float)y;
  _invtrans[2] = -_trans[2];
  _invtrans[5] = -_trans[5];
  //_displacement.x += x;
  //_displacement.y += y;
  //printf("translating to %d %d\n",_displacement.x, _displacement.y);
}

void ImageAbs::scale(float s) { // relative
  _transformed = true;
  _trans[0] *= s;
  _trans[4] *= s;
  s = 1./s;
  _invtrans[0] *= s;
  _invtrans[4] *= s;
}

void ImageAbs::rotate(float s) { // relative
  //printf("rotating %f\n",s);
  _transformed = true;
  float rad = s*M_PI / 180.f;
  float coss = cos(rad);
  float sins = sin(rad);
  
  float rot[4] = {coss, -sins, sins, coss};
  matMult(_trans,rot);
  //printTrans();
  
  rot[1] = sins; rot[2] = -sins;
  matMult(_invtrans, rot);
}

void ImageAbs::setTrans(const float* M) { // absolute
  _transformed = true;
  memcpy(_trans, M, 6*sizeof(float));
  //printTrans();
  // compute inverse
  _invtrans[2] = -_trans[2];
  _invtrans[5] = -_trans[5];

  float fac = 1. / (_trans[0]*_trans[4] - _trans[1]*_trans[3]);
  _invtrans[0] = fac*_trans[4];
  _invtrans[1] = -fac*_trans[1];
  _invtrans[3] = -fac*_trans[3];
  _invtrans[4] = fac*_trans[0];
  _invtrans[2] = -_trans[2];
  _invtrans[5] = -_trans[5];

  //printf("%.3f %.3f %.3f\n", _invtrans[0], _invtrans[1], _invtrans[2]);
  //printf("%.3f %.3f %.3f\n\n", _invtrans[3], _invtrans[4], _invtrans[5]);
}

void ImageAbs::printTrans() {
  printf("%.3f %.3f %.3f\n", _trans[0], _trans[1], _trans[2]);
  printf("%.3f %.3f %.3f\n\n", _trans[3], _trans[4], _trans[5]);
}

// A[1:2,1:2] *= B, where A is 2x3 and B is 2x2
void ImageAbs::matMult(float* A, const float* B) {
  float Ao[4] = {A[0],A[1],A[3],A[4]};
  A[0] = Ao[0]*B[0] + Ao[1]*B[2];
  A[1] = Ao[0]*B[1] + Ao[1]*B[3];
  A[3] = Ao[2]*B[0] + Ao[3]*B[2];
  A[4] = Ao[2]*B[1] + Ao[3]*B[3];
}


void ImageAbs::setScale(const Vec2f& s) {
  float costheta = sqrt(1. - _trans[1]*_trans[1]);
  _trans[0] = s.x() * costheta;
  _trans[4] = s.y() * costheta;
  _invtrans[0] = costheta / s.x();
  _invtrans[4] = costheta / s.y();
  _transformed = true;
}


void ImageAbs::setRotate(const float a) {
  _transformed = true;
  float costp = cos(a*M_PI / 180.f);
  float sintp = sqrt(1. - costp*costp);
  float cost = sqrt(1. - _trans[1]*_trans[1]);
  
  _invtrans[0] = costp * cost /_trans[0];
  _invtrans[4] = costp * cost /_trans[4];
  _invtrans[1] = -sintp;  _invtrans[3] = sintp;

  _trans[0] = _trans[0] * costp / cost;
  _trans[4] = _trans[4] * costp / cost;
  _trans[1] = sintp;  _trans[3] = -sintp;

  //if (_rot!=0)
  //_transformed = true;
  //_rot = a;
  //_rotcos = cos(_rot*M_PI / 180.f);
  //_rotsin = sin(_rot*M_PI / 180.f);
}

void ImageAbs::getGLTrans(float* M) const {
  float myM[16] = {_trans[0], _trans[3], 0, 0, 
		   _trans[1], _trans[4], 0, 0, 
		   0,          0,        1, 0,
		   _trans[2], _trans[5], 0, 1,};
  memcpy(M, myM, 16*sizeof(float));  
}
unsigned char* ImageAbs::transformedPixel(Coord p) {
  if (transformed())
      displace(p);
    //_displace(p,l);
    if (p>=Coord(0,0) && p<_size) {
      //assert (*(_images[(int)l]->_data));
      unsigned char *res = _data + 3*(p.y * _size.x + p.x);
      return res;
    }
    else
      return __black;
}


const unsigned char* ImageAbs::transformedPixel(Coord p) const {
  if (transformed())
      displace(p);
    //_displace(p,l);
    if (p>=Coord(0,0) && p<_size) {
      //assert (*(_images[(int)l]->_data));
      unsigned char *res = _data + 3*(p.y * _size.x + p.x);
      return res;
    }
    else
      return __black;
}



