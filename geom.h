#ifndef __geom_h
#define __geom_h


#include <vector>

using namespace std; 


//note that coordinates are, for now, integers
typedef struct _point3d {
  double x,y,z; 
} point3d;


typedef struct _triangle3d {
  point3d *a,*b,*c; 
  //to avoid duplication a triangle stores pointers to the points
  //(which are stored in the vector of points)
} triangle3d;


/* returns 6 times the signed volume of abcd. The volume is positive
   if d is behind abc (i.e. on opposite side as the normal); negative
   if d is in front (i.e. same side as the normal) of abc, and 0 if
   abcd are coplanar.
 */
double signed_volume(point3d a, point3d b, point3d c, point3d d);


/* return True if points are on the same plane, and False otherwise */
bool coplanar(point3d a, point3d b, point3d c, point3d d);


/* return True if d is  strictly in front of abc; False otherwise */
bool infront (point3d a, point3d b, point3d c, point3d d); 


/* compute the convex hull of the points */
void naive_hull(vector<point3d>& points, vector<triangle3d>& hull);
void incremental_hull(vector<point3d>& points, vector<triangle3d> & hull);
void giftwrapping_hull(vector<point3d>& points, vector<triangle3d>& hull);


#endif
