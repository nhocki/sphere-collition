#ifndef SPHERE
#define SPHERE
#include "../math/Vector3.h"
#include "../math/Utility.h"
#include "GL/glut.h"

/* Class Sphere
 *  Contains all the necesary info about the spheres
 */
class Sphere
{
private:
    //Position and direction
    Vector3 pos, dir;
    //Velocity, mass and radius
    GLfloat vel, mass, r;
    //Sphere materials
    GLfloat amb[4], spec[4], diff[4];

public:
    Sphere(void);
    Sphere(GLfloat r, Vector3 pos);

    Vector3 getPos(void);
    Vector3 getDir(void);
    GLfloat getVel(void);
    GLfloat getMass(void);
    GLfloat getR(void);

    void draw(void);
};
#endif
