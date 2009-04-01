#ifndef SPHERE
#define SPHERE
#include "../math/Vector3.h"
#include "GL/glut.h"

/* Class Sphere
 *  Contains all the necesary info about the spheres
 */
class Sphere
{
 private:
  //Position and direction
  Vector3 pos,vel;
  //Velocity, mass and radius
  GLfloat  mass, r;
  //Sphere materials
  GLfloat amb[4], spec[4], diff[4];

 public:
  Sphere(void);
  Sphere(GLfloat r, Vector3 pos);
  Sphere(GLfloat r, Vector3 pos, Vector3 vel);

  void move(void);

  Vector3 getPos(void);
  Vector3 getVel(void);
  void setVel(Vector3 v);
  GLfloat getMass(void);
  GLfloat getR(void);

  void draw(void);
};
#endif
