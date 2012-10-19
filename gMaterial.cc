#include "gMaterial.h"

gMaterial::gMaterial (const gVector diff, const gVector spec, double r, const gVector ideal) {
  _diff = diff;
  _spec = spec;
  _r = r;
  _ideal = ideal;
}

gMaterial::gMaterial (const gMaterial &m) {
  _diff = m._diff;
  _spec = m._spec;
  _r = m._r;
  _ideal = m._ideal;
}

gVector gMaterial::getDiff() {
  return _diff;
}

gVector gMaterial::getSpec() {
  return _spec;
}

gVector gMaterial::getIdeal() {
  return _ideal;
}

double gMaterial::getPhong() {
  return _r;
}

ostream& operator<<(ostream& output, const gMaterial &m) {
  output << "diffuse: " << m._diff << " specular: " << m._spec << " ideal: " << m._ideal << " phong const: " << m._r; 
  return output;
}
