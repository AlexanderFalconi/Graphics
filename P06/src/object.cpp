#include <GL/glew.h> // glew must be included before the main gl libs
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <cstdlib>
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include "include/universe.h"
#include "include/object.h"

struct Vertex
{
    GLfloat position[3];
    GLfloat color[2];
    GLfloat normals[3];
};

Object::Object(Universe* d, GLint program, int width, int height)
{
	daemon = d;
	environment = nullptr;
	initialize(program, width, height);
}

Object::Object (Universe *d, std::string obName, float obMass, float obDensity, GLint program, int width, int height)
{
	daemon = d;
	environment = nullptr;
	name = obName;
	mass = obMass;
	density = obDensity;
	initialize(program, width, height);
}

bool Object::initialize(GLuint program, int width, int height)
{
	float *vertexArray;
	float *normalArray;
	float *uvArray;
	int numVerts;
	Assimp::Importer importer; 
	const aiScene *scene = importer.ReadFile("board.obj", aiProcess_Triangulate);
	aiMesh *mesh = scene->mMeshes[0];
	vertices_size = mesh->mNumVertices;
    cout << std::to_string(vertices_size) << std::endl;
	numVerts = mesh->mNumFaces*3;
	vertexArray = new float[mesh->mNumFaces*3*3];
	normalArray = new float[mesh->mNumFaces*3*3];
	uvArray = new float[mesh->mNumFaces*3*2];
	for(unsigned int i=0;i<mesh->mNumFaces;i++)
	{
		const aiFace& face = mesh->mFaces[i];
		for(int j=0;j<3;j++)
		{
			aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
			memcpy(uvArray,&uv,sizeof(float)*2);
			uvArray+=2;
			aiVector3D normal = mesh->mNormals[face.mIndices[j]];
			memcpy(normalArray,&normal,sizeof(float)*3);
			normalArray+=3;
			aiVector3D pos = mesh->mVertices[face.mIndices[j]];
			memcpy(vertexArray,&pos,sizeof(float)*3);
			vertexArray+=3;
			vertices_size++;
		}
	}
	uvArray-=mesh->mNumFaces*3*2;
	normalArray-=mesh->mNumFaces*3*3;
	vertexArray-=mesh->mNumFaces*3*3;
	//TRYING TO START TEXTURE STUFF HERE
	m_fileName = "board.png";
	try 
	{
		m_pImage = new Magick::Image(m_fileName);
		m_pImage->write(&m_blob, "RGBA");
	}
	catch(Magick::Error& Error) 
	{
		std::cout << "Error loading texture '"<<m_fileName<<"': "<<Error.what()<<std::endl;
		return false;
	}
	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	void Texture::Bind(GLenum TextureUnit)
	{
		glActiveTexture(TextureUnit);
		glBindTexture(m_textureTarget, m)textureObj);
	}

	//STILL GOING>..
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,vertexArray);
	glNormalPointer(GL_FLOAT,0,normalArray);
	glClientActiveTexture(GL_TEXTURE0_ARB);
	glTexCoordPointer(2,GL_FLOAT,0,uvArray);
	glDrawArrays(GL_TRIANGLES,0,numVerts);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	// Create a Vertex Buffer object to store this vertex info on the GPU
	glGenBuffers(1, &vbo_geometry);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumFaces*3*3 * sizeof(float), &vertexArray[0], GL_STATIC_DRAW);
	glGenBuffers(1, &vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumFaces*3*2 * sizeof(float), &uvArray[0], GL_STATIC_DRAW);
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

vector<Object*> Object::getInventory()
{
	return inventory;
}

void Object::render(GLuint program, int width, int height)
{
	//premultiply the matrix for this example
	mvp = projection * view * model;
	//enable the shader program
	glUseProgram(program);
	//upload the matrix to the shader
	glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
	//set up the Vertex Buffer Object so it can be drawn
	glEnableVertexAttribArray(loc_position);	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
	//set pointers into the vbo for each of the attributes(position and color)
	glVertexAttribPointer(loc_position,//location of attribute
		3,//number of elements
		GL_FLOAT,//type
		GL_FALSE,//normalized?
		0,//stride
		(void*)0);//offset
	glEnableVertexAttribArray(loc_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glVertexAttribPointer(loc_color,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);
	glDrawArrays(GL_TRIANGLES, 0, vertices_size);//mode, starting index, count
	//clean up
	glDisableVertexAttribArray(loc_position);
	glDisableVertexAttribArray(loc_color);
	//Iterate through inventory
	for(unsigned int i=0; i<inventory.size(); i++)
		inventory[i]->render(program, width, height);
}

void Object::reshape(int width, int height)
{
	projection = glm::perspective(45.0f, float(width) / float(height), 0.01f, 100.0f);
	//Iterate through inventory
	for(unsigned int i=0; i<inventory.size(); i++)
		inventory[i]->reshape(width, height);
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
	for(unsigned int i=0; i<inventory.size(); i++)
		inventory[i]->update(dt);
}

void Object::eventMove(Object *ob)
{
	if(environment != nullptr)
		environment->release();//this);
	environment = ob;
	ob->receive(this);
}

void Object::receive(Object *ob)
{
	inventory.push_back(ob);
}

void Object::release()
{
	//TODO: Need a search algorithm to remove
	inventory.pop_back();
}