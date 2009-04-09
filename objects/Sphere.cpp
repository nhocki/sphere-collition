#include "Sphere.h"

Sphere::Sphere()
{
    Sphere(1.0f, Vector3(0.0f, 0.0f, 0.0f));
}

Sphere::Sphere(GLfloat r, Vector3 pos)
{
    Sphere(r, pos, Vector3(0.0f, 0.0f, 1.0f), 0);
}

Sphere::Sphere(GLfloat r, Vector3 pos, Vector3 vel, GLuint tex)
{
    Sphere::r = r;
    Sphere::pos = pos;
    Sphere::vel = vel;
    Sphere::tex = tex;
    mass = 1.0f;

    //Set the materials
    //Ambient
    amb[0] = 0.3f;
    amb[1] = 0.3f;
    amb[2] = 0.3f;
    amb[3] = 1.0f;

    //Specular
    spec[0] = 0.6f;
    spec[1] = 0.6f;
    spec[2] = 0.6f;
    spec[3] = 0.6f;

    shin[0] = 2.0f;

    //Rotation
    rot = Vector3(vel[0], 0.0f, -vel[2]);
}

/* moVes the sphere
 */
void Sphere::move(GLfloat delta, GLfloat g)
{
    pos += vel*delta;
    vel[1] -= g;
    angle += 0.2;

    //Calculates the new rotation
    //is arbitrary, don know how to calculate the real one
    rot[0] = vel[0];
    rot[1] = 0.0f;
    rot[2] = -vel[2];
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
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
        //Enable Texture
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, amb);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT, GL_SHININESS, shin);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTranslatef(pos[0],pos[1],pos[2]);
        glRotatef(angle, rot[0],rot[1],rot[2]);
        //glutSolidSphere(r, 40, 40);
        gluSphere(quad, r, 20, 20);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    gluDeleteQuadric(quad);
}
