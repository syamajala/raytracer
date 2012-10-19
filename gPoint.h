#ifndef GPOINT_H
#define GPOINT_H
#include "gVector.h"
#include <iostream>

using namespace std;

class gPoint {
  double _x, _y, _z;
  friend gPoint operator+ (const gPoint&, const gVector&);
  friend gPoint operator+ (const gVector&, const gPoint&);
  friend istream& operator>>(istream&, gPoint&);
  friend ostream& operator<<(ostream&, const gPoint&);
 public:
  gPoint ();
  gPoint (double, double, double);
  gPoint (const gPoint&);
  gPoint (gVector v);
  gVector operator- (const gPoint p);
  double operator[] (int i);
};

#endif
