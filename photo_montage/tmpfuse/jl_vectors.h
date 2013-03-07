#ifndef __JL_VECTORS_H__
#define __JL_VECTORS_H__


#include <stdlib.h>
//#include <iostream.h>
#include <math.h>
#include <assert.h>
//#include "jllib_include.h"

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#define SGN(a) ((a)<0 ? -1 : 1)


class Vec2i {

	private:
		int		_data[2];

	public:
		Vec2i() { _data[0] = _data[1] = 0; }
		Vec2i(const Vec2i &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
			}
		Vec2i(int d0, int d1) {
				_data[0] = d0;
				_data[1] = d1;
			}
		Vec2i(const Vec2i &V1, const Vec2i &V2) {
				_data[0] = V1._data[0] - V2._data[0];
				_data[1] = V1._data[1] - V2._data[1];
			}
		~Vec2i() { }

		int x() const { return _data[0]; }
		int y() const { return _data[1]; }
		void set_x(int x) { _data[0] = x; }
		void set_y(int y) { _data[1] = y; }
		const int* data() const { return _data; }
		int* data() { return _data; }

		Vec2i& operator=(const Vec2i &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				return *this;
			}/*
		friend ostream& operator<<(ostream &co, const Vec2i &V) {
				co << "[ " << V._data[0]
				   << " " << V._data[1] << " ]";
				return co;
				}*/
		int operator==(const Vec2i &V) const {
				return ((_data[0] == V._data[0]) &&
						(_data[1] == V._data[1]));
			}
		int operator!=(const Vec2i &V) const {
				return ((_data[0] != V._data[0]) ||
						(_data[1] != V._data[1]));
			}
		Vec2i& operator+=(const Vec2i &V) {
				_data[0] += V._data[0];
				_data[1] += V._data[1];
				return *this;
			}
		Vec2i& operator-=(const Vec2i &V) {
				_data[0] -= V._data[0];
				_data[1] -= V._data[1];
				return *this;
			}
		Vec2i& operator*=(int i) {
				_data[0] *= i;
				_data[1] *= i;
				return *this;
			}
		Vec2i& operator*=(float f) {
				_data[0] = int(_data[0] * f);
				_data[1] = int(_data[1] * f);
				return *this;
			}
		Vec2i& operator/=(int i) {
				_data[0] /= i;
				_data[1] /= i;
				return *this;
		}
		Vec2i& operator/=(float f) {
				_data[0] = int(_data[0] / f);
				_data[1] = int(_data[1] / f);
				return *this;
			}

		friend void Vec2i_Add(Vec2i &a , const Vec2i &b , const Vec2i &c ) {
				a._data[0] = b._data[0] + c._data[0];
				a._data[1] = b._data[1] + c._data[1];
			}
		friend void Vec2i_Sub(Vec2i &a , const Vec2i &b , const Vec2i &c ) {
				a._data[0] = b._data[0] - c._data[0];
				a._data[1] = b._data[1] - c._data[1];
			}
		friend void Vec2i_CopyScale(Vec2i &a , const Vec2i &b , int c ) {
				a._data[0] = b._data[0] * c;
				a._data[1] = b._data[1] * c;
			}
		friend void Vec2i_AddScale(Vec2i &a , const Vec2i &b , const Vec2i &c , int d ) {
				a._data[0] = b._data[0] + c._data[0] * d;
				a._data[1] = b._data[1] + c._data[1] * d;
			}
		friend void Vec2i_Average(Vec2i &a , const Vec2i &b , const Vec2i &c ) {
				a._data[0] = (b._data[0] + c._data[0]) / 2;
				a._data[1] = (b._data[1] + c._data[1]) / 2;
		}
  
		friend void Vec2i_WeightedSum(Vec2i &a , const Vec2i &b , int c , const Vec2i &d , int e ) {
				a._data[0] = b._data[0] * c + d._data[0] * e;
				a._data[1] = b._data[1] * c + d._data[1] * e;
			}

		int Dot2(const Vec2i &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] ;
			}

		void Set(int d0, int d1) {
				_data[0] = d0;
				_data[1] = d1;
			}
		void Get(int &d0, int &d1) const {
				d0 = _data[0];
				d1 = _data[1];
			}
		void Inc(int d0, int d1) {
				_data[0] += d0;
				_data[1] += d1;
			}
		void Dec(int d0, int d1) {
				_data[0] -= d0;
				_data[1] -= d1;
			}
		void Scale(int d0, int d1) {
				_data[0] *= d0;
				_data[1] *= d1;
			}
		void Divide(int d0, int d1) {
				_data[0] /= d0;
				_data[1] /= d1;
			}
		void Negate() {
				_data[0] = -_data[0];
				_data[1] = -_data[1];
			}
		void Clamp(int min, int max) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max) _data[1] = max;
			}
  // aseem:

		void Clamp(int min, int max, int max2) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max2) _data[1] = max2;
			}

  float distanceTo(const Vec2i& P) const {
    return sqrt(float((P._data[0] - _data[0])*(P._data[0] - _data[0]) + (P._data[1] - _data[1])*(P._data[1] - _data[1])));
  }

    int distanceTo2(const Vec2i& P) const {
	return ((P._data[0] - _data[0])*(P._data[0] - _data[0]) + (P._data[1] - _data[1])*(P._data[1] - _data[1]));
    }

    float Len() const {
      return sqrt(float(_data[0]*_data[0] + _data[1]*_data[1]));
    }
  
  float squaredLength() {
    return (_data[0]*_data[0] + _data[1]*_data[1]);
  }

    float Cross2(const Vec2i &V) const {
    return _data[0]*V._data[1] - V._data[0]*_data[1]; }
};


class Vec3i {

	private:
		int		_data[3];

