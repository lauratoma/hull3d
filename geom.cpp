#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

using namespace std; 

/* returns 6 times the signed volume of abcd. The volume is positive
   if d is behind abc (i.e. on opposite side as the normal); negative
   if d is in front (i.e. same side as the normal) of abc, and 0 if
   abcd are coplanar.
 */
double signed_volume(point3d a, point3d b, point3d c, point3d d) {

  return 0; 
}


/* return True if points are on the same plane, and False otherwise */
bool  coplanar(point3d a, point3d b, point3d c, point3d d) {

  return 0; 
}


/* return True if d is  strictly in front of abc; False otherwise */
bool  infront (point3d a, point3d b, point3d c, point3d d) {

  return 0; 
}




/* compute the convex hull of the points */
void naive_hull(vector<point3d>& points, vector<triangle3d>& hull) {
  
  //your code goes here 
}

/* compute the convex hull of the points */
void incremental_hull(vector<point3d>& points, vector<triangle3d> & hull) {
  
  //your code goes here 
} 

/* compute the convex hull of the points */
void giftwrapping_hull(vector<point3d>& points, vector<triangle3d>& hull) {
  
  //your code goes here 
} 



