#ifndef RAYTRA_H
#define RAYTRA_H
#include "gPoint.h"
#include "gVector.h"
#include "gLight.h"
#include "gMaterial.h"
#include <list>
#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>

using namespace std;
using namespace Imf;
using namespace Imath;

class gRay {
  gPoint _origin;
  gVector _dir;
 public:
  gRay ();
  gRay (gPoint&, gPoint&, gPoint&);
  gRay (gPoint&, gVector&);
  gRay (const gRay&);
  gPoint getOrigin();
  gVector getDir();
};

class gShape;

class gCamera {
  gPoint _eye;
  gVector _w;
  gVector _v;
  gVector _u;   
  double _d;
  
  int _nx;
  int _ny;
  
  double _width, _height;

  list <gShape *> *_s;
  list <gLight *> *_l;
  list <gMaterial *> *_m;
  gALight _al;
 public:
  gCamera (gPoint, gVector, double, double, double, int, int);
  ~gCamera ();
  gRay compute_ray(int, int, int, int, int);
  void render(const char*);
  void writeRgba (const Rgba*, const char*);
  void setShapes(list<gShape *>*);
  void setLights(list<gLight *>*);
  void setMaterials(list<gMaterial *>*);
  void setAL(const gALight);  
  gVector L(gRay, double, double, int, int, gVector, gLight *);
};

#endif 
