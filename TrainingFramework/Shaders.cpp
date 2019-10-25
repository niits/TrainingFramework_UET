#include "Shaders.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	strcpy(fileVS, fileVertexShader);
	strcpy(fileFS, fileFragmentShader);
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shaders::SetId(int id)
{
	m_id = id;
}

int Shaders::GetId()
{
	return m_id;
}