	public:
		Vec3i() { _data[0] = _data[1] = _data[2] = 0; }
		Vec3i(const Vec3i &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				_data[2] = V._data[2];
			}
		Vec3i(int d0, int d1, int d2) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
			}
		Vec3i(const Vec3i &V1, const Vec3i &V2) {
				_data[0] = V1._data[0] - V2._data[0];
				_data[1] = V1._data[1] - V2._data[1];
				_data[2] = V1._data[2] - V2._data[2];
			}
		~Vec3i() { }

		int x() const { return _data[0]; }
		int y() const { return _data[1]; }
		int z() const { return _data[2]; }
		int r() const { return _data[0]; }
		int g() const { return _data[1]; }
		int b() const { return _data[2]; }
		void set_x(int x) { _data[0] = x; }
		void set_y(int y) { _data[1] = y; }
		void set_z(int z) { _data[2] = z; }
		void set_r(int r) { _data[0] = r; }
		void set_g(int g) { _data[1] = g; }
		void set_b(int b) { _data[2] = b; }
		const int* data() const { return _data; }
		int* data() { return _data; }

		Vec3i& operator=(const Vec3i &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				_data[2] = V._data[2];
				return *this;
			}/*
		friend ostream& operator<<(ostream &co, const Vec3i &V) {
				co << "[ " << V._data[0]
				   << " " << V._data[1]
				   << " " << V._data[2] << " ]";
				return co;
				}*/
		int operator==(const Vec3i &V) const {
				return ((_data[0] == V._data[0]) &&
						(_data[1] == V._data[1]) &&
						(_data[2] == V._data[2]));
			}
		int operator!=(const Vec3i &V) const {
				return ((_data[0] != V._data[0]) ||
						(_data[1] != V._data[1]) ||
						(_data[2] != V._data[2]));
			}
		Vec3i& operator+=(const Vec3i &V) {
				_data[0] += V._data[0];
				_data[1] += V._data[1];
				_data[2] += V._data[2];
				return *this;
			}
		Vec3i& operator-=(const Vec3i &V) {
				_data[0] -= V._data[0];
				_data[1] -= V._data[1];
				_data[2] -= V._data[2];
				return *this;
			}
		Vec3i& operator*=(int i) {
				_data[0] *= i;
				_data[1] *= i;
				_data[2] *= i;
				return *this;
			}
		Vec3i& operator*=(float f) {
				_data[0] = int(_data[0] * f);
				_data[1] = int(_data[1] * f);
				_data[2] = int(_data[2] * f);
				return *this;
			}
		Vec3i& operator/=(int i) {
				_data[0] /= i;
				_data[1] /= i;
				_data[2] /= i;
				return *this;
			}
		Vec3i& operator/=(float f) {
				_data[0] = int(_data[0] / f);
				_data[1] = int(_data[1] / f);
				_data[2] = int(_data[2] / f);
				return *this;
			}

		friend void Vec3i_Add(Vec3i &a , const Vec3i &b , const Vec3i &c ) {
				a._data[0] = b._data[0] + c._data[0];
				a._data[1] = b._data[1] + c._data[1];
				a._data[2] = b._data[2] + c._data[2];
			}
		friend void Vec3i_Sub(Vec3i &a , const Vec3i &b , const Vec3i &c ) {
				a._data[0] = b._data[0] - c._data[0];
				a._data[1] = b._data[1] - c._data[1];
				a._data[2] = b._data[2] - c._data[2];
			}
		friend void Vec3i_CopyScale(Vec3i &a , const Vec3i &b , int c ) {
				a._data[0] = b._data[0] * c;
				a._data[1] = b._data[1] * c;
				a._data[2] = b._data[2] * c;
			}
		friend void Vec3i_AddScale(Vec3i &a , const Vec3i &b , const Vec3i &c , int d ) {
				a._data[0] = b._data[0] + c._data[0] * d;
				a._data[1] = b._data[1] + c._data[1] * d;
				a._data[2] = b._data[2] + c._data[2] * d;
			}
		friend void Vec3i_Average(Vec3i &a , const Vec3i &b , const Vec3i &c ) {
				a._data[0] = (b._data[0] + c._data[0]) / 2;
				a._data[1] = (b._data[1] + c._data[1]) / 2;
				a._data[2] = (b._data[2] + c._data[2]) / 2;
			}
		friend void Vec3i_WeightedSum(Vec3i &a , const Vec3i &b , int c , const Vec3i &d , int e ) {
				a._data[0] = b._data[0] * c + d._data[0] * e;
				a._data[1] = b._data[1] * c + d._data[1] * e;
				a._data[2] = b._data[2] * c + d._data[2] * e;
			}

		int Dot2(const Vec3i &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] ;
			}
		int Dot3(const Vec3i &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] +
				       _data[2] * V._data[2] ;
			}
		friend void Vec3i_Cross3(Vec3i &c, const Vec3i &v1, const Vec3i &v2) {
				int x = v1._data[1]*v2._data[2] - v1._data[2]*v2._data[1];
				int y = v1._data[2]*v2._data[0] - v1._data[0]*v2._data[2];
				int z = v1._data[0]*v2._data[1] - v1._data[1]*v2._data[0];
				c._data[0] = x; c._data[1] = y; c._data[2] = z;
			}

		void Set(int d0, int d1, int d2) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
			}
		void Get(int &d0, int &d1, int &d2) const {
				d0 = _data[0];
				d1 = _data[1];
				d2 = _data[2];
			}
		void Inc(int d0, int d1, int d2) {
				_data[0] += d0;
				_data[1] += d1;
				_data[2] += d2;
			}
		void Dec(int d0, int d1, int d2) {
				_data[0] -= d0;
				_data[1] -= d1;
				_data[2] -= d2;
			}
		void Scale(int d0, int d1, int d2) {
				_data[0] *= d0;
				_data[1] *= d1;
				_data[2] *= d2;
			}
		void Divide(int d0, int d1, int d2) {
				_data[0] /= d0;
				_data[1] /= d1;
				_data[2] /= d2;
			}
		void Negate() {
				_data[0] = -_data[0];
				_data[1] = -_data[1];
				_data[2] = -_data[2];
			}
		void Clamp(int min, int max) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max) _data[1] = max;
				if      (_data[2] < min) _data[2] = min;
				else if (_data[2] > max) _data[2] = max;
			}

		void AugmentBound(int min_max[2][3]) const {
				if      (_data[0] < min_max[0][0]) min_max[0][0] = _data[0];
				else if (_data[0] > min_max[1][0]) min_max[1][0] = _data[0];
				if      (_data[1] < min_max[0][1]) min_max[0][1] = _data[1];
				else if (_data[1] > min_max[1][1]) min_max[1][1] = _data[1];
				if      (_data[2] < min_max[0][2]) min_max[0][2] = _data[2];
				else if (_data[2] > min_max[1][2]) min_max[1][2] = _data[2];
			}

  float distanceTo2(const Vec3i& other) const {
    return ((_data[0]-other._data[0])*(_data[0]-other._data[0]) +
	    (_data[1]-other._data[1])*(_data[1]-other._data[1]) +
	    (_data[2]-other._data[2])*(_data[2]-other._data[2]));
  }
};


