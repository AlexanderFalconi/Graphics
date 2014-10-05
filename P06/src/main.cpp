#include <GL/glew.h> // glew must be included before the main gl libs
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <cstdlib>
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include "include/universe.h"
#include "include/object.h"
using std::string;

int width = 640, height = 480;// Window size
int leftClick = 0;//handle left clicking
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void mouse(int button, int state, int x_pos, int y_pos);
void mainMenu(int value);
void specialInput(int key, int x, int y);

//--Resource management
bool initialize();

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

Universe* engine;

int main(int argc, char **argv)
{
	// Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	// Name and create the Window
	glutCreateWindow("CS480 Graphics");
	// Initialize GLEW to prepare work with shaders
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "[F] GLEW NOT INITIALIZED: ";
		std::cerr << glewGetErrorString(status) << std::endl;
		return -1;
	}
	// Set all of the callbacks to GLUT that we need
	glutDisplayFunc(render);// Called when its time to display
	glutReshapeFunc(reshape);// Called if the window is resized
	glutIdleFunc(update);// Called if there is nothing else to do
	glutKeyboardFunc(keyboard);// Called if there is keyboard input
	glutMouseFunc(mouse);// Called if there is mouse input
	glutSpecialFunc(specialInput);
	// Set up menu
	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Start Rotation", 1);
	glutAddMenuEntry("Stop Rotation", 2);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// Initialize all of our resources(shaders, geometry)
	bool init = initialize();
	if (init)
	{
		t1 = std::chrono::high_resolution_clock::now();
		glutMainLoop();
	}
	return 0;
}

bool initialize()
{
	engine = new Universe();
	Object* center = new Object(engine, "Planet", 0.0, 0.0, program, width, height);
	engine->setCenter(center);
	Object* moon = new Object(engine, "Moon", 0.0, 0.0, program, width, height);
	moon->eventMove(center);
	//glBindAttribLocation(program, 0, "v_position");
	//glBindAttribLocation(program, 1, "textCoord");
	return true;
}

float getDT()
{
	float ret;
	t2 = std::chrono::high_resolution_clock::now();
	ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2 - t1).count();
	t1 = std::chrono::high_resolution_clock::now();
	return ret;
}

void reshape(int n_w, int n_h)
{
	width = n_w;
	height = n_h;
	//Change the viewport to be correct
	glViewport(0, 0, width, height);
	//Update the projection matrix as well
	engine->getCenter()->reshape(width, height);
}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
	// Handle keyboard input
	if (key == 27)//ESC
		exit(0);
	else if (key == 97)//A
		engine->getCenter()->rotation.inverse = !engine->getCenter()->rotation.inverse;//reverse rotation
}

void specialInput(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			engine->getCenter()->rotation.inverse = true;
			break;
		case GLUT_KEY_RIGHT:
			engine->getCenter()->rotation.inverse = false;
			break;
	}
}

void mouse(int button, int state, int x_pos, int y_pos)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		leftClick = GLUT_LEFT_BUTTON;
	if (state == GLUT_UP && button == leftClick)
	{
		leftClick = 0;
		engine->getCenter()->rotation.inverse = !engine->getCenter()->rotation.inverse;//reverses direction
	}
}

void mainMenu(int value)
{
	switch (value)
	{
		case 1://Start rotating
			engine->getCenter()->orbit.active = true;
			break;
		case 2://Pause rotating
			engine->getCenter()->orbit.active = false;
			break;
		case 3://Exit
			exit(0);
			break;
	}
}

void render()
{
	//clear the screen
	glClearColor(0.0, 0.0, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	engine->getCenter()->render(program, width, height);
	//swap the buffers
	glutSwapBuffers();
}

void update()
{
	float dt = getDT();// if you have anything moving, use dt.
	engine->getCenter()->update(dt);
	glutPostRedisplay();//call the display callback
}