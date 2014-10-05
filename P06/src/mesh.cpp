//Copyright 2011 Etay Meiri
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>
#include "include/mesh.h"

Mesh::Mesh(const std::string& fileName)
{
    float *vertexArray, *normalArray, *uvArray;//Prep initial arrays
    Assimp::Importer importer;//Prep importer object
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate);//Import object
    aiMesh *mesh = scene->mMeshes[0];//Get meshes from importer object
    vertexArray = new float[mesh->mNumFaces*3*3];//Dynamically create vertices array
    normalArray = new float[mesh->mNumFaces*3*3];//Dynamically create normals array
    uvArray = new float[mesh->mNumFaces*3*2];//Dynamically create uv array
    m_numIndices = mesh->mNumFaces;
    for(unsigned int i=0;i<mesh->mNumFaces;i++)
    {//For each face in the mesh
        const aiFace& face = mesh->mFaces[i];//Get face object
        for(int j=0;j<3;j++)
        {//For each aiVector in the face
            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
            memcpy(uvArray,&uv,sizeof(float)*2);
            uvArray+=2;
            aiVector3D normal = mesh->mNormals[face.mIndices[j]];
            memcpy(normalArray,&normal,sizeof(float)*3);
            normalArray+=3;
            aiVector3D pos = mesh->mVertices[face.mIndices[j]];
            memcpy(vertexArray,&pos,sizeof(float)*3);
            vertexArray+=3;
        }
    }
    uvArray-=mesh->mNumFaces*3*2;
    normalArray-=mesh->mNumFaces*3*3;
    vertexArray-=mesh->mNumFaces*3*3;
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_numIndices*3*3, &vertexArray[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_numIndices*3*2, &uvArray[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_numIndices*3*3, &normalArray[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //INDICES NEEDED?
    glBindVertexArray(0);      
}

void Mesh::Draw()
{
    glBindVertexArray(m_vertexArrayObject);
    glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}