#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glu.h"
#endif

#include "math/Utility.h"
#include "math/Vector3.h"
#include "objects/Sphere.h"
#include "objects/Wall.h"
#include "Camera.h"
#include "Loader.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define D(x) cout <<"Line "<< __LINE__ <<"    "<<#x"  is  " << x << endl
#define MAX 85

using namespace std;

GLfloat gravity = 0.0098;

//Keyboard
bool keyN[256];
bool keyS[21];

//Mouse
bool pointer = false;
GLint lastx,lasty;
bool outside = true;

//Example spheres
vector<Sphere> spheres;
//Sphere textures
GLuint texts[10];
string texNames[10] = {"textures/bola1.bmp", "textures/bola2.bmp", "textures/bola3.bmp", "textures/bola4.bmp" 
                       ,"textures/bola5.bmp", "textures/bola6.bmp", "textures/bola7.bmp", "textures/bola8.bmp"
                       ,"textures/names.bmp", "textures/instructions.bmp"};

//Example walls
vector<Wall> walls;

//Camera
Camera camera(Vector3(0.0,10.0,10.0), PI/2, -PI/2, 5.0);

//Speed Variables
GLfloat delta = 0.005f;
GLfloat deltaBall = 0.001f;

//Light Varaibles
GLfloat LA1[]= {0.3f, 0.3f, 0.3f, 1.0f };
GLfloat LD1[]= {0.6f, 0.6f, 0.6f, 1.0f };
GLfloat LS1[]= {1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LP1[]= {5.0f, 5.0f, 0.0f, 1.0f };

//FPS calculation variables, and speed control
GLint currTime, lastTime, fps;

//Declaration of a function
void addSphere();

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

//Checks if the mouse leaves the window
void mousePressed(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
        if(state == GLUT_DOWN)
            for(int i = 0; i < 10; ++i)
                addSphere();
}

void mouseMotion(int x, int y)
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	if(x >= width - 10)x = 11, outside = true;
	if(x <= 10)x = width-11, outside = true;
	if(y >= height - 10)y = 11, outside = true;
	if(y <= 10)y = height-11, outside=true;

	if(!pointer && outside)glutWarpPointer(x, y);
    
	if(outside || x >= width-2 || y >= height-2 | x <= 2 || y <= 2)
        lastx=x,lasty=y, outside=false;
    
	GLfloat deltax, deltay;
	deltax = 1.3*(GLfloat)(x-lastx)/width;
	deltay = 1.3*(GLfloat)(y-lasty)/height;
    
	if(!pointer)
    {
        camera.rotate(LEFT,deltax*10);
        camera.rotate(UP,deltay*10);
    }

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

	//Creates random Spheres
	if(keyN['c'] || keyN['C'])
    {
        for(int i = 0; i < 10; ++i)
            addSphere();
        keyN['c']=keyN['C']=false;
    }

	//Enables the pointer
	if(keyN['p']||keyN['P'])
		{
			pointer = !pointer;
			if(!pointer)
				glutSetCursor(GLUT_CURSOR_NONE);
			else
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			keyN['p']=keyN['P']=false;
		}

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
	for(unsigned int i = 0; i < spheres.size(); ++i)
		spheres[i].draw();
    
	//Draw the walls
	glDisable(GL_CULL_FACE);
	for(unsigned int i = 0; i < walls.size(); ++i)
		walls[i].draw();
	glEnable(GL_CULL_FACE);

	glutSwapBuffers();
}

//Updates the logic
void update()
{
	currTime=glutGet(GLUT_ELAPSED_TIME);

	//Checks the keyboard input
	keyboard();

	//Update the spheres positions, and then checks if they collide
	for(unsigned int i = 0; i < spheres.size(); ++i)
		spheres[i].move(deltaBall, gravity);

	//Check if any sphere is colliding
	/* Provisional method, doesn't take the mass into account
	 */
	for(unsigned int i = 0; i < spheres.size(); ++i)
		for(unsigned int j = i+1; j < spheres.size(); ++j)
			if(areColliding(spheres[i], spheres[j]))
				collision(spheres[i], spheres[j]);

	//Checks if the balls collide with the walls
	for(unsigned int i = 0; i < spheres.size(); ++i)
		for(unsigned int j = 0; j < walls.size(); ++j)
			if(sphereWallColliding(spheres[i], walls[j]))
				wallCollision(spheres[i],walls[j]);

	//Draws the simulation
	draw();

	//FPS calculation
	if (currTime - lastTime > 1000) {
		stringstream ss;
        int n = spheres.size();
		ss << "Sphere collision " << "FPS: " << fps*1000/(currTime-lastTime);
        ss << " Number of spheres: " << n << " Calculations: " << n*(n+1)/2 + 6*n;
		glutSetWindowTitle(ss.str().c_str());
		lastTime = currTime;   

		//Recalculates deltas
		GLfloat dfps = (GLfloat)1/fps;
		gravity = 2.5*9.8*dfps;
		delta = 10*dfps;
		deltaBall = 2.5*dfps;

		//cout << gravity << "delta: "<< delta << "deltaBall: "<< deltaBall <<endl;
        
		fps = 0;
	}
	fps++;
}

