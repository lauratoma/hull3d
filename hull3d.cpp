/* hull3d.cpp

   What it does: Renders  a set of points in 3D allowing to
   translate and rotate.

   This code is provided as a startup for your 3d hull.  Change it as
   needed to work with your project. 

   OpenGL 1.x
   Laura Toma
*/

#include "geom.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

//this allows this code to compile both on apple and linux platforms
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <vector>

//to read the mesh from a file 
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>

using namespace std; 



//pre-defined colors for convenience 
GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};

GLfloat darkmagenta[3] = { 0.5, 0.0, 0.5};
GLfloat lightgrey[3] = { 0.8, 0.8, 0.8};
GLfloat darkgrey[3] = { 0.6, 0.6, 0.6};
GLfloat darkcyan[3] = { 0.0, 0.4, 0.4};
GLfloat lightcyan[3] = { 0.0, 0.6, 0.6};
GLfloat LimeGreen[3] ={ 0.196078,  0.8 , 0.196078}; 
GLfloat Orange[3] = { 1, .5, 0}; 
GLfloat Silver[3] = { 0.90, 0.91, 0.98};
GLfloat Wheat[3] = { 0.847059 , 0.847059, 0.74902}; 





/********************************************************************/
/* global variables */
/********************************************************************/

int n;  //desired number of points

//the vector  of n points;
//note: needs to be global in order to be rendered
vector<point3d>  points;

//the convex hull
//note: needs to be global in order to be  rendered
vector<triangle3d>  hull; 


///window size for the graphics window
const int WINDOWSIZE = 500; 


//global translation matrix
//updated when the user translates the scene 
GLfloat pos[3] = {0,0,0};

//global rotation matrix
//updated when the user translates the scene 
GLfloat theta[3] = {0,0,0};


// render mode: draw polygons line or filled.  
GLint fillmode = 0; 






/********************************************************************/
/* forward declarations of functions */
/********************************************************************/

void display(void);
void keypress(unsigned char key, int x, int y);

//initializers
void initialize_points_random(vector<point3d>& points, int n);
void initialize_points_sphere(vector<point3d>& points, int n, double rad);
int initialize_points_from_mesh(vector<point3d>& pts, char* fpath); 
				
//print label, then the vector 
void print_vector(const char* label, vector<point3d> p);

/* render the points; each point is drawn as a small square.  */
void draw_points(vector<point3d> pts);

/* render the hull. note: for each triangle, the points are expected to
   be in ccw boundary order. 
*/
void draw_hull(vector<triangle3d> hull);


//various helper functions 
void draw_xy_rect(GLfloat z, GLfloat* col); 
void draw_xz_rect(GLfloat y, GLfloat* col); 
void draw_yz_rect(GLfloat x, GLfloat* col); 
void cube(GLfloat side); 
void filledcube(GLfloat side); 
void draw_axes(); 

//you'll add more





/* ************************************************************ */
int main(int argc, char** argv) {

  printf("hello!");
  //read number of points from user
  if (argc!=2) {
    printf("usage: hull3d <nbPoints>\n");
    exit(1); 
  }
  n = atoi(argv[1]); 
  printf("you entered n=%d\n", n);
  assert(n>0); 

  //populate the points 
  initialize_points_random(points, n);
  //initialize_points_sphere(points, n, .8);
  //print_vector("points:", points);

  //compute the hull 
  naive_hull(points, hull); 
  //print_hull(hull);

  
  /* OPEN GL STUFF */
    /* initialize GLUT and open a window */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display); 
  glutKeyboardFunc(keypress);
  
  /* GL init */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);  
  //when depth test is enabled, GL determines which objects are in
  //front/behind and renders them correctly
  glEnable(GL_DEPTH_TEST); 

  // setup the projection  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1 /* aspect */, 1, 10.0);
  /* the frustrum is from z=-1 to z=-10 */
  /* camera is at (0,0,0) looking along negative y axis */
  
  //initialize the translation to bring the points in the view
  //frustrum which is [-1, -10]
  pos[2] = -3;


  /* give control to the event handler */
  glutMainLoop();

  return 0;
}




/* ************************************************************ */
/* This is the function that renders the window. We registered this
   function as the "displayFunc". It will be called by GL everytime
   the window needs to be rendered.
 */
