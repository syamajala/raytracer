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
  virtual gVector shading(gMaterial&, gVector&, gVector&, gVector&, list<gShape *>*);
  virtual bool shadow(gRay&, list<gShape *>*, double, double);
  virtual gPoint getPos();
};

class gALight : public gLight {
 public:
  gALight();
  gALight(const gVector);
  gALight(const gALight&);
  gVector shading(gMaterial&, gVector&, gVector&, gVector&, list<gShape *>*);
  bool shadow(gRay&, list<gShape *>*, double, double);
};


class gPLight : public gLight {
  gPoint _pos;
 public:
  gPLight();
  gPLight(const gPoint, const gVector);
  gPLight(const gPLight&);
  gVector shading(gMaterial&, gVector&, gVector&, gVector&, list<gShape *>*);
  bool shadow(gRay&, list<gShape *>*, double, double, int);
  gPoint getPos();
};

class gARLight : public gLight {
  gPoint _pos;
  gVector _dir;
  gVector _udir;
  gVector _vdir;
  double _len;
 public:
  gARLight();
  gARLight(gPoint, gVector, gVector, double, gVector);
  gPoint getPos();
  gVector getDir();
  gVector getUDir();
  gVector getVDir();
  double getLen();
  gVector shading(gMaterial&, gVector&, gVector&, gVector&, list<gShape *>*);
  bool shadow(gRay&, list<gShape *>*, double, double, int);
};

#endif
