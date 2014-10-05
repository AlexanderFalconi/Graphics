// Copyright 2011 Etay Meiri
#ifndef TEXTURE_H
#define	TEXTURE_H
#include <string>
#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

class Texture
{
public:
    Texture(const std::string& FileName);
    void Bind();
    void Unbind();

private:
    std::string m_fileName;
	GLuint m_texture;// VBO handle for our texture
    Magick::Image* m_pImage;
    Magick::Blob m_blob;
};

#endif	/* TEXTURE_H */