void display(void) {
  
  //clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* The default GL window is x=[-1,1], y= [-1,1] with the origin in
     the center.  The view frustrum was set up from z=-1 to z=-10. The
     camera is at (0,0,0) looking along negative z axis.
  */

  
  //clear all modeling transformations
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();

  /*  First we translate and rotate our local reference system with the
      user transformation. pos[] represents the cumulative translation
      entered by the user, and theta[] the cumulative rotation entered
      by the user */
  glTranslatef(pos[0], pos[1], pos[2]);  
  glRotatef(theta[0], 1,0,0); //rotate theta[0] around x-axis, etc 
  glRotatef(theta[1], 0,1,0);
  glRotatef(theta[2], 0,0,1);
   

  // now we draw the objects in the local reference system. 
  
  //don't need to draw a cube but nice for perspective 
  cube(1); 

  // The points are in the range [-1, 1]
  draw_points(points);
  draw_hull(hull); 
  
  // execute the drawing commands
  glFlush();
}



/* ************************************************************ */
/* This is the function that handles key presses, We registered this
   function as the keypressFunc.  It will be called by GL whenever a
   key is pressed */
void keypress(unsigned char key, int x, int y) {
  
  switch(key) {
    
  case 'i': 
    //re-initialize  RANDOM
    initialize_points_random(points, n); 
    //re-compute the hull
    naive_hull(points, hull);
    //gift_wrapping_hull(points, hull); 
    glutPostRedisplay(); 
    break; 
    
  case 's': 
    //re-initialize SPHERE 
    initialize_points_sphere(points, n, .8); 
    //re-compute the hull
    naive_hull(points, hull);
    //gift_wrapping_hull(points, hull); 
    glutPostRedisplay(); 
    break; 
    
  case 'm':
    //re-initialize from mesh 
    if (initialize_points_from_mesh(points, "./meshes/spot/spot_control_mesh.obj")) {
      //re-compute the hull
      naive_hull(points, hull);
      //gift_wrapping_hull(points, hull); 
      glutPostRedisplay(); 
    }
    break;
    
    
  case 'c': 
    //fillmode
    fillmode = !fillmode; 
    glutPostRedisplay();
    break;
    
    
    //ROTATIONS
  case 'x':
    theta[0] += 5.0; 
    glutPostRedisplay();
    break;
  case 'y':
    theta[1] += 5.0;
    glutPostRedisplay();
    break;
  case 'z':
    theta[2] += 5.0;
    glutPostRedisplay();
    break;
  case 'X':
    theta[0] -= 5.0; 
    glutPostRedisplay();
    break;
  case 'Y':
    theta[1] -= 5.0; 
    glutPostRedisplay();
    break;
  case 'Z':
    theta[2] -= 5.0; 
    glutPostRedisplay();
    break;

    //TRANSLATIONS 
    //backward (zoom out)
  case 'b':
    pos[2] -= 0.1; 
    glutPostRedisplay();
    break;
    //forward (zoom in)
  case 'f':
    pos[2] += 0.1; 
    //glTranslatef(0,0, 0.5);
    glutPostRedisplay();
    break;
    //down 
  case 'd': 
     pos[1] -= 0.1; 
    //glTranslatef(0,0.5,0);
    glutPostRedisplay();
    break;
    //up
  case 'u': 
    pos[1] += 0.1; 
    //glTranslatef(0,-0.5,0);
    glutPostRedisplay();
    break;
    //left 
  case 'l':
    pos[0] -= 0.1; 
    glutPostRedisplay();
    break;
    //right
  case 'r':
    pos[0] += 0.1; 
    glutPostRedisplay();
    break;


  case '0':
    //reset all transformations (back to original position)
    pos[0] = pos[1] = 0; pos[2] = -3;
    theta[0] = theta[1] = theta[2] = 0;
    glutPostRedisplay();
    break;
    
  case 'q':
    exit(0);
    break;
  } 

}//keypress









/* ************************************************************** */
/* populate the vector  with n random points in the range [-1, 1] 
 */
void initialize_points_random(vector<point3d>& points, int n) {
  
  printf("initialize points random\n"); 
  //clear the vector just to be safe 
  points.clear(); 

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(-.8, .8);
  point3d p;
  for (int i=0; i<n; i++) {
    p.x = dist(gen); 
    p.y =  dist(gen);
    p.z=  dist(gen);
    //cout << p.x << ","<< p.y << "," <<  p.z << endl; 
    points.push_back(p); 
  }
}



/* ************************************************************** */
/* initialize n random points on a sphere of radius rad

   spherical coordinates: 
   x = r sin PHI cos THETA
   y = r sin PHI sin THETA 
   z = r cos PHI
*/
void initialize_points_sphere(vector<point3d>& points, int n, double rad) {

  printf("initialize points sphere\n"); 
  //clear the vector just to be safe 
  points.clear(); 

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(0, 2*M_PI);
  
  double phi, theta;
  point3d p;
  for (int i=0; i<n; i++) {
    phi = dist(gen); 
    theta = dist(gen);
    p.x = rad * sin(phi) * cos(theta);
    p.y = rad * sin(phi) * sin(theta);
    p.z = rad * cos(phi); 
    points.push_back(p); 
  }
}




