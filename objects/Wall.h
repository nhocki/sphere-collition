#ifndef WALL
#define WALL
#include "../math/Vector3.h"
#include "../math/Utility.h"
#include "GL/glut.h"

/* Class Sphere
 *  Contains all the necesary info about the Walls
 */
class Wall
{
private:
    //Lower left corner and upper right corner
    Vector3 min, max;
    //Wall materials
    GLfloat amb[4], spec[4], diff[4];
    //Texture
    GLuint text;
public:
    Wall(void);
    Wall(Vector3 min, Vector3 max);
    Vector3 getMin(void);
    Vector3 getMax(void);
    GLuint getText(void);
    void setText(GLuint);
};
#endif
