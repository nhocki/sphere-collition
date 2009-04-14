/*
  Program that simulates the collision of a lot of spheres
  using a fast algorithm.
  Alejandro Pelaez
  Nicolas Hock
  Cristian Isaza
 */

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
#include "Octree.h"
#include "Timer.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define D(x) cout <<"Line "<< __LINE__ <<"    "<<#x"  is  " << x << endl
#define MAX 500

using namespace std;

GLfloat gravity = 0.0098;

//Keyboard
bool keyN[256];
bool keyS[21];

//Mouse
bool pointer = false;
GLint lastx,lasty;
bool outside = true;

//Sphere list and octree
vector<Sphere*> spheres;
Octree octree(Vector3(-15.0f, 0.0f, -15.0f), Vector3(15.0f, 30.0f, 15.0f), 0);
//bolean that tells to use the octree or not
bool octr = false;

//Quadric for the spheres
GLUquadric *quad;

//Sphere textures
GLuint texts[10];

string texNames[10] = {"textures/bola1.bmp","textures/bola2.bmp","textures/bola3.bmp",
					   "textures/bola4.bmp","textures/bola5.bmp","textures/bola6.bmp",
					   "textures/bola7.bmp","textures/bola8.bmp","textures/names.bmp",
					   "textures/instructions.bmp"};

//Example walls
vector<Wall*> walls;

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
//Number of collition calculations
GLint calc;
//Timer
Timer timer;

//Toggles between elastic and inellastic collisions
bool inellastic = false;

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
            for(int i = 0; i < 5; ++i)
                addSphere();

	if(button == GLUT_RIGHT_BUTTON)
        if(state == GLUT_DOWN)
			if(spheres.size() > 0)
				for(int i = 0; i < 5; ++i)
					octree.remove(spheres.back()), spheres.erase(spheres.end()-1);
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
	if(outside || x >= width-2 || y >= height-2 || x <= 2 || y <= 2)
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

	//Switch between ellastic and inellastic collisions
	if(keyN['i'] || keyN['I'])
		inellastic = !inellastic, keyN['i']=false, keyN['I']=false;

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

	//Switch between octree or slow method
	if(keyN['o'] || keyN['O'])
		octr = !octr, keyN['o']=false, keyN['O']=false;

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
	{
		spheres[i]->draw();
		Vector3 pos = spheres[i]->getPos();
		/*glBegin(GL_LINES);
		glVertex3f(pos[0], pos[1], pos[2]);
		glVertex3f(0,15,0);
		glEnd();*/
    }
	//Draw the walls
	glDisable(GL_CULL_FACE);
	for(unsigned int i = 0; i < walls.size(); ++i)
		walls[i]->draw();
	glEnable(GL_CULL_FACE);

	glutSwapBuffers();
}

//Updates the logic
void update()
{
	double prevTime = currTime;
	currTime = timer.getElapsedTime();

	double deltaTime = currTime - prevTime;

	GLfloat dfps = (GLfloat)deltaTime/1000000;
	gravity = 4.0*9.8*dfps;
	delta = 10*dfps;
	deltaBall = 2.5*dfps;


	//Checks the keyboard input
	keyboard();

	//Update the spheres positions, and then checks if they collide
	for(unsigned int i = 0; i < spheres.size(); ++i)
	{
		Vector3 oldPos = spheres[i]->getPos();
		spheres[i]->move(deltaBall, gravity);
		octree.sphereMoved(spheres[i], oldPos);
	}

	//Spheres collitions
	if(octr)
	{
		vector<SpherePair> sp;
		octree.potentialSphereCollisions(sp);
		for(unsigned int i = 0; i < sp.size(); ++i, calc++)
			if(areColliding(sp[i].first, sp[i].second))
				collision(sp[i].first, sp[i].second, inellastic);
	}
	else
		for(unsigned int i = 0; i < spheres.size(); ++i)
			for(unsigned int j = i+1; j < spheres.size(); ++j, calc++)
				if(areColliding(spheres[i], spheres[j]))
					collision(spheres[i], spheres[j], inellastic);

	if (octr)
	{
		vector<SphereWallPair> sw;
		octree.potentialSphereWallCollisions(sw, walls);
		for(unsigned int i = 0; i < sw.size(); ++i, calc++)
			if(sphereWallColliding(sw[i].first, sw[i].second))
				wallCollision(sw[i].first, sw[i].second, inellastic);
	}
	else
		//Checks if the balls collide with the walls
		for(unsigned int i = 0; i < spheres.size(); ++i)
			for(unsigned int j = 0; j < walls.size(); ++j, calc++)
				if(sphereWallColliding(spheres[i], walls[j]))
					wallCollision(spheres[i],walls[j], inellastic);

	//Draws the simulation
	draw();

	//FPS calculation
	//Happens every 1/5 of a second, more precision but flickier
	if (currTime - lastTime > 1000000/5) {
		stringstream ss;
        int n = spheres.size();
		ss << "Sphere collision " << "FPS: " << (double)fps*1000000/(currTime-lastTime);
        ss << " Number of spheres: " << n << " Calculations: " << calc;
		if(octr)
			ss << " Octree method";
		else
			ss << " Slow method";
		glutSetWindowTitle(ss.str().c_str());
		lastTime = currTime;   

		//Recalculates deltas
		/*GLfloat dfps = (GLfloat)1/(fps*5);
		gravity = 3.0*9.8*dfps;
		delta = 10*dfps;
		deltaBall = 2.5*dfps;*/

		//cout << gravity << "delta: "<< delta << "deltaBall: "<< deltaBall <<endl;
        
		fps = 0;
	}
	fps++;
	calc = 0;
}

