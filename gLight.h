#ifndef GLIGHT_H
#define GLIGHT_H
#include <algorithm>
#include <math.h>
#include <list>
#include "gPoint.h"
#include "gVector.h"
#include "gMaterial.h"
using namespace std;

class gShape;
class gRay;
class gLight {
 protected:
  gVector _color;  
 public:
  gVector getColor();
  virtual gVector shading(gMaterial, gVector, gVector, gVector, list<gShape *>*);
  virtual bool shadow(gRay, list<gShape *>*, double, double);
  virtual gPoint getPos();
};

class gALight : public gLight {
 public:
  gALight();
  gALight(const gVector);
  gALight(const gALight&);
  gVector shading(gMaterial, gVector, gVector, gVector, list<gShape *>*);
  bool shadow(gRay, list<gShape *>*, double, double);
};


class gPLight : public gLight {
  gPoint _pos;
 public:
  gPLight();
  gPLight(const gPoint, const gVector);
  gPLight(const gPLight&);
  gVector shading(gMaterial, gVector, gVector, gVector, list<gShape *>*);
  bool shadow(gRay, list<gShape *>*, double, double);
  gPoint getPos();
};

#endif