class Vec4i {

	private:
		int		_data[4];

	public:
		Vec4i() { _data[0] = _data[1] = _data[2] = _data[3] = 0; }
		Vec4i(const Vec4i &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				_data[2] = V._data[2];
				_data[3] = V._data[3];
			}
		Vec4i(int d0, int d1, int d2, int d3) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
				_data[3] = d3;
			}
		Vec4i(const Vec3i &V, int w) {
				_data[0] = V.x();
				_data[1] = V.y();
				_data[2] = V.z();
				_data[3] = w;
			}
		Vec4i(const Vec4i &V1, const Vec4i &V2) {
				_data[0] = V1._data[0] - V2._data[0];
				_data[1] = V1._data[1] - V2._data[1];
				_data[2] = V1._data[2] - V2._data[2];
				_data[3] = V1._data[3] - V2._data[3];
			}
		~Vec4i() { }

		int x() const { return _data[0]; }
		int y() const { return _data[1]; }
		int z() const { return _data[2]; }
		int w() const { return _data[3]; }
		int r() const { return _data[0]; }
		int g() const { return _data[1]; }
		int b() const { return _data[2]; }
		int a() const { return _data[3]; }
		void set_x(int x) { _data[0] = x; }
		void set_y(int y) { _data[1] = y; }
		void set_z(int z) { _data[2] = z; }
		void set_w(int w) { _data[3] = w; }
		void set_r(int r) { _data[0] = r; }
		void set_g(int g) { _data[1] = g; }
		void set_b(int b) { _data[2] = b; }
		void set_a(int a) { _data[0] = a; }
		const int* data() const { return _data; }
		int* data() { return _data; }

		Vec4i& operator=(const Vec4i &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				_data[2] = V._data[2];
				_data[3] = V._data[3];
				return *this;
			}/*
		friend ostream& operator<<(ostream &co, const Vec4i &V) {
				co << "[ " << V._data[0]
				   << " " << V._data[1]
				   << " " << V._data[2]
				   << " " << V._data[3] << " ]";
				return co;
				}*/
		int operator==(const Vec4i &V) const {
				return ((_data[0] == V._data[0]) &&
						(_data[1] == V._data[1]) &&
						(_data[2] == V._data[2]) &&
						(_data[3] == V._data[3]));
			}
		int operator!=(const Vec4i &V) const {
				return ((_data[0] != V._data[0]) ||
						(_data[1] != V._data[1]) ||
						(_data[2] != V._data[2]) ||
						(_data[3] != V._data[3]));
			}
		Vec4i& operator+=(const Vec4i &V) {
				_data[0] += V._data[0];
				_data[1] += V._data[1];
				_data[2] += V._data[2];
				_data[3] += V._data[3];
				return *this;
			}
		Vec4i& operator-=(const Vec4i &V) {
				_data[0] -= V._data[0];
				_data[1] -= V._data[1];
				_data[2] -= V._data[2];
				_data[3] -= V._data[3];
				return *this;
			}
		Vec4i& operator*=(int i) {
				_data[0] *= i;
				_data[1] *= i;
				_data[2] *= i;
				_data[3] *= i;
				return *this;
			}
		Vec4i& operator*=(float f) {
				_data[0] = int(_data[0] * f);
				_data[1] = int(_data[1] * f);
				_data[2] = int(_data[2] * f);
				_data[3] = int(_data[3] * f);
				return *this;
			}
		Vec4i& operator/=(int i) {
				_data[0] /= i;
				_data[1] /= i;
				_data[2] /= i;
				_data[3] /= i;
				return *this;
			}
		Vec4i& operator/=(float f) {
				_data[0] = int(_data[0] / f);
				_data[1] = int(_data[1] / f);
				_data[2] = int(_data[2] / f);
				_data[3] = int(_data[3] / f);
				return *this;
			}

		friend void Vec4i_Add(Vec4i &a , const Vec4i &b , const Vec4i &c ) {
				a._data[0] = b._data[0] + c._data[0];
				a._data[1] = b._data[1] + c._data[1];
				a._data[2] = b._data[2] + c._data[2];
				a._data[3] = b._data[3] + c._data[3];
			}
		friend void Vec4i_Sub(Vec4i &a , const Vec4i &b , const Vec4i &c ) {
				a._data[0] = b._data[0] - c._data[0];
				a._data[1] = b._data[1] - c._data[1];
				a._data[2] = b._data[2] - c._data[2];
				a._data[3] = b._data[3] - c._data[3];
			}
		friend void Vec4i_CopyScale(Vec4i &a , const Vec4i &b , int c ) {
				a._data[0] = b._data[0] * c;
				a._data[1] = b._data[1] * c;
				a._data[2] = b._data[2] * c;
				a._data[3] = b._data[3] * c;
			}
		friend void Vec4i_AddScale(Vec4i &a , const Vec4i &b , const Vec4i &c , int d ) {
				a._data[0] = b._data[0] + c._data[0] * d;
				a._data[1] = b._data[1] + c._data[1] * d;
				a._data[2] = b._data[2] + c._data[2] * d;
				a._data[3] = b._data[3] + c._data[3] * d;
			}
		friend void Vec4i_Average(Vec4i &a , const Vec4i &b , const Vec4i &c ) {
				a._data[0] = (b._data[0] + c._data[0]) / 2;
				a._data[1] = (b._data[1] + c._data[1]) / 2;
				a._data[2] = (b._data[2] + c._data[2]) / 2;
				a._data[3] = (b._data[3] + c._data[3]) / 2;
			}
		friend void Vec4i_WeightedSum(Vec4i &a , const Vec4i &b , int c , const Vec4i &d , int e ) {
				a._data[0] = b._data[0] * c + d._data[0] * e;
				a._data[1] = b._data[1] * c + d._data[1] * e;
				a._data[2] = b._data[2] * c + d._data[2] * e;
				a._data[3] = b._data[3] * c + d._data[3] * e;
			}

		int Dot2(const Vec4i &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] ;
			}
		int Dot3(const Vec4i &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] +
				       _data[2] * V._data[2] ;
			}
		int Dot3(const Vec3i &V) const {
				return _data[0] * V.x() +
				       _data[1] * V.y() +
				       _data[2] * V.z() ;
			}
		int Dot4(const Vec4i &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] +
				       _data[2] * V._data[2] +
				       _data[3] * V._data[3] ;
			}
		friend void Vec4i_Cross3(Vec4i &c, const Vec4i &v1, const Vec4i &v2) {
				int x = v1._data[1]*v2._data[2] - v1._data[2]*v2._data[1];
				int y = v1._data[2]*v2._data[0] - v1._data[0]*v2._data[2];
				int z = v1._data[0]*v2._data[1] - v1._data[1]*v2._data[0];
				c._data[0] = x; c._data[1] = y; c._data[2] = z;
			}

		void Set(int d0, int d1, int d2, int d3) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
				_data[3] = d3;
			}
		void Get(int &d0, int &d1, int &d2, int &d3) const {
				d0 = _data[0];
				d1 = _data[1];
				d2 = _data[2];
				d3 = _data[3];
			}
		void Inc(int d0, int d1, int d2, int d3) {
				_data[0] += d0;
				_data[1] += d1;
				_data[2] += d2;
				_data[3] += d3;
			}
		void Dec(int d0, int d1, int d2, int d3) {
				_data[0] -= d0;
				_data[1] -= d1;
				_data[2] -= d2;
				_data[3] -= d3;
			}
		void Scale(int d0, int d1, int d2, int d3) {
				_data[0] *= d0;
				_data[1] *= d1;
				_data[2] *= d2;
				_data[3] *= d3;
			}
		void Divide(int d0, int d1, int d2, int d3) {
				_data[0] /= d0;
				_data[1] /= d1;
				_data[2] /= d2;
				_data[3] /= d3;
			}
		void Negate() {
				_data[0] = -_data[0];
				_data[1] = -_data[1];
				_data[2] = -_data[2];
				_data[3] = -_data[3];
			}
		void Clamp(int min, int max) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max) _data[1] = max;
				if      (_data[2] < min) _data[2] = min;
				else if (_data[2] > max) _data[2] = max;
				if      (_data[3] < min) _data[3] = min;
				else if (_data[3] > max) _data[3] = max;
			}

		void AugmentBound(int min_max[2][3]) const {
				if      (_data[0] < min_max[0][0]) min_max[0][0] = _data[0];
				else if (_data[0] > min_max[1][0]) min_max[1][0] = _data[0];
				if      (_data[1] < min_max[0][1]) min_max[0][1] = _data[1];
				else if (_data[1] > min_max[1][1]) min_max[1][1] = _data[1];
				if      (_data[2] < min_max[0][2]) min_max[0][2] = _data[2];
				else if (_data[2] > min_max[1][2]) min_max[1][2] = _data[2];
			}

};


