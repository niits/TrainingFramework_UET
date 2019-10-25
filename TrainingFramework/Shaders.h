#pragma once
#include <vector>
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
	void SetId(int id);
	int GetId();

private:
	int m_id;
};