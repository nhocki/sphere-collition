#include "Wall.h"
#include <iostream>
#include <math.h>

using namespace std;
Wall::Wall()
{
    Wall(Vector3(0.0f, -20.0f, -20.0f), Vector3(0.0f, 20.0f, 20.0f), 1.0f, 0.0f, 0.0f, 0.0f, false, 0, 'x');
}

Wall:: Wall(Vector3 min, Vector3 max, GLfloat a, GLfloat b, GLfloat c, GLfloat d, GLboolean wire, GLuint tex, char coord)
{
    Wall::min = min;
    Wall::max = max;
    Wall::a = a;
    Wall::b = b;
    Wall::c = c;
    Wall::d = d;
    Wall::wire = wire;
    Wall::tex = tex;
	Wall::coord = coord;

    calculatePoints();

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

    shin[0] = 6.0f;
}

void Wall::calculatePoints()
{
    GLfloat dx = (max[0] - min[0])/5;
    GLfloat dy = (max[1] - min[1])/5;
    GLfloat dz = (max[2] - min[2])/5;

    for(int i = 0; i < 6; ++i)
    {
        for(int j = 0; j < 6; ++j)
        {
            if(dy!=0)
                points[j + i*6] = Vector3(min[0] + j*dx, min[1] + i*dy, min[2] + j*dz);
            else
                 points[j + i*6] = Vector3(min[0] + j*dx, min[1] + i*dy, min[2] + i*dz);
        }
    }
}

/* Get the plane ecuation components
 */
GLfloat Wall::getA(void){return a;}
GLfloat Wall::getB(void){return b;}
GLfloat Wall::getC(void){return c;}
GLfloat Wall::getD(void){return d;}
char Wall::getCoord(void){return coord;}

/*
  Draws the wall
 */
void Wall::draw()
{
    //If I want the wall to be drawn in wire frame, I call the glPolygonMode
    if(wire)glPolygonMode(GL_BACK, GL_LINE);
    glPushMatrix();
    /*glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, amb);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT, GL_SHININESS, shin);*/

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex);
        
        glBegin(GL_QUADS);

        for(int i = 0; i < 5; ++i)
        {
            for(int j = 0; j < 5; ++j)
            {
                int p1=j + i*6,p2=(j+1) + i*6,p3=(j+1)+(i+1)*6,p4=j+ (i+1)*6;
                glNormal3f(a,b,c);
                glTexCoord2f((float)j/5, (float)i/5);
                glVertex3f(points[p1][0], points[p1][1], points[p1][2]);

                glNormal3f(a,b,c);
                glTexCoord2f((float)(j+1)/5, (float)i/5);
                glVertex3f(points[p2][0], points[p2][1], points[p2][2]);

                glNormal3f(a,b,c);
                glTexCoord2f((float)(j+1)/5, (float)(i+1)/5);
                glVertex3f(points[p3][0], points[p3][1], points[p3][2]);

                glNormal3f(a,b,c);
                glTexCoord2f((float)j/5, (float)(i+1)/5);
                glVertex3f(points[p4][0], points[p4][1], points[p4][2]);
            }
        }
        glEnd();  
    glPopMatrix();
    //Restore the solid mode
    if(wire)glPolygonMode(GL_BACK, GL_FILL);
}
