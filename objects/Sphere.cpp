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

    //Ambient
    amb[0] = 0.0f;
    amb[1] = 0.0f;
    amb[2] = 0.5f;
    amb[3] = 1.0f;

    //Specular
    spec[0] = 0.0f;
    spec[1] = 0.0f;
    spec[2] = 1.0f;
    spec[3] = 1.0f;

    //Shinness
    shin[0] = 4.0f;
}

/* moVes the sphere
 */
void Sphere::move(GLfloat delta, GLfloat g)
{
    pos += vel*delta;
    vel[1] -= g;
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
    GLUquadric *quad = gluNewQuadric();
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, amb);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT, GL_SHININESS, shin);
        glTranslatef(pos[0],pos[1],pos[2]);
        //glutSolidSphere(r, 40, 40);
        gluSphere(quad, r, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);
}