class Vec2f {

	private:
		float		_data[2];

	public:
		Vec2f() { _data[0] = _data[1] = 0; }
		Vec2f(const Vec2f &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
			}
		Vec2f(float d0, float d1) {
				_data[0] = d0;
				_data[1] = d1;
			}
		Vec2f(const Vec2f &V1, const Vec2f &V2) {
				_data[0] = V1._data[0] - V2._data[0];
				_data[1] = V1._data[1] - V2._data[1];
			}
		~Vec2f() { }

		float x() const { return _data[0]; }
		float y() const { return _data[1]; }
		void set_x(float x) { _data[0] = x; }
		void set_y(float y) { _data[1] = y; }
		const float* data() const { return _data; }
		float* data() { return _data; }

		Vec2f& operator=(const Vec2f &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				return *this;
			}/*
		friend ostream& operator<<(ostream &co, const Vec2f &V) {
				co << "[ " << V._data[0]
				   << " " << V._data[1] << " ]";
				return co;
				}*/
		int operator==(const Vec2f &V) const {
				return ((_data[0] == V._data[0]) &&
						(_data[1] == V._data[1]));
			}
		int operator!=(const Vec2f &V) const {
				return ((_data[0] != V._data[0]) ||
						(_data[1] != V._data[1]));
			}
		Vec2f& operator+=(const Vec2f &V) {
				_data[0] += V._data[0];
				_data[1] += V._data[1];
				return *this;
			}
		Vec2f& operator-=(const Vec2f &V) {
				_data[0] -= V._data[0];
				_data[1] -= V._data[1];
				return *this;
			}
		Vec2f& operator*=(int i) {
				_data[0] = float(_data[0] * i);
				_data[1] = float(_data[1] * i);
				return *this;
			}
		Vec2f& operator*=(float f) {
				_data[0] *= f;
				_data[1] *= f;
				return *this;
			}
		Vec2f& operator/=(int i) {
				_data[0] = float(_data[0] / i);
				_data[1] = float(_data[1] / i);
				return *this;
			}
		Vec2f& operator/=(float f) {
				_data[0] /= f;
				_data[1] /= f;
				return *this;
			}

