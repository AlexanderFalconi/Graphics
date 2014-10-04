// Copyright 2011 Etay Meiri
#ifndef MESH_H
#define	MESH_H

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

class Mesh
{
	public:
		//Vertex(const glm:vec3& pos, const glm::vec2& textCoord)
		//{
			//this->pos = pos;
			//this->textCoord = textCoord;
		//}
	    Mesh(const std::string& FileName);
	    bool Load(GLuint position, GLuint mvpmat);
	    void Bind(glm::mat4 mvp);
	    void Unbind();
	    int getNumVertices();

	private:
    	float *vertexArray, *normalArray, *uvArray;//Prep initial arrays
	    std::string m_fileName;
	    glm::vec3 pos;
	    glm::vec2 textCoord;
		int num_vertices, num_faces;
		GLuint vbo_geometry, loc_position, loc_mvpmat;//modelviewprojection matrix in the shader
};

#endif	/* MESH_H */