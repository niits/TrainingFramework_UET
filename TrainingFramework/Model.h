#pragma once
#include "../Utilities/utilities.h"

class Model
{
public:
	Model();
	~Model();
	void LoadNfg(const char * fileName);
	void SetId(int id);
	GLuint GetVboId();
	GLuint GetIboId();
	int GetNumberOfIndices();
	int GetId();
	
private:
	int m_id;
	GLuint m_vboId;
	GLuint m_iboId;
	int m_numVertices;
	int m_numIndices;
};