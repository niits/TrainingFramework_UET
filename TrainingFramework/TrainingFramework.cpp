// Feel free to contact me for support: hung.hoangmanh2@gameloft.com

#include <stdio.h>
#include <conio.h>
#include <memory>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Globals.h"
#include "Camera.h"
#include "Vertex.h"
#include "Shaders.h"
#include "Texture.h"

int Globals::keyPressed = 0;
std::shared_ptr<Camera> gCamera;

GLuint vboId, iboId;
Shaders* shader;
Matrix WVP, W;

int Init(ESContext *esContext)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	gCamera = std::make_shared<Camera>(0, Vector3(0.0f, 1.0f, 3.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), 1.0f, 0.1f, 500.0f, 10.0f, 2.0f);

	// TODO: Initialize data here

	// Init vertices data
	Vertex verticesData[3];
	verticesData[0].pos = Vector3(0.0f, 1.0f, 0.0f);
	verticesData[1].pos = Vector3(-1.0f, -1.0f, 0.0f);
	verticesData[2].pos = Vector3(1.0f, -1.0f, 0.0f);

	// Push vertices data to GPU
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Init indices data
	unsigned int indicesData[] = { 0, 1, 2 };

	// Push indices data to GPU
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Init shader
	shader = new Shaders();
	shader->Init("../Resources/Shaders/Object.vs", "../Resources/Shaders/Object.fs");

	// Init WVP matrix
	W = Matrix().SetScale(1.0f, 1.0f, 1.0f)
		* Matrix().SetRotationY(TO_RAD(45))
		* Matrix().SetTranslation(0.0f, 0.0f, 0.0f);
	Matrix V = gCamera->GetViewMatrix();
	Matrix P = gCamera->GetProjecttionMatrix();
	WVP = W * V * P;

	return 0;

}

void Draw(ESContext *esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Draw something here

	glUseProgram(shader->program);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	GLint positionAtrribute = glGetAttribLocation(shader->program, "a_posL");
	if (positionAtrribute != -1)
	{
		glEnableVertexAttribArray(positionAtrribute);
		glVertexAttribPointer(positionAtrribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) OFFSET_POS);
	}

	GLint wvpUniform = glGetUniformLocation(shader->program, "u_wvpMatrix");
	if (wvpUniform != -1)
	{
		glUniformMatrix4fv(wvpUniform, 1, GL_FALSE, &WVP.m[0][0]);
	}

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext *esContext, float deltaTime)
{
	// TODO: Update scene objects here
	gCamera->Update(deltaTime);
	Matrix V = gCamera->GetViewMatrix();
	Matrix P = gCamera->GetProjecttionMatrix();
	WVP = W * V * P;
}

void OnKeyEvent(ESContext *esContext, unsigned char key, bool bIsPressed)
{
	// TODO: Handle key events
	if (bIsPressed)
	{
		switch (key)
		{
		case 'W':
			Globals::keyPressed |= MOVE_FORWARD;
			break;
		case 'S':
			Globals::keyPressed |= MOVE_BACK;
			break;
		case 'A':
			Globals::keyPressed |= MOVE_LEFT;
			break;
		case 'D':
			Globals::keyPressed |= MOVE_RIGHT;
			break;
		case VK_LEFT:
			Globals::keyPressed |= ROTATE_LEFT;
			break;
		case VK_RIGHT:
			Globals::keyPressed |= ROTATE_RIGHT;
			break;
		case VK_UP:
			Globals::keyPressed |= ROTATE_UP;
			break;
		case VK_DOWN:
			Globals::keyPressed |= ROTATE_DOWN;
			break;
		}
	}
	else
	{
		switch (key)
		{
		case 'W':
			Globals::keyPressed ^= MOVE_FORWARD;
			break;
		case 'S':
			Globals::keyPressed ^= MOVE_BACK;
			break;
		case 'A':
			Globals::keyPressed ^= MOVE_LEFT;
			break;
		case 'D':
			Globals::keyPressed ^= MOVE_RIGHT;
			break;
		case VK_LEFT:
			Globals::keyPressed ^= ROTATE_LEFT;
			break;
		case VK_RIGHT:
			Globals::keyPressed ^= ROTATE_RIGHT;
			break;
		case VK_UP:
			Globals::keyPressed ^= ROTATE_UP;
			break;
		case VK_DOWN:
			Globals::keyPressed ^= ROTATE_DOWN;
			break;
		}
	}
}

void OnMouseEvent(ESContext *esContext, unsigned char type, int x, int y)
{
	// TODO: Handle mouse events
	switch (type)
	{
	case MOUSE_DOWN:
		printf("MOUSE_DOWN at [%d, %d]\n", x, y);
		break;
	case MOUSE_UP:
		printf("MOUSE_UP at [%d, %d]\n", x, y);
		break;
	case MOUSE_MOVE:
		printf("MOUSE_MOVE at [%d, %d]\n", x, y);
		break;
	default:
		break;
	}
}

void CleanUp()
{
	// TODO: Clean up memory
	gCamera.reset();
}

int main()
{
	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "HMH@Gameloft", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, OnKeyEvent);
	esRegisterMouseFunc(&esContext, OnMouseEvent);

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

