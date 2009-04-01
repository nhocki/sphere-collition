#include "Sphere.h"

Sphere::Sphere()
{
    Sphere(1.0f, Vector3(0.0f, 0.0f, 0.0f));
}

Sphere::Sphere(GLfloat r, Vector3 pos)
{
    Sphere(r, pos, Vector3(0.0f, 0.0f, 1.0f));
}

Sphere::Sphere(GLfloat r, Vector3 pos, Vector3 vel)
{
    Sphere::r = r;
    Sphere::pos = pos;
    Sphere::vel = vel;
    mass = 1.0f;
}

/* moVes the sphere
 */
void Sphere::move()
{
    pos += vel;
}

/*
  A lot of getters and setters
 */
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


GLfloat Sphere::getR()
{
    return r;
}

void Sphere::setVel(Vector3 v)
{
    vel = v;
}

/* Draws the sphere
 */
void Sphere::draw()
{
    glPushMatrix();
        glTranslatef(pos[0],pos[1],pos[2]);
        glutSolidSphere(r, 30, 30);
    glPopMatrix();
}
