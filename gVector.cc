#include "gVector.h"
#include <cmath>

gVector::gVector () {
  _x = 0;
  _y = 0;
  _z = 0;
}

gVector::gVector (const gVector &v) {
  _x = v._x;
  _y = v._y;
  _z = v._z;
}

gVector::gVector (double x1, double y1, double z1) {
  _x = x1;
  _y = y1;
  _z = z1;
}

gVector gVector::operator+ (const gVector v) {
  return gVector (_x + v._x, _y + v._y, _z + v._z);
}

gVector gVector::operator- (const gVector v) {
  return gVector (_x - v._x, _y - v._y, _z - v._z);
}

gVector gVector::operator* (const gVector v) {
  return gVector (_y*v._z - _z*v._y, _z*v._x - _x*v._z, _x*v._y - _y*v._x);
}

gVector operator* (const gVector v, double n) {
  return gVector(v._x*n, v._y*n, v._z*n);
}

gVector operator* (double n, const gVector v) {
  return v*n;
}

ostream& operator<<(ostream& output, const gVector &v) {
  output << "(" << v._x << ", " << v._y << ", " << v._z << ")";
  return output;
}

double gVector::operator[] (int i) {
  switch (i) {
  case 0: return _x;
  case 1: return _y;
  case 2: return _z;
  default: return 0;
  }
}

double gVector::dot (gVector v) {
  return (_x*v._x)+(_y*v._y)+(_z*v._z);
}

double gVector::length () {
  return sqrt(_x*_x + _y*_y + _z*_z);
}

gVector gVector::normalize() {
  double l;
  if (length() != 0)
    l = 1/((*this).length());
  else
    l = 1;
  return l*(*this);
}

gVector& gVector::operator=(const gVector &v) {
  if (this == &v)
    return *this;
  
  _x = v._x;
  _y = v._y;
  _z = v._z;
  return *this;
}

bool gVector::operator==(const gVector &v) {
  if ((_x == v._x) && (_y == v._y) && (_z == v._z))
    return true;
  else
    return false;
}

bool gVector::operator!=(const gVector &v) {
  return !((*this) == v);
}
