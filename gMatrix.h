#ifndef GMATRIX_H
#define GMATRIX_H
#include <cstddef>
#include <iostream>
using namespace std;

class gNNMatrix {
  friend ostream& operator<<(ostream&, const gNNMatrix&);
 protected:
  double **_m;
  int _x;
  int _y;
 public:
  virtual double det();
  gNNMatrix();
  ~gNNMatrix();
  void setIJ(int i, int j, double v);
  double getIJ(int i, int j);
};

class g33Matrix: public gNNMatrix {  
 public:
  g33Matrix();
  double det();
};

#endif
