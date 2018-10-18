#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#include "../lib/glad.h" //glad needs to be included before glfw
#include "../gl.hpp"

using namespace std;

Texture::Texture(const char* file, unsigned int wrapS, unsigned int wrapT, unsigned int minFilter, unsigned int magFilter)
{
	int nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file, &width, &height, &nrComponents, 0);
	
	if(data)
	{
		unsigned int format;
		switch(nrComponents)
		{
			case 1:
				format = GL_RED;
				break;
			case 2:
				format = GL_RG;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				format = 69;
				break;
		}
		
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	}
	else
	{
		cerr << "Failed to load texture: \"" << file << "\"" << endl;
	}
	
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int active) const
{
	glActiveTexture(GL_TEXTURE0 + active);
	glBindTexture(GL_TEXTURE_2D, id);
}

unsigned int Texture::getID() const
{
	return id;
}

int Texture::getWidth() const
{
	return width;
}

int Texture::getHeight() const
{
	return height;
}
