#include "gShape.h"

using namespace std;

gShape::gShape() {
  _m = NULL;
}

gMaterial gShape::getMaterial() {
  return *_m;
}

gIntersection::gIntersection(double ip1, double ip2, const gVector& v, bool i1) {
  _t1 = ip1;
  _t2 = ip2;
  _n = v;
  _i = i1;
}

gIntersection::gIntersection() {
  _t1 = 0;
  _t2 = 0;
  _n = gVector();
  _i = false;
}

gIntersection::gIntersection(const gIntersection &i1) {
  _t1 = i1._t1;
  _t2 = i1._t2;
  _n = i1._n;
  _i = i1._i;
}

bool gIntersection::intersected() {
  return _i;
}

gVector gIntersection::getNormal() {
  return _n;
}

double gIntersection::getBestT() {
  if ((_t2 < 0) && _t1)
    return _t1;

  return min(_t1, _t2);
}

double gIntersection::getT1() {
  return _t1;
}

double gIntersection::getT2() {
  return _t2;
}

gIntersection& gIntersection::operator=(const gIntersection &i) {
  if (this == &i)
    return *this;

  _t1 = i._t1;
  _t2 = i._t2;
  _n = i._n;
  _i = i._i;
  
  return *this;
}

gIntersection gShape::intersection(gRay& r) {
  return gIntersection();
}

gSphere::gSphere (const gVector pos1, double r1, gMaterial *m) {
  _pos = pos1;
  _r = r1;
  _m = m;
}

gSphere::gSphere (const gSphere &s) {
  _pos = s._pos;
  _r = s._r;
}

gIntersection gSphere::intersection(gRay& r1) {
  gVector d = r1.getDir();
  gPoint p = r1.getOrigin();
  gVector p0 = gVector(p[0], p[1], p[2]);
  gVector p0minuspos = p0-_pos;

  double a = d.dot(d);
  double b = d.dot(p0minuspos);
  double c = (p0minuspos.dot(p0minuspos))-(_r*_r);
  double disc = (b*b)-(a*c);

  double t1 = 0;
  double t2 = 0;

  gVector sp;

  if (disc < 0) {
    return gIntersection();        
  } else if (disc == 1) {
    t1 = (-b + sqrt(disc))/a;
    sp = p0+(t1*d);
    return gIntersection(t1, -1, (1/_r)*(sp-_pos), true);
  } else {
    t1 = (-b + sqrt(disc))/a;
    t2 = (-b - sqrt(disc))/a;
    if (t1 > t2) 
      sp = p0+(t2*d);
    else if (t1 < t2)
      sp = p0+(t1*d);    
    return gIntersection(t1, t2, (1/_r)*(sp-_pos), true);
    }    
}

ostream& operator<<(ostream& output, const gSphere &s) {
  output << s._pos << " " << s._r;
  return output;
}

gTriangle::gTriangle (const gPoint p1, const gPoint p2, const gPoint p3, gMaterial *m) {
  _p1 = p1;
  _p2 = p2;
  _p3 = p3;
  _m = m;
}

ostream& operator<<(ostream& output, const gTriangle &t) {
  output << t._p1 << " " << t._p2 << " " << t._p3;
  return output;
}

gIntersection gTriangle::intersection(gRay& r) {
  gPoint e = r.getOrigin();
  gVector d = r.getDir();  
  double t0 = 0;
  double t1 = 100000;

  g33Matrix a = g33Matrix();
  a.setIJ(1,1, _p1[0] - _p2[0]); // Xa - Xb
  a.setIJ(2,1, _p1[1] - _p2[1]); // Ya - Yb
  a.setIJ(3,1, _p1[2] - _p2[2]); // Za - Zb

  a.setIJ(1,2, _p1[0] - _p3[0]); // Xa - Xc
  a.setIJ(2,2, _p1[1] - _p3[1]); // Ya - Yc
  a.setIJ(3,2, _p1[2] - _p3[2]); // Za  - Zc
  
  a.setIJ(1, 3, d[0]); // Xd
  a.setIJ(2, 3, d[1]); // Yd
  a.setIJ(3, 3, d[2]); // Zd

  double deta = a.det();

  g33Matrix tm = g33Matrix();
  tm.setIJ(1,1, _p1[0] - _p2[0]); // Xa - Xb
  tm.setIJ(2,1, _p1[1] - _p2[1]); // Ya - Yb;
  tm.setIJ(3,1, _p1[2] - _p2[2]); // Za - Zb;

  tm.setIJ(1,2, _p1[0] - _p3[0]); // Xa - Xc
  tm.setIJ(2,2, _p1[1] - _p3[1]); // Ya - Yc
  tm.setIJ(3,2, _p1[2] - _p3[2]); // Za  - Zc

  tm.setIJ(1,3, _p1[0] - e[0]); // Xa - Xe
  tm.setIJ(2,3, _p1[1] - e[1]); // Ya - Ye
  tm.setIJ(3,3, _p1[2] - e[2]); // Za - Ze

  double dett = tm.det();
  double t = dett/deta;
  
  if ((t < t0) || (t > t1))
    return gIntersection();

  g33Matrix g = g33Matrix();
  g.setIJ(1,1, _p1[0] - _p2[0]); // Xa - Xb
  g.setIJ(2,1, _p1[1] - _p2[1]); // Ya - Yb;
  g.setIJ(3,1, _p1[2] - _p2[2]); // Za - Zb;

  g.setIJ(1,2, _p1[0] - e[0]); // Xa - Xe
  g.setIJ(2,2, _p1[1] - e[1]); // Ya - Ye
  g.setIJ(3,2, _p1[2] - e[2]); // Za - Ze

  g.setIJ(1,3, d[0]); // Xd
  g.setIJ(2,3, d[1]); // Yd
  g.setIJ(3,3, d[2]); // Zd
  
  double detg = g.det();
  double gamma = detg/deta;
  
  if ((gamma < 0) || (gamma > 1))
    return gIntersection();

  g33Matrix b = g33Matrix();
  b.setIJ(1,1, _p1[0] - e[0]); // Xa - Xe
  b.setIJ(2,1, _p1[1] - e[1]); // Ya - Ye
  b.setIJ(3,1, _p1[2] - e[2]); // Za - Ze
  
  b.setIJ(1,2, _p1[0] - _p3[0]); // Xa - Xc
  b.setIJ(2,2, _p1[1] - _p3[1]); // Ya - Yc
  b.setIJ(3,2, _p1[2] - _p3[2]); // Za - Zc
  
  b.setIJ(1,3, d[0]); // Xd
  b.setIJ(2,3, d[1]); // Yd
  b.setIJ(3,3, d[2]); // Zd

  double detb = b.det();
  double beta = detb/deta;
  
  if ((beta < 0) || (beta > (1-gamma))) 
      return gIntersection();
  
  gVector u = _p2 - _p1;
  gVector v = _p3 - _p1;
  gVector n = (u.normalize()*v.normalize()).normalize();
  return gIntersection(t, -1, n, true);
}

gPlane::gPlane (gVector n, double d, gMaterial *m) {
  _n = n;
  _d = d;
  _m = m;
}

gIntersection gPlane::intersection(gRay& r) {
  gVector d = r.getDir();
  gPoint p = r.getOrigin();
  gVector p0 = gVector(p[0], p[1], p[2]);

  double ddotn = d.dot(_n);
  double t = 0;

  if (ddotn != 0)
    t = -(p0.dot(_n)+_d)/(d.dot(_n));
  else
    t = 0;

  if (t > 0)
    return gIntersection(t, -1, _n, true);
  else
  return gIntersection();    
}