		friend void Vec2f_Add(Vec2f &a , const Vec2f &b , const Vec2f &c ) {
				a._data[0] = b._data[0] + c._data[0];
				a._data[1] = b._data[1] + c._data[1];
			}
		friend void Vec2f_Sub(Vec2f &a , const Vec2f &b , const Vec2f &c ) {
				a._data[0] = b._data[0] - c._data[0];
				a._data[1] = b._data[1] - c._data[1];
			}
		friend void Vec2f_CopyScale(Vec2f &a , const Vec2f &b , float c ) {
				a._data[0] = b._data[0] * c;
				a._data[1] = b._data[1] * c;
			}
		friend void Vec2f_AddScale(Vec2f &a , const Vec2f &b , const Vec2f &c , float d ) {
				a._data[0] = b._data[0] + c._data[0] * d;
				a._data[1] = b._data[1] + c._data[1] * d;
			}
		friend void Vec2f_Average(Vec2f &a , const Vec2f &b , const Vec2f &c ) {
				a._data[0] = (b._data[0] + c._data[0]) * 0.5;
				a._data[1] = (b._data[1] + c._data[1]) * 0.5;
			}
		friend void Vec2f_WeightedSum(Vec2f &a , const Vec2f &b , float c , const Vec2f &d , float e ) {
				a._data[0] = b._data[0] * c + d._data[0] * e;
				a._data[1] = b._data[1] * c + d._data[1] * e;
			}

  friend void Vec2f_LinInterp(Vec2f &a, const Vec2f &b, const Vec2f &c, float t) {
    a._data[0] = t*(c._data[0]-b._data[0]) + b._data[0];
    a._data[1] = t*(c._data[1]-b._data[1]) + b._data[1];
  }
  
		float Dot2(const Vec2f &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] ;
			}

  float Cross2(const Vec2f &V) const {
    return _data[0]*V._data[1] - V._data[0]*_data[1]; }
  
		void Set(float d0, float d1) {
				_data[0] = d0;
				_data[1] = d1;
			}
		void Get(float &d0, float &d1) const {
				d0 = _data[0];
				d1 = _data[1];
			}
		void Inc(float d0, float d1) {
				_data[0] += d0;
				_data[1] += d1;
			}
		void Dec(float d0, float d1) {
				_data[0] -= d0;
				_data[1] -= d1;
			}
		void Scale(float d0, float d1) {
				_data[0] *= d0;
				_data[1] *= d1;
			}
		void Divide(float d0, float d1) {
				_data[0] /= d0;
				_data[1] /= d1;
			}
		float Len() const {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] );
				return l;
		}

  float Len2() const {return (_data[0] * _data[0] + _data[1] * _data[1]); }
  
		float Length() const {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] );
				return l;
			}
		void Negate() {
				_data[0] = -_data[0];
				_data[1] = -_data[1];
			}
		void Clamp(float min, float max) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max) _data[1] = max;
			}

		void Clamp(float min, float max, float max2) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max2) _data[1] = max2;
			}

		float Normalize() {
				float l = sqrt( _data[0] * _data[0] +
						_data[1] * _data[1]);
				if (l > 0) {
					_data[0] /= l;
					_data[1] /= l;
				}
				return l;
		}

    float distanceTo2(const Vec2f& P) const {
	return ((P._data[0] - _data[0])*(P._data[0] - _data[0]) + (P._data[1] - _data[1])*(P._data[1] - _data[1]));
    }

  float distanceTo(const Vec2f& P) const { return sqrt(distanceTo2(P)); }
};


class Vec3f {

	private:
		float		_data[3];

