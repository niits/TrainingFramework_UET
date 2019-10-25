#include "Texture.h"
Texture::Texture()
{}
Texture::~Texture()
{}
void Texture::InitTexture2D(const char * fileName)
{
	int w, h, bpp;
	char * imgData = LoadTGA(fileName, &w, &h, &bpp);
	if(!imgData) {
		return;
	}
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	switch (bpp)
	{
	case 24:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		break;
	case 32:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		break;
	default:
		break;
	}
	delete[] imgData;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}
GLuint Texture::GetTextureId()
{
	return m_textureId;
}