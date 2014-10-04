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

Mesh::Mesh(const std::string& FileName)
{
    m_fileName      = FileName;
}

bool Mesh::Load(GLuint position, GLuint mvpmat)
{
    loc_position = position;
    loc_mvpmat = mvpmat;
    Assimp::Importer importer;//Prep importer object
    const aiScene *scene = importer.ReadFile(m_fileName, aiProcess_Triangulate);//Import object
    aiMesh *mesh = scene->mMeshes[0];//Get meshes from importer object
    num_vertices = mesh->mNumVertices;//Set number of vertices
    num_faces = mesh->mNumFaces;
    vertexArray = new float[mesh->mNumFaces*3*3];//Dynamically create vertices array
    normalArray = new float[mesh->mNumFaces*3*3];//Dynamically create normals array
    uvArray = new float[mesh->mNumFaces*3*2];//Dynamically create uv array
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
    return true;
}

void Mesh::Bind(glm::mat4 mvp)
{
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));//upload the matrix to the shader
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,vertexArray);
    glNormalPointer(GL_FLOAT,0,normalArray);
    glClientActiveTexture(GL_TEXTURE0_ARB);
    glTexCoordPointer(2,GL_FLOAT,0,uvArray);
    glDrawArrays(GL_TRIANGLES,0,num_vertices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glEnableVertexAttribArray(0);    
    glVertexAttribPointer(0,//location of attribute
        3,//number of elements
        GL_FLOAT,//type
        GL_FALSE,//normalized?
        0,//stride
        (void*)0);//offset

}

int Mesh::getNumVertices()
{
    return num_vertices;
}

void Mesh::Unbind()
{
    glDisableVertexAttribArray(0);//clean up
}