/* ************************************************************ */
/* credit: Juan Atehortua and Lily Smith, fall 2021 */
int initialize_points_from_mesh(vector<point3d>& pts, char* fpath) {
  printf("initialize points mesh\n"); 
  pts.clear();
  
  string line;
  ifstream mesh;
  //mesh.open("./meshes/spot/spot_control_mesh.obj", ios::in);
  mesh.open(fpath, ios::in);
  point3d point;
  if (mesh.is_open()) {
    while (getline(mesh, line)) {
      if (line.substr(0, 2) == ("v ")){
        istringstream v(line.substr(2));
        v >> point.x;
        v >> point.y;
        v >> point.z;
        pts.push_back(point);
      }//if
    }//while 
  } else {
    cerr << "Could not open file.\n";
    return 0;
  }
  mesh.close();
  return 1;
}

  
  
/* ******************************************************** */
/* Draw the array of points, Each point is drawn as a small cube.
   The points are in the range x, y, z in [-1,1],
*/
  void draw_points(vector<point3d> pts){

  //filled 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //set color 
  glColor3fv(yellow);   
  
  for (int i=0; i < points.size(); i++) {
    
    //draw small filled cube at (points[i].x, points[i].y, points[i].z)
    
    //first save local coordinate system 
    glPushMatrix(); 
    //translate our local coordinate system to the point that we want to draw
    glTranslatef(points[i].x, points[i].y, points[i].z);
    //draw the cube 
    filledcube(.005); 
    //go  back to where we were
    glPopMatrix(); 
  } //for 
  
}//draw_points
  
  
  
  /* ********************************************************* */
  // draw the faces of the hull.
  void draw_hull(vector<triangle3d> hull){

    if (hull.size() == 0) return;
    for (int i=0; i< hull.size()-1; i++) {
      
      //draw the triangle
      
    }
  }//draw-hull 







//draw a square x=[-side,side] x y=[-side,side] at depth z
void draw_xy_rect(GLfloat z, GLfloat side, GLfloat* color) {

  glColor3fv(color);
  glBegin(GL_POLYGON);
  glVertex3f(-side,-side, z);
  glVertex3f(-side,side, z);
  glVertex3f(side,side, z);
  glVertex3f(side,-side, z);
  glEnd();
}


//draw a square y=[-side,side] x z=[-side,side] at given x
void draw_yz_rect(GLfloat x, GLfloat side, GLfloat* color) {
  
  glColor3fv(color);
  glBegin(GL_POLYGON);
  glVertex3f(x,-side, side);
  glVertex3f(x,side, side);
  glVertex3f(x,side, -side);
  glVertex3f(x,-side, -side);
  glEnd();
}


//draw a square x=[-side,side] x z=[-side,side] at given y
void draw_xz_rect(GLfloat y, GLfloat side, GLfloat* color) {

  glColor3fv(color);
  glBegin(GL_POLYGON);
  glVertex3f(-side,y, side);
  glVertex3f(-side,y, -side);
  glVertex3f(side,y, -side);
  glVertex3f(side,y, side);
  glEnd();
}




//draw a cube with middle planes 
void cube(GLfloat side) {
  GLfloat f = side, b = -side;
 
  if (fillmode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }


  /* back face  BLUE*/
  draw_xy_rect(b,side, lightcyan);
 /* front face  RED*/
  //draw_xy_rect(f,side, red);
  /* side faces  GREEN*/
  draw_yz_rect(b, side, darkcyan);
  draw_yz_rect(f, side, darkcyan);
  //up, down faces missing to be able to see inside 

  /* middle z=0 face CYAN*/
  draw_xy_rect(0, side, lightgrey);
  /* middle x=0 face WHITE*/
  draw_yz_rect(0,side, darkgrey);
  /* middle y=0 face  pink*/
  draw_xz_rect(0, side, darkmagenta);
}



//draw a filled cube [-side,side]^3
void filledcube(GLfloat side) {
  
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  /* back, front faces */
  draw_xy_rect(-side,side, yellow);
  draw_xy_rect(side,side, yellow);
  
  /* left, right faces*/
  draw_yz_rect(-side, side, yellow);
  draw_yz_rect(side, side, yellow);
  
  /* up, down  faces  */
  draw_xz_rect(side,side, yellow);
  draw_xz_rect(-side,side, yellow);
}


  

/* ****************************** */
/* print the vector of points */
void print_vector(const char* label, vector<point3d> points) {
  
  printf("%s ", label);
  for (int i=0; i< points.size(); i++) {
    printf("[%.1f,%.1f, %.1f] ", points[i].x, points[i].y, points[i].x);
  }
  printf("\n");
}

