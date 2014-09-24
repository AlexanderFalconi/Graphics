#include <GL/glew.h> // glew must be included before the main gl libs
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <cstdlib>
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "include/dllist.h"
#include "include/universe.h"
#include "include/object.h"
using namespace std;

//--Data types
//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
	GLfloat position[3];
	GLfloat color[3];
};

//--Evil Global variables
//Just for this example!
int width = 640, height = 480;// Window size
bool rotation = true;//handles rotation direction
bool action = true;//starts and pauses rotation
int leftClick = 0;//handle left clicking
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_color;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

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
void cleanUp();

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
	cleanUp();
	return 0;
}

bool initialize()
{
	// Initialize basic geometry and shaders for this example
	Vertex geometry[] = { { { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, -1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { -1.0, -1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { -1.0, -1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } }
	};
	// Create a Vertex Buffer object to store this vertex info on the GPU
	glGenBuffers(1, &vbo_geometry);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);
	//--Geometry done
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); 
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//Shader Sources
	std::ifstream file("shader.vert");
	std::string vsContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char *vs = vsContent.c_str();
	file.close();
	file.open("shader.frag");
	std::string fsContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char *fs = fsContent.c_str();
	//compile the shaders
	GLint shader_status;
	// Vertex shader first
	glShaderSource(vertex_shader, 1, &vs, NULL);
	glCompileShader(vertex_shader);
	//check the compile status
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
	if (!shader_status)
	{
		std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
		return false;
	}
	// Now the Fragment shader
	glShaderSource(fragment_shader, 1, &fs, NULL);
	glCompileShader(fragment_shader);
	//check the compile status
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
	if (!shader_status)
	{
		std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
		return false;
	}
	//Now we link the 2 shader objects into a program
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	//check if everything linked ok
	glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
	if (!shader_status)
	{
		std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
		return false;
	}
	//Start loading objects
	engine = new Universe();
	Object* center = new Object(engine, "Planet", 0.0, 0.0, program, width, height);
	engine->setCenter(center);
	Object* moon = new Object(engine, "Moon", 0.0, 0.0, program, width, height);
	moon->eventMove(center);
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//and its done
	return true;
}

void cleanUp()
{
	// Clean up, Clean up
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_geometry);
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
	projection = glm::perspective(45.0f, float(width) / float(height), 0.01f, 100.0f);
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
	engine->getCenter()->render(program, vbo_geometry, width, height);
	//swap the buffers
	glutSwapBuffers();
}

void update()
{
	float dt = getDT();// if you have anything moving, use dt.
	engine->getCenter()->update(dt);
	glutPostRedisplay();//call the display callback
}