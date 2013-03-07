#ifndef COORD_H
#define COORD_H
#include <iostream>
#include <math.h>

typedef unsigned int uint;

struct Coord
{
  int x, y;

  Coord() {}
  Coord(int a, int b) { x = a; y = b; }

  Coord operator- ()        { return Coord(-x, -y); }
  Coord operator+ (Coord a) { return Coord(x + a.x, y + a.y); }
  Coord operator- (Coord a) { return Coord(x - a.x, y - a.y); }
  bool  operator< (Coord a) { return (x <  a.x) && (y <  a.y); }
  bool  operator<=(Coord a) { return (x <= a.x) && (y <= a.y); }
  bool  operator> (Coord a) { return (x >  a.x) && (y >  a.y); }
  bool  operator>=(Coord a) { return (x >= a.x) && (y >= a.y); }
  bool  operator==(Coord a) { return (x == a.x) && (y == a.y); }
  bool  operator!=(Coord a) { return (x != a.x) || (y != a.y); }
  void operator-=(Coord a) { x-=a.x; y-=a.y;}
  void Inc(const int i, const int j) { x+=i; y+=j;}
  void Dec(const int i, const int j) { x-=i; y-=j;}
  double distTo(const Coord& o) { return sqrt(double((x-o.x)*(x-o.x) + (y-o.y)*(y-o.y))); }


};

std::istream& operator>>(std::istream& s, Coord& c);

std::ostream& operator<<(std::ostream& s, Coord& c);

#define IMREF(im, p) (imRef((im), (p).x, (p).y))

//#pragma managed

#endif