	public:
		Vec3f() { _data[0] = _data[1] = _data[2] = 0; }
		Vec3f(const Vec3f &V) {
				_data[0] = V._data[0];
			_data[1] = V._data[1];
				_data[2] = V._data[2];
			}
		Vec3f(float d0, float d1, float d2) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
			}
		Vec3f(const Vec3f &V1, const Vec3f &V2) {
				_data[0] = V1._data[0] - V2._data[0];
				_data[1] = V1._data[1] - V2._data[1];
				_data[2] = V1._data[2] - V2._data[2];
			}
		~Vec3f() { }

		float x() const { return _data[0]; }
		float y() const { return _data[1]; }
		float z() const { return _data[2]; }
		float r() const { return _data[0]; }
		float g() const { return _data[1]; }
		float b() const { return _data[2]; }
		void set_x(float x) { _data[0] = x; }
		void set_y(float y) { _data[1] = y; }
		void set_z(float z) { _data[2] = z; }
		void set_r(float r) { _data[0] = r; }
		void set_g(float g) { _data[1] = g; }
		void set_b(float b) { _data[2] = b; }
		const float* data() const { return _data; }
		float* data() { return _data; }
  float operator[](const int &i) {assert(i>-1 && i<3); return _data[i];}
		Vec3f& operator=(const Vec3f &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				_data[2] = V._data[2];
				return *this;
			}/*
		friend ostream& operator<<(ostream &co, const Vec3f &V) {
				co << "[ " << V._data[0]
				   << " " << V._data[1]
				   << " " << V._data[2] << " ]";
				return co;
				}*/
		int operator==(const Vec3f &V) const {
				return ((_data[0] == V._data[0]) &&
						(_data[1] == V._data[1]) &&
						(_data[2] == V._data[2]));
			}
		int operator!=(const Vec3f &V) const {
				return ((_data[0] != V._data[0]) ||
						(_data[1] != V._data[1]) ||
						(_data[2] != V._data[2]));
			}
		Vec3f& operator+=(const Vec3f &V) {
				_data[0] += V._data[0];
				_data[1] += V._data[1];
				_data[2] += V._data[2];
				return *this;
			}
		Vec3f& operator-=(const Vec3f &V) {
				_data[0] -= V._data[0];
				_data[1] -= V._data[1];
				_data[2] -= V._data[2];
				return *this;
			}
		Vec3f& operator*=(int i) {
				_data[0] = float(_data[0] * i);
				_data[1] = float(_data[1] * i);
				_data[2] = float(_data[2] * i);
				return *this;
			}
		Vec3f& operator*=(float f) {
				_data[0] *= f;
				_data[1] *= f;
				_data[2] *= f;
				return *this;
			}
		Vec3f& operator/=(int i) {
				_data[0] = float(_data[0] / i);
				_data[1] = float(_data[1] / i);
				_data[2] = float(_data[2] / i);
				return *this;
			}
		Vec3f& operator/=(float f) {
				_data[0] /= f;
				_data[1] /= f;
				_data[2] /= f;
				return *this;
			}

		friend void Vec3f_Add(Vec3f &a , const Vec3f &b , const Vec3f &c ) {
				a._data[0] = b._data[0] + c._data[0];
				a._data[1] = b._data[1] + c._data[1];
				a._data[2] = b._data[2] + c._data[2];
			}
		friend void Vec3f_Sub(Vec3f &a , const Vec3f &b , const Vec3f &c ) {
				a._data[0] = b._data[0] - c._data[0];
				a._data[1] = b._data[1] - c._data[1];
				a._data[2] = b._data[2] - c._data[2];
			}
		friend void Vec3f_CopyScale(Vec3f &a , const Vec3f &b , float c ) {
				a._data[0] = b._data[0] * c;
				a._data[1] = b._data[1] * c;
				a._data[2] = b._data[2] * c;
			}
		friend void Vec3f_AddScale(Vec3f &a , const Vec3f &b , const Vec3f &c , float d ) {
				a._data[0] = b._data[0] + c._data[0] * d;
				a._data[1] = b._data[1] + c._data[1] * d;
				a._data[2] = b._data[2] + c._data[2] * d;
			}
		friend void Vec3f_Average(Vec3f &a , const Vec3f &b , const Vec3f &c ) {
				a._data[0] = (b._data[0] + c._data[0]) * 0.5;
				a._data[1] = (b._data[1] + c._data[1]) * 0.5;
				a._data[2] = (b._data[2] + c._data[2]) * 0.5;
			}
		friend void Vec3f_WeightedSum(Vec3f &a , const Vec3f &b , float c , const Vec3f &d , float e ) {
				a._data[0] = b._data[0] * c + d._data[0] * e;
				a._data[1] = b._data[1] * c + d._data[1] * e;
				a._data[2] = b._data[2] * c + d._data[2] * e;
			}

		friend void Vec3f_Lerp(Vec3f& r, const Vec3f& a, const Vec3f &b, float i) {
		  r._data[0] = a._data[0] + i*(b._data[0] - a._data[0]);
 		  r._data[1] = a._data[1] + i*(b._data[1] - a._data[1]);
		  r._data[2] = a._data[2] + i*(b._data[2] - a._data[2]);
		}

		float Dot2(const Vec3f &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] ;
			}
		float Dot3(const Vec3f &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] +
				       _data[2] * V._data[2] ;
			}
		friend void Vec3f_Cross3(Vec3f &c, const Vec3f &v1, const Vec3f &v2) {
				float x = v1._data[1]*v2._data[2] - v1._data[2]*v2._data[1];
				float y = v1._data[2]*v2._data[0] - v1._data[0]*v2._data[2];
				float z = v1._data[0]*v2._data[1] - v1._data[1]*v2._data[0];
				c._data[0] = x; c._data[1] = y; c._data[2] = z;
			}

		void Set(float d0, float d1, float d2) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
			}
		void Get(float &d0, float &d1, float &d2) const {
				d0 = _data[0];
				d1 = _data[1];
				d2 = _data[2];
			}
		void Inc(float d0, float d1, float d2) {
				_data[0] += d0;
				_data[1] += d1;
				_data[2] += d2;
			}
		void Dec(float d0, float d1, float d2) {
				_data[0] -= d0;
				_data[1] -= d1;
				_data[2] -= d2;
			}
		void Scale(float d0, float d1, float d2) {
				_data[0] *= d0;
				_data[1] *= d1;
				_data[2] *= d2;
			}
		void Divide(float d0, float d1, float d2) {
				_data[0] /= d0;
				_data[1] /= d1;
				_data[2] /= d2;
			}
		float Len() const {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] );
				return l;
		}
  
  float Len2() const { return (_data[0] * _data[0] +
			     _data[1] * _data[1] +
			     _data[2] * _data[2]);
  }

  float distanceTo2(const Vec3f& P) const {
    return ((P._data[0] - _data[0])*(P._data[0] - _data[0]) + 
	    (P._data[1] - _data[1])*(P._data[1] - _data[1]) +
	    (P._data[2] - _data[2])*(P._data[2] - _data[2]));
  }

  
		float Length() const {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] );
				return l;
			}
		void MakeUnit() {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] );
				if (l > 0) {
					_data[0] /= l;
					_data[1] /= l;
					_data[2] /= l;
				}
			}
		void Normalize() {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] );
				if (l > 0) {
					_data[0] /= l;
					_data[1] /= l;
					_data[2] /= l;
				}
			}
		Vec3f& MakeAndGetUnit() {
				MakeUnit();
				return *this;
			}
		void Negate() {
				_data[0] = -_data[0];
				_data[1] = -_data[1];
				_data[2] = -_data[2];
			}
		void Clamp(float min, float max) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max) _data[1] = max;
				if      (_data[2] < min) _data[2] = min;
				else if (_data[2] > max) _data[2] = max;
			}

		void AugmentBound(float min_max[2][3]) const {
				if      (_data[0] < min_max[0][0]) min_max[0][0] = _data[0];
				else if (_data[0] > min_max[1][0]) min_max[1][0] = _data[0];
				if      (_data[1] < min_max[0][1]) min_max[0][1] = _data[1];
				else if (_data[1] > min_max[1][1]) min_max[1][1] = _data[1];
				if      (_data[2] < min_max[0][2]) min_max[0][2] = _data[2];
				else if (_data[2] > min_max[1][2]) min_max[1][2] = _data[2];
			}

		unsigned long RGBColor() const {
				return ((unsigned long)(_data[0] * 255.99999)) |
					   ((unsigned long)(_data[1] * 255.99999) << 8) |
					   ((unsigned long)(_data[2] * 255.99999) << 16);
			}
		void FromRGBColor(unsigned long c) {
				_data[0] = float(c & 0x00FF) / 255;
				_data[1] = float((c >> 8) & 0x00FF) / 255;
				_data[2] = float((c >> 16) & 0x00FF) / 255;
		}

  void addIntegers(const Vec3i& ints) {
    _data[0] += float(ints.x());
    _data[1] += float(ints.y());
    _data[2] += float(ints.z());
  }
  
};


