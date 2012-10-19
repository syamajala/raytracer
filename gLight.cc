#include "gLight.h"
#include "gShape.h"
using namespace std;

gVector gLight::getColor() {
  return _color;
}

gVector gLight::shading(gMaterial& m, gVector& v1, gVector& v2, gVector& dd, list<gShape *> *shapes) {
  return gVector();
}

bool gLight::shadow(gRay& r, list<gShape *> *shapes, double tmin, double tmax) {
  return false;
}

gPoint gLight::getPos() {
  return gPoint();
}

gALight::gALight() {
  _color = gVector(0,0,0);
}

gALight::gALight(const gALight &a) {
  _color = a._color;
}

gALight::gALight(const gVector c) {
  _color = c;
}

gVector gALight::shading(gMaterial& m, gVector& v1, gVector& v2, gVector& dd, list<gShape *> *shapes) {    
  gVector ka = m.getDiff();
  return gVector(ka[0]*_color[0], ka[1]*_color[1], ka[2]*_color[2]);
}

bool gALight::shadow(gRay& r, list<gShape *> *shapes, double tmin, double tmax) {
  return false;
}

gPLight::gPLight() {
  _color = gVector();
  _pos = gPoint();
}

gPLight::gPLight(const gPoint pos, const gVector color) {
  _color = color;
  _pos = pos;
}

gPLight::gPLight(const gPLight &p) {
  _color = p._color;
  _pos = p._pos;
}

gVector gPLight::shading(gMaterial& m, gVector& nn, gVector& pp, gVector& dd, list<gShape *>*shapes) {  gVector l = (gVector(_pos[0], _pos[1], _pos[2])-pp).normalize();
  gVector n = nn.normalize();

  gVector diffShading = gVector();
  if (n.dot(l) > 0) {
    gVector diff = m.getDiff();
    double ma2 = max(0.0, n.dot(l));
    diffShading = (ma2*gVector(diff[0]*_color[0], diff[1]*_color[1], diff[2]*_color[2]));
  }

  gVector h = (l + ((-1*dd).normalize())).normalize();
  gVector specShading = gVector();
  if (h.length() > 0) {
    if (n.dot(h) > 0) {
      gVector spec = m.getSpec();
      double ma = pow(max(0.0, n.dot(h)), m.getPhong());
      specShading = (ma*gVector(spec[0]*_color[0],spec[1]*_color[1],spec[2]*_color[2]));
    }
  }
  return (diffShading + specShading);
}

bool gPLight::shadow(gRay& r, list<gShape *>*shapes, double tmin, double tmax) {
  list <gShape *>::iterator s;
  for (s = (*shapes).begin(); s != (*shapes).end(); ++s) {
    gIntersection in = (*(*s)).intersection(r);    
    if (in.intersected() && 
	(in.getT1() > tmin) &&
	(in.getT2() > tmin) &&
	(in.getT1() < tmax) &&
	(in.getT2() < tmax))
	return true;
  }
  return false;
}

gPoint gPLight::getPos() {
  return _pos;
}

gARLight::gARLight() {
  _pos = gPoint();
  _dir = gVector();
  _udir = gVector();
  _len = 0;
  _color = gVector();
}

gARLight::gARLight(gPoint p, gVector dir, gVector udir, double len, gVector color) {
  _pos = p;
  _dir = dir;
  _udir = udir;
  _len = len;
  _color = color;
}