void addSphere()
{
	if(spheres.size()>=MAX) return ;
	GLfloat r, x, y, z, vx, vy, vz;
	int signo;
	
	r = rand() / (RAND_MAX + 1.0); 
	while(r<0.3) 	r = rand() / (RAND_MAX + 1.0); 
	
	signo = pow(-1 , (rand()%2)+1); //Generates 1 or 2

	x = signo*(rand()%8)/(rand()%8 + 1.0);
	signo = pow(-1 , (rand()%2)+1);

	y = (rand()%8)/(rand()%8+1.0) + 7.0;
	signo = pow(-1 , (rand()%2)+1);

	z = signo*(rand()%8)/(rand()%8 + 1.0);
	signo = pow(-1 , (rand()%2)+1);

	vx = signo*(rand()%4+1);
	signo = pow(-1 , (rand()%2)+1);

	vy = signo*(rand()%4+1);
	signo = pow(-1 , (rand()%2)+1);

	vz = signo*(rand()%4+1);
	signo = pow(-1 , (rand()%2)+1);
	GLint tex=rand()%8;

	//cout <<"radio: "<< r <<"  " << x <<"   " << y <<"   " << z <<endl;
	//D(x),D(y),D(z);

	spheres.push_back(Sphere(r,Vector3(x,y,z),Vector3(vx,vy,vz),texts[tex]));
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
	//Load the textures
	glGenTextures(10, texts);
	
	for(int i = 0; i < 10; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, texts[i]);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        // when texture area is small, bilinear filter the closest mipmap
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
        // when texture area is large, bilinear filter the original
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // the texture wraps over at the edges (repeat)
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        
        Image *image = loadBMP(texNames[i].c_str());
        
        //Builds the texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->getWidth(), image->getHeight(),
                          GL_RGB, GL_UNSIGNED_BYTE, image->getPixels());

        delete image;
    }

	//Add some spheres
	srand((unsigned)time(NULL)); //Starts the random int generator

	spheres.push_back(Sphere(0.5f, Vector3(1.0f, 8.0f, 5.0f), Vector3(0.0f, 0.0f, -1.5f), texts[0]));
	spheres.push_back(Sphere(0.5f, Vector3(0.0f, 8.0f, -5.0f), Vector3(0.0f, 0.0f, 1.8f), texts[1]));
	spheres.push_back(Sphere(0.5f, Vector3(5.0f, 8.0f, -5.0f), Vector3(2.0f, 0.0f, 2.0f), texts[2]));
	spheres.push_back(Sphere(0.5f, Vector3(0.0f, 15.0f, -5.0f), Vector3(2.0f, 0.0f, 2.0f), texts[3]));
	spheres.push_back(Sphere(0.5f, Vector3(5.0f, 15.0f, 0.0f), Vector3(1.0f, 1.0f, 2.0f), texts[4]));
	//for(int i = 0; i < 10; ++i)  addSphere();

	//Add some walls
	walls.push_back(Wall(Vector3(10.0f, 0.0f, -10.0f), Vector3(10.0f, 20.0f, 10.0f), 
                         -1.0f, 0.0f, 0.0f, 10.0f, true, texts[8]));

	walls.push_back(Wall(Vector3(-10.0f, 0.0f, 10.0f), Vector3(-10.0f, 20.0f, -10.0f), 
                         1.0f, 0.0f, 0.0f, 10.0f, true, texts[9]));

	walls.push_back(Wall(Vector3(-10.0f, 0.0f, -10.0f), Vector3(10.0f, 20.0f, -10.0f), 
                         0.0f, 0.0f, 1.0f, 10.0f, true, texts[8]));

	walls.push_back(Wall(Vector3(10.0f, 0.0f, 10.0f), Vector3(-10.0f, 20.0f, 10.0f), 
                         0.0f, 0.0f, -1.0f, 10.0f, true, texts[9]));
    
	//Ceiling and floor
	walls.push_back(Wall(Vector3(-10.0f, 0.0f, 10.0f), Vector3(10.0f, 0.0f, -10.0f), 
                         0.0f, 1.0f, 0.0f, 0.0f, true, texts[8]));

	walls.push_back(Wall(Vector3(-10.0f, 20.0f, -10.0f), Vector3(10.0f, 20.0f, 10.0f), 
                         0.0f, -1.0f, 0.0f, 20.0f, true, texts[8]));
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

	glShadeModel (GL_SMOOTH);

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	//Disables the pointer, can be enable by pressing P
	glutSetCursor(GLUT_CURSOR_NONE);
    //Position the mouse
    glutWarpPointer(400, 300);
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
    glutMouseFunc(mousePressed);
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
