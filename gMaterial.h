#ifndef GMATERIAL_H
#define GMATERIAL_H
#include "gVector.h"

using namespace std;

class gMaterial {
  gVector _diff;
  gVector _spec;
  gVector _ideal;
  double _r;
  friend ostream& operator<<(ostream&, const gMaterial&);
 public:
  gMaterial(const gVector, const gVector, double, const gVector);
  gMaterial(const gMaterial&);
  gVector getDiff();
  gVector getSpec();
  gVector getIdeal();
  double getPhong();
};

#endif