class Vec4f {

	private:
		float		_data[4];

	public:
		Vec4f() { _data[0] = _data[1] = _data[2] = _data[3] = 0; }
		Vec4f(const Vec4f &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				_data[2] = V._data[2];
				_data[3] = V._data[3];
			}
		Vec4f(float d0, float d1, float d2, float d3) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
				_data[3] = d3;
			}
		Vec4f(const Vec3f &V, float w) {
				_data[0] = V.x();
				_data[1] = V.y();
				_data[2] = V.z();
				_data[3] = w;
			}
		Vec4f(const Vec4f &V1, const Vec4f &V2) {
				_data[0] = V1._data[0] - V2._data[0];
				_data[1] = V1._data[1] - V2._data[1];
				_data[2] = V1._data[2] - V2._data[2];
				_data[3] = V1._data[3] - V2._data[3];
			}
		~Vec4f() { }

		float x() const { return _data[0]; }
		float y() const { return _data[1]; }
		float z() const { return _data[2]; }
		float w() const { return _data[3]; }
		float r() const { return _data[0]; }
		float g() const { return _data[1]; }
		float b() const { return _data[2]; }
		float a() const { return _data[3]; }
		void set_x(float x) { _data[0] = x; }
		void set_y(float y) { _data[1] = y; }
		void set_z(float z) { _data[2] = z; }
		void set_w(float w) { _data[3] = w; }
		void set_r(float r) { _data[0] = r; }
		void set_g(float g) { _data[1] = g; }
		void set_b(float b) { _data[2] = b; }
		void set_a(float a) { _data[0] = a; }
		const float* data() const { return _data; }
		float* data() { return _data; }
  float operator[](const int &i) {assert(i>-1 && i<4); return _data[i];}
		Vec4f& operator=(const Vec4f &V) {
				_data[0] = V._data[0];
				_data[1] = V._data[1];
				_data[2] = V._data[2];
				_data[3] = V._data[3];
				return *this;
			}/*
		friend ostream& operator<<(ostream &co, const Vec4f &V) {
				co << "[ " << V._data[0]
				   << " " << V._data[1]
				   << " " << V._data[2]
				   << " " << V._data[3] << " ]";
				return co;
				}*/
		int operator==(const Vec4f &V) const {
				return ((_data[0] == V._data[0]) &&
						(_data[1] == V._data[1]) &&
						(_data[2] == V._data[2]) &&
						(_data[3] == V._data[3]));
			}
		int operator!=(const Vec4f &V) const {
				return ((_data[0] != V._data[0]) ||
						(_data[1] != V._data[1]) ||
						(_data[2] != V._data[2]) ||
						(_data[3] != V._data[3]));
			}
		Vec4f& operator+=(const Vec4f &V) {
				_data[0] += V._data[0];
				_data[1] += V._data[1];
				_data[2] += V._data[2];
				_data[3] += V._data[3];
				return *this;
			}
		Vec4f& operator-=(const Vec4f &V) {
				_data[0] -= V._data[0];
				_data[1] -= V._data[1];
				_data[2] -= V._data[2];
				_data[3] -= V._data[3];
				return *this;
			}
		Vec4f& operator*=(int i) {
				_data[0] = float(_data[0] * i);
				_data[1] = float(_data[1] * i);
				_data[2] = float(_data[2] * i);
				_data[3] = float(_data[3] * i);
				return *this;
			}
		Vec4f& operator*=(float f) {
				_data[0] *= f;
				_data[1] *= f;
				_data[2] *= f;
				_data[3] *= f;
				return *this;
			}
		Vec4f& operator/=(int i) {
				_data[0] = float(_data[0] / i);
				_data[1] = float(_data[1] / i);
				_data[2] = float(_data[2] / i);
				_data[3] = float(_data[3] / i);
				return *this;
			}
		Vec4f& operator/=(float f) {
				_data[0] /= f;
				_data[1] /= f;
				_data[2] /= f;
				_data[3] /= f;
				return *this;
			}

		friend void Vec4f_Add(Vec4f &a , const Vec4f &b , const Vec4f &c ) {
				a._data[0] = b._data[0] + c._data[0];
				a._data[1] = b._data[1] + c._data[1];
				a._data[2] = b._data[2] + c._data[2];
				a._data[3] = b._data[3] + c._data[3];
			}
		friend void Vec4f_Sub(Vec4f &a , const Vec4f &b , const Vec4f &c ) {
				a._data[0] = b._data[0] - c._data[0];
				a._data[1] = b._data[1] - c._data[1];
				a._data[2] = b._data[2] - c._data[2];
				a._data[3] = b._data[3] - c._data[3];
			}
		friend void Vec4f_CopyScale(Vec4f &a , const Vec4f &b , float c ) {
				a._data[0] = b._data[0] * c;
				a._data[1] = b._data[1] * c;
				a._data[2] = b._data[2] * c;
				a._data[3] = b._data[3] * c;
			}
		friend void Vec4f_AddScale(Vec4f &a , const Vec4f &b , const Vec4f &c , float d ) {
				a._data[0] = b._data[0] + c._data[0] * d;
				a._data[1] = b._data[1] + c._data[1] * d;
				a._data[2] = b._data[2] + c._data[2] * d;
				a._data[3] = b._data[3] + c._data[3] * d;
			}
		friend void Vec4f_Average(Vec4f &a , const Vec4f &b , const Vec4f &c ) {
				a._data[0] = (b._data[0] + c._data[0]) * 0.5;
				a._data[1] = (b._data[1] + c._data[1]) * 0.5;
				a._data[2] = (b._data[2] + c._data[2]) * 0.5;
				a._data[3] = (b._data[3] + c._data[3]) * 0.5;
			}
		friend void Vec4f_WeightedSum(Vec4f &a , const Vec4f &b , float c , const Vec4f &d , float e ) {
				a._data[0] = b._data[0] * c + d._data[0] * e;
				a._data[1] = b._data[1] * c + d._data[1] * e;
				a._data[2] = b._data[2] * c + d._data[2] * e;
				a._data[3] = b._data[3] * c + d._data[3] * e;
			}

		float Dot2(const Vec4f &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] ;
			}
		float Dot3(const Vec4f &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] +
				       _data[2] * V._data[2] ;
			}
		float Dot3(const Vec3f &V) const {
				return _data[0] * V.x() +
				       _data[1] * V.y() +
				       _data[2] * V.z() ;
			}
		float Dot4(const Vec4f &V) const {
				return _data[0] * V._data[0] +
				       _data[1] * V._data[1] +
				       _data[2] * V._data[2] +
				       _data[3] * V._data[3] ;
			}
		friend void Vec4f_Cross3(Vec4f &c, const Vec4f &v1, const Vec4f &v2) {
				float x = v1._data[1]*v2._data[2] - v1._data[2]*v2._data[1];
				float y = v1._data[2]*v2._data[0] - v1._data[0]*v2._data[2];
				float z = v1._data[0]*v2._data[1] - v1._data[1]*v2._data[0];
				c._data[0] = x; c._data[1] = y; c._data[2] = z;
			}

		void Set(float d0, float d1, float d2, float d3) {
				_data[0] = d0;
				_data[1] = d1;
				_data[2] = d2;
				_data[3] = d3;
			}
		void Get(float &d0, float &d1, float &d2, float &d3) const {
				d0 = _data[0];
				d1 = _data[1];
				d2 = _data[2];
				d3 = _data[3];
			}
		void Inc(float d0, float d1, float d2, float d3) {
				_data[0] += d0;
				_data[1] += d1;
				_data[2] += d2;
				_data[3] += d3;
			}
		void Dec(float d0, float d1, float d2, float d3) {
				_data[0] -= d0;
				_data[1] -= d1;
				_data[2] -= d2;
				_data[3] -= d3;
			}
		void Scale(float d0, float d1, float d2, float d3) {
				_data[0] *= d0;
				_data[1] *= d1;
				_data[2] *= d2;
				_data[3] *= d3;
			}
		void Divide(float d0, float d1, float d2, float d3) {
				_data[0] /= d0;
				_data[1] /= d1;
				_data[2] /= d2;
				_data[3] /= d3;
			}
		float Len() const {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] +
								 _data[3] * _data[3] );
				return l;
			}
		float Length() const {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] +
								 _data[3] * _data[3] );
				return l;
			}
		void MakeUnit() {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] );
				if (l > 0) {
					_data[0] /= l;
					_data[1] /= l;
					_data[2] /= l;
				}
			}
		void Normalize() {
				float l = sqrt( _data[0] * _data[0] +
								 _data[1] * _data[1] +
								 _data[2] * _data[2] );
				if (l > 0) {
					_data[0] /= l;
					_data[1] /= l;
					_data[2] /= l;
				}
			}
		Vec4f& MakeAndGetUnit() {
				MakeUnit();
				return *this;
			}
		void Negate() {
				_data[0] = -_data[0];
				_data[1] = -_data[1];
				_data[2] = -_data[2];
				_data[3] = -_data[3];
			}
		void Clamp(float min, float max) {
				if      (_data[0] < min) _data[0] = min;
				else if (_data[0] > max) _data[0] = max;
				if      (_data[1] < min) _data[1] = min;
				else if (_data[1] > max) _data[1] = max;
				if      (_data[2] < min) _data[2] = min;
				else if (_data[2] > max) _data[2] = max;
				if      (_data[3] < min) _data[3] = min;
				else if (_data[3] > max) _data[3] = max;
			}

		void AugmentBound(float min_max[2][3]) const {
				if      (_data[0] < min_max[0][0]) min_max[0][0] = _data[0];
				else if (_data[0] > min_max[1][0]) min_max[1][0] = _data[0];
				if      (_data[1] < min_max[0][1]) min_max[0][1] = _data[1];
				else if (_data[1] > min_max[1][1]) min_max[1][1] = _data[1];
				if      (_data[2] < min_max[0][2]) min_max[0][2] = _data[2];
				else if (_data[2] > min_max[1][2]) min_max[1][2] = _data[2];
			}
		void DivideByW() {
				if (_data[3] != 0) {
					_data[0] /= _data[3];
					_data[1] /= _data[3];
					_data[2] /= _data[3];
				}
				else
					_data[0] = _data[1] = _data[2] = 0;
				_data[3] = 1;
			}

		unsigned long RGBColor() const {
				return ((unsigned long)(_data[0] * 255.99999)) |
					   ((unsigned long)(_data[1] * 255.99999) << 8) |
					   ((unsigned long)(_data[2] * 255.99999) << 16);
			}
		unsigned long RGBAColor() const {
				return ((unsigned long)(_data[0] * 255.99999)) |
					   ((unsigned long)(_data[1] * 255.99999) << 8) |
					   ((unsigned long)(_data[2] * 255.99999) << 16) |
					   ((unsigned long)(_data[3] * 255.99999) << 24);
			}
		void FromRGBColor(unsigned long c) {
				_data[0] = float(c & 0x00FF) / 255;
				_data[1] = float((c >> 8) & 0x00FF) / 255;
				_data[2] = float((c >> 16) & 0x00FF) / 255;
				_data[3] = 0;
			}
		void FromRGBAColor(unsigned long c) {
				_data[0] = float(c & 0x00FF) / 255;
				_data[1] = float((c >> 8) & 0x00FF) / 255;
				_data[2] = float((c >> 16) & 0x00FF) / 255;
				_data[3] = float((c >> 24) & 0x00FF) / 255;
			}
};


#endif
