#include "math/Utility.h"
#include "math/Vector3.h"
#include "objects/Sphere.h"
#include "objects/Wall.h"
#include "Camera.h"
#include "GL/glut.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

//Keyboard
bool keyN[256];
bool keyS[21];

//Mouse
GLint lastx,lasty;
bool outside = true;

//Example spheres
vector<Sphere> spheres;

//Example walls
vector<Wall> walls;

//Camera
Camera camera(Vector3(0.0,0.0,5.0), PI/2, -PI/2, 5.0);
//Speed Variables
float delta = 0.005;

//Light Varaibles
GLfloat LA1[]= {0.4f, 0.4f, 0.4f, 1.0f };
GLfloat LD1[]= {0.8f, 0.8f, 0.8f, 1.0f };
GLfloat LS1[]= {1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LP1[]= {7.0f, 5.0f, 7.0f, 1.0f };

//FPS calculation variables
GLint currTime, lastTime, fps;

//Keyboard functions
void keyDown(unsigned char key, int x, int y)
{
  keyN[key] = true;
}
void keyUp(unsigned char key, int x, int y)
{
  keyN[key] = false;
}
void keySDown(int key, int x, int y)
{
  keyS[key] = true;
}
void keySUp(int key, int x, int y)
{
   keyS[key] = false;
}

//Mouse functions
void mouseMotion(int x, int y)
{
    if(outside || x >= glutGet(GLUT_WINDOW_WIDTH)-2 || y >= glutGet(GLUT_WINDOW_HEIGHT)-2 | x <= 2 || y <= 2)
        lastx=x,lasty=y, outside=false;
    GLfloat deltax, deltay;
    deltax = 1.3*(GLfloat)(x-lastx)/glutGet(GLUT_WINDOW_WIDTH);
    deltay = 1.3*(GLfloat)(y-lasty)/glutGet(GLUT_WINDOW_WIDTH);
    
    camera.rotate(LEFT,deltax*10);
    camera.rotate(UP,deltay*10);

    lastx = x;
    lasty = y;
}

//Checks if the mouse leaves the window
void mouseEntry(int state)
{
    if(state == GLUT_LEFT)
        outside = true;
    if(state == GLUT_ENTERED)
        outside = false;
}

/* 
   Reads the keyboard state and updates
   the simulation state
 */
void keyboard()
{
    //Camera movement
    if(keyN['w'] || keyN['W'])camera.move(UP, delta);
    if(keyN['s'] || keyN['S'])camera.move(DOWN, delta);
    if(keyN['a'] || keyN['A'])camera.move(LEFT, delta);
    if(keyN['d'] || keyN['D'])camera.move(RIGHT, delta);

    /*//Camera rotation
    if(keyS[GLUT_KEY_UP])camera.rotate(DOWN, delta/10);
    if(keyS[GLUT_KEY_DOWN])camera.rotate(UP, delta/10);
    if(keyS[GLUT_KEY_LEFT] || keyN['q'] || keyN['Q'])camera.rotate(RIGHT, delta/10);
    if(keyS[GLUT_KEY_RIGHT] || keyN['e'] || keyN['E'])camera.rotate(LEFT, delta/10);
    */
    if(keyN[27])exit(0);
}

//Draw the simulation
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Reposition the light
    glLightfv(GL_LIGHT0, GL_POSITION, LP1);
    //Draw a little sphere in the light position
    glPushMatrix();
    glTranslatef(LP1[0], LP1[1], LP1[2]);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutWireSphere(0.2, 20, 20);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    //Draw the spheres
    for(int i = 0; i < spheres.size(); ++i)
      spheres[i].draw();
    
    //Draw the walls
    glDisable(GL_CULL_FACE);
    for(int i = 0; i < walls.size(); ++i)
        walls[i].draw();
    glEnable(GL_CULL_FACE);

    glutSwapBuffers();
}

