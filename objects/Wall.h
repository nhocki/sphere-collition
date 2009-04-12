#ifndef WALL
#define WALL
#include "../math/Vector3.h"
#include <iostream>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#endif

/* Class Sphere
 *  Contains all the necesary info about the Walls
 */
class Wall
{
private:
	//Relevant coord of the wall
	char coord;
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
    GLuint tex;
public:
    Wall(void);
    Wall(Vector3 min, Vector3 max, GLfloat a, GLfloat b, GLfloat c, GLfloat d, GLboolean wire, GLuint tex, char coord);
    void calculatePoints(void);
    Vector3 getMin(void);
    Vector3 getMax(void);
    GLfloat getA(void);
    GLfloat getB(void);
    GLfloat getC(void);
    GLfloat getD(void);
	char getCoord(void);
    GLuint getText(void);
    void setText(GLuint);

    void draw(void);
};
#endif
