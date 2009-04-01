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

    //Calculates the other two points
    if(min[0]==max[0])
        pt1 = Vector3(min[0], max[1], min[2]), pt2 = Vector3(min[0], min[1], max[2]);
    if(min[1]==max[1])
        pt1 = Vector3(min[0], min[1], max[2]), pt2 = Vector3(max[0], min[1], min[2]);
    if(min[2]==max[2])
        pt1 = Vector3(min[0], max[1], min[2]), pt2 = Vector3(max[0], min[1], min[2]);
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
        glVertex3f(min[0], min[1], min[2]);
        glVertex3f(pt2[0], pt2[1], pt2[2]);
        glVertex3f(max[0], max[1], max[2]);
        glVertex3f(pt1[0], pt1[1], pt1[2]);
        glEnd();  
    glPopMatrix();
}
