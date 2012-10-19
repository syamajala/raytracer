#include "parse.h"
#include "gShape.h"
#include "gLight.h"
#include "raytra.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <list>
#include <vector>
using namespace std;

gCamera* Parser::parse(const char *file) {

  ifstream in(file);
  char buffer[1025];
  string cmd;

  list<gShape *> *s = new list<gShape *>;
  list<gMaterial *> *m = new list<gMaterial *>;
  list<gLight *> *l = new list<gLight *>;
  gALight al;
  gCamera *c = NULL;
  for (int line=1; in.good(); line++) {
    in.getline(buffer,1024);
    buffer[in.gcount()]=0;

    cmd="";

    istringstream iss(buffer);
    
    iss >> cmd;
    if (cmd[0]=='/' or cmd.empty()) {
      continue;
    } else if (cmd=="s") {
      gVector pos; double r;
      iss >> pos >> r;
      (*s).push_front(new gSphere(pos,r, (*m).back()));
    } else if (cmd=="t") {
      gPoint a,b,c;
      iss >> a >> b >> c;
      (*s).push_front(new gTriangle(a, b, c, (*m).back()));
    } else if (cmd=="p") {
      gVector n; double d;
      iss >> n >> d;
      (*s).push_front(new gPlane(n,d, (*m).back())); 
    } else if (cmd=="c") {
      gPoint pos; gVector dir; double d,iw,ih; int pw,ph;
      iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
      c = new gCamera(pos,dir,d,iw,ih,pw,ph);
    } else if (cmd=="l") {
      iss >> cmd;
      if (cmd=="p") {
	gPoint pos; gVector color;
	iss >> pos >> color;
	(*l).push_front(new gPLight(pos, color));
      } else if (cmd=="d") {
	//gVector dir,rgb;
	//iss >> dir >> rgb;
	//	directionalLight(dir,rgb);
      } else if (cmd=="a") {
	gVector color;
	iss >> color;
	al = gALight(color);
      } else {
	cout << "Parser error: invalid light at line " << line << endl;
      }
    } else if (cmd=="m") {
      gVector diff,spec,ideal; double r;
      iss >> diff >> spec >> r >> ideal;
      (*m).push_back(new gMaterial(diff,spec,r,ideal));
    } else if (cmd=="w") {
      char f[1025];
      iss >> f;
      (*this).read_wavefront_file(f, s, (*m).back());
    } else {
      cout << "Parser error: invalid command at line " << line << endl;
    }
  }
  (*c).setShapes(s);
  (*c).setAL(al);
  (*c).setLights(l);
  (*c).setMaterials(m);
  in.close();
  return c;
}


// Given the name of a wavefront (OBJ) file that consists JUST of
// vertices, triangles, and comments, read it into the tris and verts
// vectors.
//
// tris is a vector of ints that is 3*n long, where n is the number of
// triangles. The ith triangle has vertex indexes 3*i, 3*i+1, and 3*i+2.
//
// The ith triangle has vertices verts[3*i], verts[3*i+1], and verts[3*i+2],
// given in counterclockwise order with respect to the surface normal
//
// If you are using the supplied Parser class, you should probably make this
// a method on it: Parser::read_wavefront_file().
//
void Parser::read_wavefront_file (const char *file, list<gShape*>*s, gMaterial *m) {
  vector<gPoint> verts;
  ifstream in(file);
  char buffer[1025];
  string cmd;
        
  for (int line=1; in.good(); line++) {
    in.getline(buffer,1024);
    buffer[in.gcount()]=0;
                
    cmd="";
        
    istringstream iss (buffer);
        
    iss >> cmd;
        
    if (cmd[0]=='#' or cmd.empty()) {
      // ignore comments or blank lines
      continue;
    } 
    else if (cmd=="v") {
      gPoint a;
      iss >> a;
      verts.push_back (a);
    } 
    else if (cmd=="f") {
      // got a face (triangle)
            
      // read in the parameters:
      int i, j, k;
      iss >> i >> j >> k;
      (*s).push_front(new gTriangle(verts[i-1], verts[j-1], verts[k-1], m));
    } 
    else {
      std::cerr << "Parser error: invalid command at line " << line << std::endl;
    }
        
  }
    
  in.close();
  
  //   std::cout << "found this many tris, verts: " << tris.size () / 3.0 << "  " << verts.size () / 3.0 << std::endl;
}