void addSphere()
{
	if(spheres.size()>=MAX) return ;
	GLfloat r, x, y, z, vx, vy, vz;
	int signo;
	
	r = (rand()/ (RAND_MAX + 1.0))/2.75 + 0.3;

	signo = pow(-1.0, (rand()%2)+1); //Generates 1 or 2
	x = signo*(rand()%12)/(rand()%12 + 1.0);

	y = (rand()%15)/(rand()%15+1.0) + 10.0;

	signo = pow(-1.0, (rand()%2)+1);
	z = signo*(rand()%12)/(rand()%12 + 1.0);

	signo = pow(-1.0, (rand()%2)+1);
	vx = signo*(rand()%2+1);

	signo = pow(-1.0, (rand()%2)+1);
	vy = signo*(rand()%2+1);

	signo = pow(-1.0, (rand()%2)+1);
	vz = signo*(rand()%2+1);

	GLint tex=rand()%8;

	Vector3 pos(x,y,z), vel(vx,vy,vz);

	//cout << "r: "<< r << " pos: " << pos << " vel: " << vel << " tex: "<< texNames[tex] << endl;

	Sphere *s = new Sphere(r,pos,vel,texts[tex], quad);
	spheres.push_back(s);
	octree.add(s);
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

	//Add some walls
	walls.push_back(new Wall(Vector3(15.0f, 0.0f, -15.0f), Vector3(15.0f, 30.0f, 15.0f),
                         -1.0f, 0.0f, 0.0f, 15.0f, true, texts[8],'x'));

	walls.push_back(new Wall(Vector3(-15.0f, 0.0f, 15.0f), Vector3(-15.0f, 30.0f, -15.0f),
                         1.0f, 0.0f, 0.0f, 15.0f, true, texts[9],'x'));

	walls.push_back(new Wall(Vector3(-15.0f, 0.0f, -15.0f), Vector3(15.0f, 30.0f, -15.0f),
                         0.0f, 0.0f, 1.0f, 15.0f, true, texts[8], 'z'));

	walls.push_back(new Wall(Vector3(15.0f, 0.0f, 15.0f), Vector3(-15.0f, 30.0f, 15.0f),
                         0.0f, 0.0f, -1.0f, 15.0f, true, texts[9], 'z'));

	//Ceiling and floor
	walls.push_back(new Wall(Vector3(-15.0f, 0.0f, 15.0f), Vector3(15.0f, 0.0f, -15.0f),
                         0.0f, 1.0f, 0.0f, 0.0f, true, texts[8], 'y'));

	walls.push_back(new Wall(Vector3(-15.0f, 30.0f, -15.0f), Vector3(15.0f, 30.0f, 15.0f),
                         0.0f, -1.0f, 0.0f, 30.0f, true, texts[8], 'y'));

	//Creates the quadric for the balls
	quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);

	//Adds some spheres
	//addSphere();

	//Starts the timer
	timer.start();

	//Initializes the FPS counter
	lastTime = timer.getElapsedTime();
	currTime = timer.getElapsedTime();
	fps = 0;
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
