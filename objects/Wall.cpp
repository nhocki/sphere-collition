#include "Wall.h"

Wall::Wall()
{
    Wall(Vector3(0.0f, -20.0f, -20.0f), Vector3(0.0f, 20.0f, 20.0f), 1.0f, 0.0f, 0.0f, 0.0f, false);
}

Wall:: Wall(Vector3 min, Vector3 max, GLfloat a, GLfloat b, GLfloat c, GLfloat d, GLboolean wire)
{
    Wall::min = min;
    Wall::max = max;
    Wall::a = a;
    Wall::b = b;
    Wall::c = c;
    Wall::d = d;
    Wall::wire = wire;
}

void Wall::calculatePoints()
{
    GLfloat dx = min[0] - max[0];
    GLfloat dy = min[1] - max[1];
    GLfloat dz = min[2] - max[2];

    for(int i = 0; i < 6; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            
        }
    }
}

/* Get the plane ecuation components
 */
GLfloat Wall::getA(void){return a;}
GLfloat Wall::getB(void){return b;}
GLfloat Wall::getC(void){return c;}
GLfloat Wall::getD(void){return d;}

/*
  Draws the wall
 */
void Wall::draw()
{
    glPushMatrix();
        glBegin(GL_QUADS);

        glEnd();  
    glPopMatrix();
}