//Updates the logic
void update()
{
    //FPS calculation
    currTime=glutGet(GLUT_ELAPSED_TIME);
	if (currTime - lastTime > 1000) {
        stringstream ss;
        ss << "Sphere collition " << "FPS: " << fps*1000/(currTime-lastTime);
        glutSetWindowTitle(ss.str().c_str());
        lastTime = currTime;
		fps = 0;
     }

    //Checks the keyboard input
    keyboard();

    //Update the spheres positions, and then checks if they collide
    for(int i = 0; i < spheres.size(); ++i)
        spheres[i].move();

    //Check if any sphere is colliding
    /* Provisional method, doesn't take the mass into account
     */
    for(int i = 0; i < spheres.size(); ++i)
        for(int j = i+1; j < spheres.size(); ++j)
            if(areColliding(spheres[i], spheres[j]))
                collision(spheres[i], spheres[j]);
    /*//Check if they are colliding REAL METHOD; BUT IS NOT WORKING RIGHT NOW
    if(areColliding(sp1, sp2))
    {
        //collision(sp1,sp2);
        Vector3 disp = (sp1.getPos() - sp2.getPos()).normalize();
        sp1.setVel(sp1.getVel() - (disp * sp1.getVel().dot(disp) * 2));
        sp2.setVel(sp2.getVel() - (disp * sp2.getVel().dot(disp) * 2));
        }*/

   //Checks if the balls collide with the walls
    for(int i = 0; i < spheres.size(); ++i)
        for(int j = 0; j < walls.size(); ++j)
            if(sphereWallColliding(spheres[i], walls[j]))
                wallCollision(spheres[i],walls[j]);

    //Draws the simulation
    draw();
    
    fps++;
}

/*
  Handles the resize events
 */
void resize(int w, int h)
{
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if(h == 0)
    h = 1;
  
  float ratio = 1.0* w / h;
  
  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);
  
  // Set the correct perspective.
  gluPerspective(45,ratio,1,500);
  
  camera.setUp();
}

/*
  Initializes some stuff
 */
void init()
{
    //Add some spheres
    spheres.push_back(Sphere(1.0f, Vector3(1.0f, 0.0f, 5.0f), Vector3(0.0f, 0.0f, -0.004f)));
    spheres.push_back(Sphere(1.0f, Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, 0.004f)));
    spheres.push_back(Sphere(1.0f, Vector3(5.0f, 0.0f, -5.0f), Vector3(0.002f, 0.0f, 0.002f)));

    //Add some walls
    walls.push_back(Wall(Vector3(10.0f, 0.0f, -10.0f), Vector3(10.0f, 5.0f, 10.0f), -1.0f, 0.0f, 0.0f, 10.0f, true));
    walls.push_back(Wall(Vector3(-10.0f, 0.0f, -10.0f), Vector3(-10.0f, 5.0f, 10.0f), 1.0f, 0.0f, 0.0f, 10.0f, true));
    walls.push_back(Wall(Vector3(-10.0f, 0.0f, -10.0f), Vector3(10.0f, 5.0f, -10.0f), 0.0f, 0.0f, 1.0f, 10.0f, true));
    walls.push_back(Wall(Vector3(10.0f, 0.0f, 10.0f), Vector3(-10.0f, 5.0f, 10.0f), 0.0f, 0.0f, -1.0f, 10.0f, true));
}

/*
  Initializes OpenGL
 */
void initGl()
{
	glEnable(GL_LIGHTING);

	//Light 1
	glLightfv(GL_LIGHT0, GL_AMBIENT, LA1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LD1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LS1);
	glLightfv(GL_LIGHT0, GL_POSITION, LP1);
	glEnable(GL_LIGHT0);

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

int main(int args, char *argv[])
{
    glutInit(&args, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("Sphere collision");
    
    //Initializes the FPS counter
    currTime = glutGet(GLUT_ELAPSED_TIME);
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    fps = 0;
    
    //Keyboard Functions
    glutKeyboardFunc(keyDown);
    glutSpecialFunc(keySDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialUpFunc(keySUp);

    //Mouse functions
    glutPassiveMotionFunc(mouseMotion);
    
    glutReshapeFunc(resize);
    glutDisplayFunc(draw);
    glutIdleFunc(update);

    //Initializes OpenGL, and somo other stuff
    initGl();
    init();
    glutMainLoop();
    return 0;
}
