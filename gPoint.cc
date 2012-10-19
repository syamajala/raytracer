#ifndef gPoint_h
#define gPoint_h
#include "gPoint.h"
#endif

gPoint::gPoint() {
  _x = 0;
  _y = 0;
  _z = 0;
}

gPoint::gPoint(double x1, double y1, double z1) {
  _x = x1;
  _y = y1;
  _z = z1;
}

gPoint::gPoint(const gPoint &p) {
  _x = p._x;
  _y = p._y;
  _z = p._z;
}

gPoint::gPoint(gVector& v) {
  _x = v[0];
  _y = v[1];
  _z = v[2];
}

gPoint operator+(const gPoint &p, const gVector &v) {
  return gPoint(p._x+v._x, p._y+v._y, p._z+v._z);
}

gPoint operator+(const gVector &v, const gPoint &p) {
  return p + v;
}

gVector gPoint::operator-(const gPoint& p) {
  return gVector(p._x - _x, p._y - _y, p._z - _z);
}

double gPoint::operator[](int i) {
  switch (i) {
  case 0: return _x;
  case 1: return _y;
  case 2: return _z;
  default: return 0;
  }
}

ostream& operator<<(ostream& output, const gPoint &p) {
  output << "(" << p._x << ", " << p._y << ", " << p._z << ")";
  return output;
}
