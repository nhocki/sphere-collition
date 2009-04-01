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

Sphere::Vector3 getPos()
{
  return pos;
}

Sphere::Vector3 getVel()
{
  return vel;
}

Sphere::setVel(Vector3 v)
{
  vel = v;
}

Sphere::setRap(GLfloat t)
{
  rapidez = t;
}

Sphere::getRap()
{
  return rapidez;
}
