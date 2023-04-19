#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

using namespace std; 






/* ************************************************************ */
/*  ****************** 2D functions ****************** */
/* ************************************************************ */


/* **************************************** */
/* returns 2 x the signed area of triangle abc */
long long signed_area2d(point2d a, point2d b, point2d c) {
  return  (long long)(2* ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)));
}

/* **************************************** */
/* return true  if a, b, c collinear, and false otherwise */
bool collinear(point2d a, point2d b, point2d c) {
  long long area = signed_area2d(a, b, c);
  //return  (area < EPSILON &&  area > -EPSILON); 
  return (area == 0);
}

/* **************************************** */
/* return True if c is  strictly left of ab; false otherwise */
bool  left_strictly(point2d a, point2d b, point2d c) {
  //return (signed_area2d(a, b, c) > EPSILON);
  return (signed_area2d(a, b, c) > 0);
}

/* return True if c is   left of ab or on ab; false otherwise */
bool  left_on(point2d a, point2d b, point2d c) {
  //return (signed_area2d(a, b, c) > -EPSILON);
  return (signed_area2d(a, b, c) >= 0);
}


/* **************************************** */
/* return true if c is  strictly right  of ab; false otherwise */
bool  right_strictly(point2d a, point2d b, point2d c) {
  //return (signed_area2d(a, b, c) < -EPSILON);
  return (signed_area2d(a, b, c) < 0);
}

/* **************************************** */
long long  dist2d(point2d a, point2d b) {
  return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}







//////////////////////////////////////////////////////////////////////
/*  ****************** 3D functions ****************** */
//////////////////////////////////////////////////////////////////////



long long  det2(long   a1, long   a2,  long  b1,  long  b2) {
  return a1*b2 - a2*b1; 
}

long long  det3( long  a1,  long  a2,  long  a3,
		 long  b1,  long  b2,  long  b3,
		 long   c1,  long  c2,  long  c3) {
  
  long long res;
  res =  (long long)a1* det2(b2, b3, c2, c3);
  res -= (long long) a2*det2(b1, b3, c1, c3);
  res += (long long) a3*det2(b1, b2, c1, c2);
  return res; 
}

/* ************************************************************ */
/* returns 6 times the signed volume of abcd. The volume is positive
   if d is behind abc (i.e. on opposite side as the normal); negative
   if d is in front (i.e. same side as the normal) of abc, and 0 if
   abcd are coplanar.
 */
long long  signed_volume(point3d a, point3d b, point3d c, point3d d) {
  long long res;
  res =  (long long)a.x*det3(b.y, b.z, 1, c.y, c.z, 1, d.y, d.z, 1); 
  res -= (long long) a.y*det3(b.x, b.z, 1, c.x, c.z, 1, d.x, d.z, 1);
  res += (long long)a.z*det3(b.x, b.y, 1, c.x, c.y, 1, d.x, d.y, 1); 
  res -= (long long)det3(b.x, b.y, b.z, c.x, c.y, c.z, d.x, d.y, d.z);
  return res; 
}

/* ************************************************************ */
/* return True if points are on the same plane, and False otherwise */
bool  coplanar(point3d a, point3d b, point3d c, point3d d) {

  long long  vol = signed_volume(a, b, c, d); 
  // return  (vol < EPSILON && vol > -EPSILON); //if using doubles
  return  (vol==0); 
}

/* ************************************************************ */
/* return True if d is  strictly in front of abc; False otherwise */
bool  infront_strictly (point3d a, point3d b, point3d c, point3d d) {

  long long  vol = signed_volume(a, b, c, d); 
  // return  (vol < -EPSILON); //if using doubles
  return  (vol < 0); 
}

/* ************************************************************ */
//return true if face  defined by points (i,j,k) is extreme 
bool face_is_extreme(int i, int j, int k,  vector<point3d>& points) {

  int nfront=0, nback=0; 
  for (int l =0; l< points.size(); l++) {
    if (l==i || l==j || l==k)  continue;
    if (coplanar(points[i], points[j], points[k], points[l])) continue; 
    if (infront_strictly(points[i], points[j], points[k], points[l]))
      nfront++; 
    else  nback++; 
    
    if (nfront*nback >0) return false; 
  }
  //if we got here, all on same side 
  return true;
}





/* compute the convex hull of the points */
void naive_hull(vector<point3d>& points, vector<triangle3d>& hull) {
  
  //your code goes here 
}




/*
//////////////////////////////////////////////////////////////////////

                               GIFT WRAPPING
//////////////////////////////////////////////////////////////////////
*/

//helper functions
void print_point(point3d p, int i) {
  printf("%3d(%d, %d, %d) ", i, p.x, p.y, p.z); 
}

