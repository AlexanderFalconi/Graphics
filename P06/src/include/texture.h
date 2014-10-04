// Copyright 2011 Etay Meiri
#ifndef TEXTURE_H
#define	TEXTURE_H
#include <string>
#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);
    bool Load(GLuint texture);
    void Bind(GLenum TextureTarget);
    void Unbind();

private:
    std::string m_fileName;
    GLenum m_textureTarget;
	GLuint vbo_texture, loc_texture;// VBO handle for our texture
    Magick::Image* m_pImage;
    Magick::Blob m_blob;
};

#endif	/* TEXTURE_H */