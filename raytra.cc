#include <iostream>
#include "raytra.h"
#include "parse.h"
#include "gShape.h"

gRay::gRay() {
  _origin = gPoint();
  _dir = gVector();
}

gRay::gRay (gPoint origin1, gPoint p1, gPoint p2) {
  _origin = origin1;
  _dir = (p2 - p1).normalize();
}

gRay::gRay (gPoint origin1, gVector dir1) {
  _origin = origin1;
  _dir = dir1.normalize();
}

gRay::gRay (const gRay& r) {
  _origin = r._origin;
  _dir = r._dir;
}

gPoint gRay::getOrigin () {
  return _origin;
}

gVector gRay::getDir () {
  return _dir;
}

gRay gCamera::compute_ray(int i, int j) {
  double r = _width/2.0;
  double l = -1 * r;
  double t = _height/2.0;
  double b = -1 *t;

  double u1 = l + (r-l)*(i+.5)/_nx;
  double v1 = t + (b-t)*(j+.5)/_ny;
  gVector d = _w*-1*_d;
  d = d + (u1*_u);
  d = d + (v1*_v);
  return gRay(_eye, ((-_d*_w)+(u1*_u)+(v1*_v)).normalize());
}

gCamera::gCamera (gPoint pos, gVector dir, double d1, double iw, double ih, int pw, int ph) {
  _eye = pos;
  _d = d1;
  _w = (-1*dir).normalize();
  _u = (gVector(0, 1, 0)*_w).normalize();
  _v = (_w*_u).normalize();
  _nx = pw;
  _ny = ph;
  _s = NULL;
  _l = NULL;
  _m = NULL;
  _height = ih;
  _width = iw;
}

gCamera::~gCamera() {
  if (_s) {
    list <gShape *>::iterator i;
    for (i = (*_s).begin(); i != (*_s).end(); ++i) {
      delete *i;
      *i = NULL;
    }
    delete _s;
    _s = NULL;
  }
  
  if (_l) {
    list <gLight *>::iterator j;
    for (j = (*_l).begin(); j != (*_l).end(); ++j) {
      delete *j;
      *j = NULL;
    }
    delete _l;
    _l = NULL;
  }
  
  if (_m) {
    list <gMaterial *>::iterator k;
    for (k = (*_m).begin(); k != (*_m).end(); ++k) {
      delete *k;
      *k = NULL;
    }
    _m = NULL;
  }
}

void gCamera::render(const char *file) {
  Array2D<Rgba> p(_ny, _nx);
  gLight *l = NULL;
  for (int y = 0; y < _ny; ++y) {
    for (int x = 0; x < _nx; ++x) {
      gRay r = compute_ray(x, y);
      Rgba &px = p[y][x];  
      gVector color = L(r, 0.0001, 100000.0, 3, 0, gVector(), l);
      px.r = color[0];
      px.g = color[1];
      px.b = color[2];
      px.a = 1;
    }
  }
  (*this).writeRgba(&p[0][0], file);
}

gVector gCamera::L(gRay r, double tmin, double tmax, int recurse_limit, int type, gVector c, gLight *light) {
  if (recurse_limit == 0)
    return gVector();

  if (type == 1) {
    if ((*light).shadow(r, _s, tmin, tmax))
      return gVector();
    return (*light).getColor();
  }

  gShape *surface = NULL;
  gIntersection bestI;
  list <gShape *>::iterator s;
  for (s = (*_s).begin(); s != (*_s).end(); ++s) {	
    gIntersection in = (*(*s)).intersection(r);
    if (in.intersected() && (in.getBestT() > tmin) && (in.getBestT() < tmax)) {
      if(!surface) {
	surface = *s;
	bestI = in;
      } else if (bestI.getBestT() > in.getBestT()) {
	surface = *s;
	bestI = in;
      }
    } 	  
  }
  
  if (!surface)
    return gVector();
  
  gMaterial m = (*surface).getMaterial();    
  gPoint p = r.getOrigin() + (bestI.getBestT()*r.getDir());
  list <gLight *>::iterator l;
  for (l = (*_l).begin(); l != (*_l).end(); ++l) {
    gPoint pos = (*(*l)).getPos();
    gVector ll = (gVector(pos[0], pos[1], pos[2]) - gVector(p[0], p[1], p[2])).normalize();
    gRay sray = gRay(p, ll);
    gVector posminusp = gVector(pos[0]-p[0], pos[1]-p[1], pos[2]-p[2]);
    double tmax = posminusp.dot(ll);
    gVector shadow = L(sray, tmin, tmax, 1, 1, c, *l);
    if (shadow.length() == 0)
      return gVector();
    gVector pp = gVector(p[0], p[1], p[2]);
    c = c + (*(*l)).shading(m, bestI.getNormal(), pp, r.getDir(), _s);      
  }

  c = c + _al.shading(m, gVector(), gVector(), gVector(), _s);

  if (m.getIdeal().length() == 0) 
    return c;
  else {
    double ddotn = (r.getDir()).dot(bestI.getNormal());
    gVector refv = r.getDir() - (2*ddotn*(bestI.getNormal()));
    gRay refr = gRay(p, refv);
    gVector rc = L(refr, .0001, tmax, recurse_limit - 1, 0, c, light);
    return (c+gVector(m.getIdeal()[0]*rc[0], m.getIdeal()[1]*rc[1], m.getIdeal()[2]*rc[2]));    
  }
}

void gCamera::writeRgba (const Rgba *pixels, const char *f) {
  RgbaOutputFile file (f, _nx, _ny, WRITE_RGBA); // 1
  file.setFrameBuffer (pixels, 1, _nx); // 2
  file.writePixels (_ny); // 3
}

void gCamera::setShapes(list<gShape *> *s) {
  _s = s;
}

void gCamera::setLights(list<gLight *> *l) {
  _l = l;
}

void gCamera::setMaterials(list<gMaterial *> *m) {
  _m = m;
}

void gCamera::setAL(const gALight al) {
  _al = al;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "error: pass a scene as the first argument, and an output file as the second" << endl;
    return 1;
  }
  
  Parser parser;
  gCamera *g = parser.parse(argv[1]);  
  (*g).render(argv[2]);  
  cout << "output in " << argv[2] << endl;
  delete g;
}
