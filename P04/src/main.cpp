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
#include "include/object.h"
#include "include/universe.h"
using namespace std;

//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
	GLfloat position[3];
	GLfloat color[3];
};

Universe daemon("Sun", 640, 480);

//--Resource management
bool initialize();
void cleanUp();

int main(int argc, char **argv)
{
	// Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(daemon.window.width, daemon.window.height);
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
	glutDisplayFunc(daemon.render);// Called when its time to display
	glutReshapeFunc(daemon.reshape);// Called if the window is resized
	glutIdleFunc(daemon.update);// Called if there is nothing else to do
	glutKeyboardFunc(daemon.keyboard);// Called if there is keyboard input
	glutMouseFunc(daemon.mouse);// Called if there is mouse input
	glutSpecialFunc(daemon.specialInput);
	// Set up menu
	glutCreateMenu(daemon.mainMenu);
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
	glGenBuffers(1, &daemon.vbo_geometry);
	glBindBuffer(GL_ARRAY_BUFFER, daemon.vbo_geometry);
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
	daemon.center = new Object("Planet", 0.0, 0.0);
	Object moon("Moon", 0.0, 0.0);
	daemon.center->receive(moon);
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