#include <iostream>
#include <stdlib.h>
#include "raytra.h"
#include "parse.h"
#include "gShape.h"

gRay::gRay() {
  _origin = gPoint();
  _dir = gVector();
}

gRay::gRay (gPoint& origin1, gPoint& p1, gPoint& p2) {
  _origin = origin1;
  _dir = (p2 - p1).normalize();
}

gRay::gRay (gPoint& origin1, gVector& dir1) {
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
  _arl = NULL;
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
    delete _m;
    _m = NULL;
  }

  if (_arl) {
    list <gARLight *>::iterator l;
    for (l = (*_arl).begin(); l != (*_arl).end(); ++l) {
      delete *l;
      *l = NULL;
    }
    delete _arl;
    _arl = NULL;
  }
}

gRay gCamera::compute_ray(int i, int j, int p, int q, int n) {
  double r = _width/2.0;
  double l = -1 * r;
  double t = _height/2.0;
  double b = -1 *t;
  double u1 = 0;
  double v1 = 0;
  if ((p != 0) && (q != 0) && (n != 0)) {
    double pp = (p+((double) rand()/RAND_MAX))/n;
    double qq = (q+((double) rand()/RAND_MAX))/n;
    u1 = l + (r-l)*(i+pp)/_nx;
    v1 = t + (b-t)*(j+qq)/_ny;
  } else {
    u1 = l + (r-l)*(i+.5)/_nx;
    v1 = t + (b-t)*(j+.5)/_ny;
  }
  gVector d = _w*-1*_d;
  d = d + (u1*_u);
  d = d + (v1*_v);
  gVector dir = ((-_d*_w)+(u1*_u)+(v1*_v)).normalize();
  return gRay(_eye, dir);
}



void gCamera::render(const char *file, int primary_ray, int shadow_ray, int bbox) {
  Array2D<Rgba> p(_ny, _nx);
  gLight *l = NULL;
  gVector color = gVector();
  for (int y = 0; y < _ny; ++y) {
    for (int x = 0; x < _nx; ++x) {
      Rgba &px = p[y][x];  
      color = gVector();
      if ((primary_ray == 1) && (shadow_ray == 1)) {
        gRay r = compute_ray(x, y, 0, 0, 0);
        color = (color + L(r, 0.0001, 100000.0, 3, 0, gVector(), l, 1, bbox));
      } else if ((primary_ray > 1) && (shadow_ray >= 1)) {
        for (int p = 0; p < primary_ray; p++) {
          for (int q = 0; q < primary_ray; q++) {	  
            gRay r = compute_ray(x, y, p, q, primary_ray);
            color = (color + L(r, 0.0001, 100000.0, 3, 0, gVector(), l, shadow_ray, bbox));
          }
        }
        color = (1.0/(primary_ray*primary_ray))*color;
      }
      px.r = color[0];
      px.g = color[1];
      px.b = color[2];
      px.a = 1;
    }
  }
  (*this).writeRgba(&p[0][0], file);
}

gVector gCamera::L(gRay r, double tmin, double tmax, int recurse_limit, int type, gVector c, gLight *light, int shadow_ray, int bbox) {
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
    gIntersection in = (*(*s)).intersection(r, bbox);
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
    gVector shadow = L(sray, tmin, tmax, 1, 1, c, *l, shadow_ray, bbox);
    if (shadow.length() == 0)
      return gVector();
    gVector pp = gVector(p[0], p[1], p[2]);
    gVector n = bestI.getNormal();
    gVector d = r.getDir();      
    c = c + (*(*l)).shading(m, n, pp, d, _s);
  }

  /*  list <gARLight *>::iterator arl;
  for (arl = (*_arl).begin(); arl != (*_arl).end(); ++arl) {
    gRay srays[shadow_ray*shadow_ray];
    double tmax[shadow_ray*shadow_ray];
    int x = 0;
    for (int i = 0; i < shadow_ray; i++) {
      for (int j = 0; j < shadow_ray; j++) {
        double ii = (i + ((double)rand()/RAND_MAX)/shadow_ray);
        double jj = (j + ((double)rand()/RAND_MAX)/shadow_ray);
        gVector u = (ii*(*(*arl)).getUDir()).normalize();
        gVector v = (jj*(*(*arl)).getVDir()).normalize();
        gPoint p = (*(*arl)).getPos();
        gPoint pos = gPoint(p[0] + u[0] + v[0], p[1] + u[1] + v[1], p[2] + u[2] + v[2]);
        gVector ll = (gVector(pos[0], pos[1], pos[2]) - gVector(p[0], p[1], p[2])).normalize();
        gRay sray = gRay(p, ll);
        srays[x] = sray;
        gVector posminusp = gVector(pos[0]-p[0], pos[1]-p[1], pos[2]-p[2]);
        tmax[x] = posminusp.dot(ll);      
        x++;
      }
    }            
    for (int x = shadow_ray-1; x > 0; x--) {
      int y = rand()%(shadow_ray)+1;
      gRay sray = srays[y];
      double t = tmax[y];
      srays[y] = srays[x];
      srays[x] = sray;
      tmax[y] = tmax[x];
      tmax[x] = t;
      }
    for (int x = 0; x < shadow_ray; x++) {
      gVector shadow = L(srays[x], tmin, tmax[x], 1, 1, c, *arl, shadow_ray, bbox);
      if (shadow.length() == 0)
        continue;
      gVector pp = gVector(p[0], p[1], p[2]);
      gVector n = bestI.getNormal();
      gVector d = r.getDir();      
      c = c + (*(*arl)).shading(m, n, pp, d, _s);
    }
    }

    c = (1.0/shadow_ray)*c;*/
  gVector zero = gVector();
  c = c + _al.shading(m, zero, zero, zero, _s);

  if (m.getIdeal().length() == 0) 
    return c;
  else {
    double ddotn = (r.getDir()).dot(bestI.getNormal());
    gVector refv = r.getDir() - (2*ddotn*(bestI.getNormal()));
    gRay refr = gRay(p, refv);
    gVector rc = L(refr, .0001, tmax, recurse_limit - 1, 0, c, light, shadow_ray, bbox);
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

void gCamera::setARL(list<gARLight *> *l) {
  _arl = l;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "error: pass a scene as the first argument, and an output file as the second" << endl;
    return 1;
  }

  Parser parser;
  gCamera *g = parser.parse(argv[1]);  

  if (argc == 6) {
    (*g).render(argv[2], atoi(argv[3]), atoi(argv[4]), 1);
  }
  else if (argc == 5) {
    (*g).render(argv[2], atoi(argv[3]), atoi(argv[4]), 0);
  } else if (argc == 4) {
    (*g).render(argv[2], atoi(argv[3]), atoi(argv[4]), 0);
  }
  else {
    (*g).render(argv[2], 3, 1, 0);
  }

  cout << "output in " << argv[2] << endl;
  delete g;
}
