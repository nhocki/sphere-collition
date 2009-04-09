#include "Camera.h"
/* Contructors*/
Camera::Camera()
{
    Camera(Vector3(0.0,0.0,5.0), PI/2, 0.0, 5.0);
}

Camera::Camera(Vector3 pos, GLfloat angleXY, GLfloat angleXZ, GLfloat r)
{
    Camera::pos = pos;
    Camera::angleXY = angleXY;
    Camera::angleXZ = angleXZ;
    Camera::r = r;
    setUp();
}
/*
  Sets the camera based on the pos and angle variables
 */
void Camera::setUp()
{
    //Calculates the direction of the camera
    GLfloat temp = sin(angleXY);
    dir[1] = cos(angleXY);
    dir[0] = temp*cos(angleXZ);
    dir[2] = temp*sin(angleXZ);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pos[0],pos[1],pos[2], 
              pos[0]+r*dir[0],pos[1]+r*dir[1],pos[2]+r*dir[2],
              0.0f,1.0f,0.0f);
/* DEBUG
    cout << "pos:"<< pos << endl << "dir:"<< dir << endl << "r:" << r << endl;
    cout << "XY: " << angleXY << " XZ: " << angleXZ << endl;*/
}

void Camera::move(Direction d, GLfloat delta)
{
    if(d == UP)
    {
        pos[0]+=delta*dir[0];
        pos[1]+=delta*dir[1];
        pos[2]+=delta*dir[2];
    }
    if(d == DOWN)
    {
        pos[0]-=delta*dir[0];
        pos[1]-=delta*dir[1];
        pos[2]-=delta*dir[2];
    }
    if(d == LEFT)
    {
        pos[0] += delta*sin(angleXZ);
        pos[2] -= delta*cos(angleXZ);
    }
    if(d == RIGHT)
    {
        pos[0] -= delta*sin(angleXZ);
        pos[2] += delta*cos(angleXZ);
    }
    setUp();
}

void Camera::rotate(Direction d, GLfloat delta)
{
    if(d == UP)angleXY+=delta;
    if(d == DOWN)angleXY-=delta;
    if(d == LEFT)angleXZ+=delta;
    if(d == RIGHT)angleXZ-=delta;

    if(angleXY > PI*0.999)angleXY = PI*0.999;
    if(angleXY < 0.0001)angleXY=0.0001;
    
    setUp();
}
