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

Object::Object(Universe* d, GLint program, int width, int height)
{
	daemon = d;
	environment = nullptr;
	inventory = new List();
	initialize(program, width, height);
}

Object::Object (Universe *d, std::string obName, float obMass, float obDensity, GLint program, int width, int height)
{
	daemon = d;
	environment = nullptr;
	inventory = new List();
	name = obName;
	mass = obMass;
	density = obDensity;
	initialize(program, width, height);
}

bool Object::initialize(GLuint program, int width, int height)
{
	//Now we set the locations of the attributes and uniforms
	loc_position = glGetAttribLocation(program,
		const_cast<const char*>("v_position"));
	if (loc_position == -1)
	{
		std::cerr << "[F] POSITION NOT FOUND" << std::endl;
		return false;
	}
	loc_color = glGetAttribLocation(program,
		const_cast<const char*>("v_color"));
	if (loc_color == -1)
	{
		std::cerr << "[F] V_COLOR NOT FOUND" << std::endl;
		return false;
	}
	loc_mvpmat = glGetUniformLocation(program,
		const_cast<const char*>("mvpMatrix"));
	if (loc_mvpmat == -1)
	{
		std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
		return false;
	}
	//--Init the view and projection matrices
	//  if you will be having a moving camera the view matrix will need to more dynamic
	//  ...Like you should update it before you render more dynamic 
	//  for this project having them static will be fine
	view = glm::lookAt(glm::vec3(0.0, 8.0, -16.0), //Eye Position
		glm::vec3(0.0, 0.0, 0.0), //Focus point
		glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

	projection = glm::perspective(45.0f, //the FoV typically 90 degrees is good which is what this is set to
		float(width) / float(height), //Aspect Ratio, so Circles stay Circular
		0.01f, //Distance to the near plane, normally a small value like this
		100.0f); //Distance to the far plane, 
	return true;
}

float Object::getMass()
{
	return mass;
}

float Object::getDensity()
{
	return density;
}

glm::mat4 Object::getModel()
{
	return model;
}

Object *Object::getEnvironment()
{
	return environment;
}

List *Object::getInventory()
{
	return inventory;
}

void Object::render(GLuint program, GLuint vbo_geometry, int width, int height)
{
	//premultiply the matrix for this example
	mvp = projection * view * model;
	//enable the shader program
	glUseProgram(program);
	//upload the matrix to the shader
	glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
	//set up the Vertex Buffer Object so it can be drawn
	glEnableVertexAttribArray(loc_position);	
	glEnableVertexAttribArray(loc_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
	//set pointers into the vbo for each of the attributes(position and color)
	glVertexAttribPointer(loc_position,//location of attribute
		3,//number of elements
		GL_FLOAT,//type
		GL_FALSE,//normalized?
		sizeof(Vertex),//stride
		0);//offset
	glVertexAttribPointer(loc_color,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, color));
	glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count
	//clean up
	glDisableVertexAttribArray(loc_position);
	glDisableVertexAttribArray(loc_color);
	//Iterate through inventory
	Object* nextOb = inventory->forNext();
	while (nextOb != nullptr)
	{
		nextOb->render(program, vbo_geometry, width, height);
		nextOb = inventory->forNext();
	}
}

void Object::update(float dt)
{
	orbit.angle += dt * M_PI / 2; //move through 90 degrees a second
	if (rotation.active)
	{//spin 90 degrees a second
		if(rotation.inverse)
			rotation.angle -= dt * M_PI / 2;//counter-clockwise
		else
			rotation.angle += dt * M_PI / 2;//clockwise
	}//else, don't spin
	if(environment == nullptr)
		model = glm::translate(glm::mat4(0.1f), glm::vec3(8.0 * sin(orbit.angle), 0.0, 8.0 * cos(orbit.angle))) * glm::rotate(glm::mat4(1.0f), rotation.angle, glm::vec3(0, 1, 0));
	else
		model = glm::translate(environment->getModel(), glm::vec3(8.0 * sin(orbit.angle), 0.0, 8.0 * cos(orbit.angle))) * glm::rotate(glm::mat4(1.0f), rotation.angle, glm::vec3(0, 1, 0));
	//Iterate through inventory
	Object* nextOb = inventory->forNext();
	while (nextOb != nullptr)
	{
		nextOb->update(dt);
		nextOb = inventory->forNext();
	}
}

void Object::eventMove(Object *ob)
{
	if(environment != nullptr)
		environment->release(this);
	environment = ob;
	ob->receive(this);
}

void Object::receive(Object *ob)
{
	inventory->push_back(ob);
}

void Object::release(Object *ob)
{
	//TODO: Need a search algorithm to remove
	inventory->pop_back();
}