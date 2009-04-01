#include "Wall.h"

Wall::Wall()
{
    Wall(Vector3(0.0f, -20.0f, -20.0f), Vector3(0.0f, 20.0f, 20.0f), 1.0f, 0.0f, 0.0f, 0.0f);
}

Wall:: Wall(Vector3 min, Vector3 max, GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
    Wall::min = min;
    Wall::max = max;
    Wall::a = a;
    Wall::b = b;
    Wall::c = c;
    Wall::d = d;
}

/* Get the plane ecuation components
 */
GLfloat Wall::getA(void){return a;}
GLfloat Wall::getB(void){return b;}
GLfloat Wall::getC(void){return c;}
GLfloat Wall::getD(void){return d;}
