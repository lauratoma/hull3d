#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

using namespace std; 



/* returns 2 times the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area3D(point3d a, point3d b, point3d c, point3d d) {

  return 1; 
}

/* return 1 if p,q,r, t on same plane, and 0 otherwise */
int collinear(point3d p, point3d q, point3d r, point3d t) {

  return 1; 
}


/* return 1 if d is  strictly left of abc; 0 otherwise */
int left (point3d a, point3d b, point3d c, point3d d) {

  return 1; 
}


/* compute and return the convex hull of the points */
vector<triangle3d> brute_force_hull(vector<point3d> points) {

  vector<triangle3d> result; 

  return result; 
}

