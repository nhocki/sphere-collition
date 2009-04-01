#ifndef WALL
#define WALL
#include "../math/Vector3.h"
//#include "../math/Utility.h"
#include "GL/glut.h"

/* Class Sphere
 *  Contains all the necesary info about the Walls
 */
class Wall
{
private:
    //Lower left corner and upper right corner, and the tesselation
    Vector3 min, max;
    Vector3 points[36];
    //Wall materials
    GLfloat amb[4], spec[4], shin[1];
    //Plane Ecuation
    GLfloat a,b,c,d;
    //Tells if the wall should be draw solid, or as a wireframe
    GLboolean wire;
    //Texture
    GLuint text;
public:
    Wall(void);
    Wall(Vector3 min, Vector3 max, GLfloat a, GLfloat b, GLfloat c, GLfloat d, GLboolean wire);
    void calculatePoints(void);
    Vector3 getMin(void);
    Vector3 getMax(void);
    GLfloat getA(void);
    GLfloat getB(void);
    GLfloat getC(void);
    GLfloat getD(void);
    GLuint getText(void);
    void setText(GLuint);

    void draw(void);
};
#endif
