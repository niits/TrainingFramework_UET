#include "Model.h"
#include <stdio.h>
#include "Vertex.h"
#include <vector>

Model::Model()
{}

Model::~Model()
{}

void Model::LoadNfg(const char * fileName)
{
	FILE* pFile = fopen(fileName, "r");
	if (!pFile)
	{
		printf("Error opening file: %s\n", fileName);
		return;
	}

	fscanf(pFile, "NrVertices: %d\n", &m_numVertices);
	Vertex * verticesData = new Vertex[m_numVertices];
	for (int i = 0; i < m_numVertices; i++)
	{
		fscanf(pFile, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];\n",
			&verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z, &verticesData[i].normal.x, &verticesData[i].normal.y, &verticesData[i].normal.z,
			&verticesData[i].binormal.x, &verticesData[i].binormal.y, &verticesData[i].binormal.z, &verticesData[i].tangent.x, &verticesData[i].tangent.y, &verticesData[i].tangent.z,
			&verticesData[i].uv.x, &verticesData[i].uv.y);
		verticesData[i].pos.z += 0.5;
	}

	fscanf(pFile, "NrIndices: %d\n", &m_numIndices);
	unsigned int * indicesData = new unsigned int[m_numIndices];
	int idx = 0;
	int numTriangles = m_numIndices / 3;
	for (int i = 0; i < numTriangles; i++)
	{
		fscanf(pFile, "   %*d.    %d,    %d,    %d\n", &indicesData[idx], &indicesData[idx + 1], &indicesData[idx + 2]);
		idx += 3;
	}
	fclose(pFile);

	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_numVertices, verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, indicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] verticesData;
	delete[] indicesData;
}

void Model::SetId(int id)
{
	m_id = id;
}

GLuint Model::GetVboId()
{
	return m_vboId;
}

GLuint Model::GetIboId()
{
	return m_iboId;
}

int Model::GetNumberOfIndices()
{
	return m_numIndices;
}

int Model::GetId()
{
	return m_id;
}