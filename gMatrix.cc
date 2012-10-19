#include "gMatrix.h"

using namespace std;

gNNMatrix::gNNMatrix() {
  _m = NULL;
}

gNNMatrix::~gNNMatrix() {
  for (int i = 0; i < _x; ++i) {
      delete[] _m[i];
  }
  delete[] _m;
}

double gNNMatrix::det() {
  return 0;
}

void gNNMatrix::setIJ(int i, int j, double v) {
  if ((i-1 > _x) || (j-1 > _y) || (i-1 < 0) || (j-1 < 0)) {
    cout << "matrix set error" << endl;
    return;
  }
  _m[i-1][j-1] = v;
}

double gNNMatrix::getIJ(int i, int j) {
  if ((i-1 > _x) || (j-1 > _y) || (i-1 < 0) || (j-1 < 0)) {
    cout << "matrix get error" << endl;
    return 0;
  }
  return _m[i-1][j-1];
}

ostream& operator<<(ostream& output, const gNNMatrix &m) {
  for (int i = 0; i < m._x; ++i) {
    for (int j = 0; j < m._y; ++j) {
      output << m._m[i][j] << "\t";
    }
    output << endl;
  }
  return output;
}

g33Matrix::g33Matrix() {
  _m = new double*[3];
  _x = 3;
  _y = 3;
  for (int i = 0; i < 3; ++i)
    _m[i] = new double[3];
  
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      _m[i][j] = 0;
}

double g33Matrix::det() {
  double aei = (_m[0][0])*(_m[1][1])*(_m[2][2]);
  double bfg = (_m[0][1])*(_m[1][2])*(_m[2][0]);
  double cdh = (_m[0][2])*(_m[1][0])*(_m[2][1]);
  double ceg = (_m[0][2])*(_m[1][1])*(_m[2][0]);
  double bdi = (_m[0][0])*(_m[1][2])*(_m[2][1]);
  double afh = (_m[0][1])*(_m[1][0])*(_m[2][2]);

  return aei+bfg+cdh-ceg-bdi-afh;
}

