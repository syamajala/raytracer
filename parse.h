#ifndef PARSE_H
#define PARSE_H
#include <cstdlib>
#include <iostream>
#include <list>
#include "gPoint.h"
#include "gVector.h"
#include "gMaterial.h"
#include "gShape.h"
using namespace std;

inline istream &operator>>(istream &is, gPoint &f) {
  return is>>f._x>>f._y>>f._z;
}

inline istream &operator>>(istream &is, gVector &f) {
  return is>>f._x>>f._y>>f._z;
}

class gCamera;

class Parser { 
 public:
  gCamera* parse(const char *file);
  void read_wavefront_file(const char*, list<gShape *>*, gMaterial*);
};

#endif
