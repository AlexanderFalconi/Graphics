// Copyright 2011 Etay Meiri
#ifndef MESH_H
#define	MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <ImageMagick-6/Magick++.h>

class Mesh
{
	public:
	    Mesh(const std::string& fileName);

		void Draw();

		virtual ~Mesh();
	protected:
	private:
		static const unsigned int NUM_BUFFERS = 4;
		void operator=(const Mesh& mesh) {}
		Mesh(const Mesh& mesh) {}

		GLuint m_vertexArrayObject;
		GLuint m_vertexArrayBuffers[NUM_BUFFERS];
		unsigned int m_numIndices;
};

#endif	/* MESH_H */