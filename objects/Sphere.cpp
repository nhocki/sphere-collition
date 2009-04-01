#include "Sphere.h"

Sphere::Sphere()
{
    r = 1;
    pos = Vector3(0.0, 0.0, 0.0);
}

Sphere::Sphere(GLfloat r, Vector3 pos)
{
    Sphere::r = r;
    Sphere::pos = pos;
}

Vector3 Sphere::getPos()
{
  return pos;
}

Vector3 Sphere::getVel()
{
  return vel;
}

GLfloat Sphere::getMass()
{
  return mass;
}

GLfloat Sphere::getRap()
{
  return rapidez;
}

GLfloat Sphere::getR()
{
  return r;
}

void Sphere::setVel(Vector3 v)
{
  vel = v;
}
void Sphere::setRap(GLfloat ra)
{
  rapidez = ra;
}
