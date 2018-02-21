#ifndef __geom_h
#define __geom_h


#include <vector>

using namespace std; 

typedef struct _point3d {
  int x,y,z; 
} point3d;

typedef struct _triangle3d {
  point3d *a,*b,*c; 
  //to avoid duplication of points, a triangle stores pointers to the
  //points (which are stored in the vector of points)
} triangle3d;


/* returns 6 times the signed volume of  abcd. The volume is positive if d
   is behind abc, and negative if d is in front (i.e. same side as the normal) of abc
 */
int signed_area3D(point3d a, point3d b, point3d c, point3d d);

/* return 1 if points are on the same plane, and 0 otherwise */
int coplaner(point3d a, point3d b, point3d c, point3d d);


/* return 1 if d is  strictly in front of abc; 0 otherwise */
int infront (point3d a, point3d b, point3d c, point3d d); 


/* compute and return the convex hull of the points */
vector<triangle3d> brute_force_hull(vector<point3d> points);


#endif
