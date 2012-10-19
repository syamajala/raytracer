#ifndef GSHAPE_H
#define GSHAPE_H
#include "gPoint.h"
#include "raytra.h"
#include "gMaterial.h"
#include "gMatrix.h"
#include <algorithm>

using namespace std;

class gIntersection {
  double _t1;
  double _t2;
  gVector _n;
  bool _i;
 public:
  gIntersection();
  gIntersection(double, double, const gVector&, bool);
  gIntersection(const gIntersection&);
  bool intersected();
  gVector getNormal();
  double getBestT();
  double getT1();
  double getT2();
  gIntersection& operator= (const gIntersection&);
};

class gShape {
 protected:
  gMaterial *_m;
 public:
  virtual gIntersection intersection(gRay&);
  gShape ();
  gMaterial getMaterial ();
};

class gSphere : public gShape{
  gVector _pos;
  double _r;
  friend ostream& operator<<(ostream&, const gSphere&);
 public:
  gSphere(const gVector, double, gMaterial*);
  gSphere(const gSphere&);
  gIntersection intersection(gRay&);  
};

class gTriangle : public gShape{
  gPoint _p1;
  gPoint _p2;
  gPoint _p3;
  friend ostream& operator<<(ostream&, const gTriangle&);
 public:
  gTriangle(const gPoint, const gPoint, const gPoint, gMaterial *);
  gIntersection intersection(gRay&);
};

class gPlane : public gShape{
  gVector _n;
  double _d;

 public:
  gPlane(gVector, double, gMaterial *);
  gIntersection intersection(gRay&);
  };
#endif