/* ************************************************************ */
//return index of  point with max x-coord 
int find_right_most_point(vector<point3d>& points) {

  if(points.size() == 0) return -1;
  
  int rightmost = 0;
  for (int i=1; i< points.size(); i++){
    if (points[rightmost].x < points[i].x) {
      rightmost = i;
    }
  }
  return rightmost; 
}


  
/* ************************************************************ */
//return true if the edge between points i, j is extreme in the 2d
//projection of all points on the z-plane
bool is_edge_projection_extreme(int first_point, int second_point, vector<point3d>& points) {
  
  point2d first = {points[first_point].x, points[first_point].y};
  point2d second = {points[second_point].x, points[second_point].y}; 
  
  for (int i=0; i<points.size(); i++) {

    if (i==first_point || i==second_point) continue;
    
    point2d p = {points[i].x, points[i].y};
    if (collinear(first, second, p)) continue; 
    
    if (right_strictly(first, second, p)) {
      printf("ERROR first EDGE  NOT extreme\n"); 
      return false; 
    }
  }//for
  return true; 
}



/* ************************************************************ */
//return an edge on the hull 
edge3d find_first_edge_on_hull(vector<point3d>& points) {

  int first_index = find_right_most_point(points);
  printf("%15s", "first point: ");
  print_point(points[first_index], first_index);
  printf("\n"); 

  //project all points onto z=0 plane and 2d gift-wrap to find first edge from first_point
  point2d first = {points[first_index].x, points[first_index].y};
  int second_index = -1;
  point2d second = {0, 0}, p;
  for (int i=0; i<points.size(); i++) {

    if (i==first_index) continue;

    //current point 
    p.x = points[i].x; p.y= points[i].y;

    if (second_index==-1) {
      second_index = i;
      second.x =  points[i].x;
      second.y = points[i].y;
    } else {
      if (right_strictly(first, second, p)) {
	second_index = i;
	second.x = points[i].x;
	second.y = points[i].y;
      }
    }
  }//for
  
  printf("%15s", "second point: ");
  print_point(points[second_index], second_index);
  printf("\n"); 

  //sanity check that edge is indeed extreme 
  assert(is_edge_projection_extreme(first_index, second_index, points)); 
  
  edge3d e = {first_index, second_index, &points[first_index], &points[second_index] }; 
  return e; 
}










/* ************************************************************ */
//p, q are indices of two points, edge (p,q) assumed to be extreme ie on the hull
//returns the index r of  vertex which  is front-most as seen from pq 
int pivot_around_edge(int p, int  q, vector<point3d>& points) {

  printf("pivot around edge (%d, %d): \n", p, q); 

  int r = -1; 
  //find the front-most point
  for (int i=0; i< points.size(); i++) {
    if (i==p || i==q ) continue;
    
    if (r == -1) {
     r = i; 
    } else {
      if (infront_strictly(points[p],  points[q], points[r],  points[i])) {
	//this point is more infront than r
	//printf("\tr=%d. point %d in front  of %d, swapping\n", r, i, r); 
	r = i;
      }
    }//else 
  }//for 
  
  //sanity check
  if (!face_is_extreme(p, q, r, points)) {
    printf("pivot_around_edge: returns a face [%d,%d,%d] that's NOT EXTREME\n", p, q, r);
    assert(face_is_extreme(p, q, r, points)); 
  }
  
  return r; 
}



/* ************************************************************ */
//orient so that the normal is outside the hull 
void orient_triangle(triangle3d *t, vector<point3d>& points) {

  //YOUR CODE GOES HERE
}


/* ************************************************************ */
triangle3d find_first_face(vector<point3d>& points) {
  
  edge3d e = find_first_edge_on_hull(points);
  int first_point = e.ia;
  int second_point = e.ib;
    
  //first_point and second_point are both on the hull. Find the third point similarly.
  int third_point = pivot_around_edge(first_point, second_point, points);
  
  printf("%15s", "third point: ");
  print_point(points[third_point], third_point);
  printf("\n"); 

  triangle3d t;
  t.a = &points[first_point];
  t.b = &points[second_point];
  t.c = &points[third_point];

  //if indices are stored 
  t.ia = first_point;
  t.ib = second_point;
  t.ic = third_point;
  
  orient_triangle(&t, points); 
  return t; 
}





/* compute the convex hull of the points */
void giftwrapping_hull(vector<point3d>& points, vector<triangle3d>& hull) {

   hull.clear(); //to be safe
   
  //your code goes here 
} 







/*
//////////////////////////////////////////////////////////////////////

                               INCREMENTAL 
//////////////////////////////////////////////////////////////////////
*/

/* compute the convex hull of the points */
void incremental_hull(vector<point3d>& points, vector<triangle3d> & hull) {
  
  //your code goes here 
} 
