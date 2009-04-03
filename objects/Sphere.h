#ifndef SPHERE
#define SPHERE
#include "../math/Vector3.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#endif

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
  //Wall materials
  GLfloat amb[4], spec[4], shin[1];
  //texture
  GLuint tex;
  //Rotation
  Vector3 rot;
  GLfloat angle;

 public:
  Sphere(void);
  Sphere(GLfloat r, Vector3 pos);
  Sphere(GLfloat r, Vector3 pos, Vector3 vel, GLuint text);

  void move(GLfloat delta, GLfloat time);

  Vector3 getPos(void);
  Vector3 getVel(void);
  void setVel(Vector3 v);
  GLfloat getMass(void);
  GLfloat getR(void);

  void draw(void);
};
#endif
