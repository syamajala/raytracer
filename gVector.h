#ifndef GVECTOR_H
#define GVECTOR_H
#include <iostream>
using namespace std;

class gPoint;

class gVector {
  double _x, _y, _z;
  friend gPoint operator+ (const gPoint&, const gVector&);
  friend gPoint operator+ (const gVector&, const gPoint&);
  friend gVector operator* (const gVector&, double);
  friend gVector operator* (double, const gVector&);
  friend istream& operator>>(istream&, gVector&);
  friend ostream& operator<<(ostream&, const gVector&);
 public:
  gVector ();
  gVector (double, double, double);
  gVector (const gVector&);
  gVector operator+ (const gVector&);
  gVector operator- (const gVector&);
  gVector operator* (const gVector&);
  gVector& operator= (const gVector&); 
  bool operator== (const gVector&);
  bool operator!= (const gVector&);
  double operator[] (int i);
  double dot (const gVector&);
  double length ();
  gVector normalize();
};

#endif
