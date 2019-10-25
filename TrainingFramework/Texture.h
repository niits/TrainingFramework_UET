#pragma once
#include "../Utilities/utilities.h"

class Texture
{
public:
	Texture();
	~Texture();
	void InitTexture2D(const char * fileName);
	GLuint GetTextureId();

private:
	GLuint m_textureId;
};