#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

using namespace std; 


/* returns 6 times the signed volume of abcd. The volume is positive
   if d is behind abc,  negative if d is in front (i.e. same side
   as the normal) of abc, and 0 if abcd are coplanar.
 */
int signed_area3D(point3d a, point3d b, point3d c, point3d d) {

  return 0; 
}


/* return 1 if points are on the same plane, and 0 otherwise */
int coplanar(point3d a, point3d b, point3d c, point3d d) {

  return 0; 
}


/* return 1 if d is  strictly in front of abc; 0 otherwise */
int infront (point3d a, point3d b, point3d c, point3d d) {

  return 0; 
}



/* compute and return the convex hull of the points */
vector<triangle3d> brute_force_hull(vector<point3d> points) {

  vector<triangle3d> result; 

  return result; 
}

