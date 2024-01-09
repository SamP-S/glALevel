#pragma once

#include "glfw3.h"
#include "BMP.h"

float greyTexture[12] = {
0.5f, 0.5f, 0.5f,	0.5f, 0.5f, 0.5f,
0.5f, 0.5f, 0.5f,	0.5f, 0.5f, 0.5f
};

float whiteTexture[12] = {
	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f
};

float checkerBoardTexture[12] = {
	0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
};

class Texture_2D
{
private:
	GLuint textureID;
	BMP bmp;

public:

	void GenerateEmptyTexture()
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void SetTexture(float pixels[], int width, int height)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void SetTexture(uint8_t pixels[], int width, int height)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}

	void SetTextureWrapping(GLenum target)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, target);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, target);
	}
	
	void SetBorderColour(float colour_RGBA[4])
	{
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour_RGBA);
	}

	void SetFiltering(GLenum target)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, target);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, target);
	}

	void SetupTexture(const char * location)
	{
		GenerateEmptyTexture();
		//bmp.read_bmp(location);
		bmp.load_bmp(location);
		SetTexture(bmp.pixels, bmp.width, bmp.height);
		SetFiltering(GL_NEAREST);
	}

	void DeleteTexure()
	{
		glDeleteTextures(1, &textureID);
	}
};