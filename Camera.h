#ifndef CAMERA
#define CAMERA
#include "math/Vector3.h"
#include "math/Utility.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#endif

//Enumeration used in some camera functions
enum Direction {UP, DOWN, LEFT, RIGHT};

/* Class Camera
 * Contains the necesary information about the camera
 */
class Camera
{
 private:
  Vector3 pos, dir;
  GLfloat angleXZ, angleXY, r;

 public:
  Camera(void);
  Camera(Vector3 dest, GLfloat angleXZ, GLfloat angleXY, GLfloat r);
  void setUp(void);
  Vector3 getPos(void);
  Vector3 getDir(void);
  void setPos(Vector3 pos);
  void setDir(Vector3 dir);
  void rotate(Direction d, GLfloat delta);
  void move(Direction d, GLfloat delta);
};
#endif
