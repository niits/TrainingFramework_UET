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
#include "Model.h"

int Globals::keyPressed = 0;
std::shared_ptr<Camera> gCamera;

GLuint vboId, iboId;
Shaders* shader;
Matrix WVP, W, WVP1, W1, WVP0, W0;

Model* model, * model_woman_1, * model_woman_2;
Texture* texture_map;
Texture* texture_grass;
Texture* texture_dirt;
Texture* texture_rock;

Texture* texture_woman_1;
Texture* texture_woman_2;

Shaders* shader_woman;

int Init(ESContext* esContext)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//gCamera = std::make_shared<Camera>(0,
	//	Vector3(0.0f, 1.0f, 3.0f), 
	//	Vector3(0.0f, 0.0f, 0.0f), 
	//	Vector3(0.0f, 1.0f, 0.0f), 
	//	1.0f, 0.1f, 500.0f, 10.0f, 2.0f);

	int id = 0;
	Vector3 position(0.0f, 1.0f, 3.0f);
	Vector3 target(0.0f, 0.0f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);
	float fovy = 1.0f;
	float nearPlan = 0.1f;
	float farPlan = 500.0f;
	float moveSpeed = 100.0f; // origin 10.0f
	float rotateSpeed = 2.0f;

	gCamera = std::make_shared<Camera>(id, position, target, up, fovy, nearPlan, farPlan, moveSpeed, rotateSpeed);

	model = new Model();
	model->LoadNfg("../Resources/Models/Terrain.nfg");

	model_woman_1 = new Model();
	model_woman_1->LoadNfg("../Resources/Models/Woman1.nfg");
	model_woman_2 = new Model();
	model_woman_2->LoadNfg("../Resources/Models/Woman2.nfg");

	// Init indices data
	// unsigned int indicesData[] = { 0, 1, 2 };

	// Push indices data to GPU
	//glGenBuffers(1, &iboId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indicesData, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Init vertex data
	Vertex verticesData[4];
	verticesData[0].pos = Vector3(-1.0f, 1.0f, 0.0f);
	verticesData[1].pos = Vector3(-1.0f, -1.0f, 0.0f);
	verticesData[2].pos = Vector3(1.0f, -1.0f, 0.0f);
	verticesData[3].pos = Vector3(1.0f, 1.0f, 0.0f);

	verticesData[0].uv = Vector2(0.0f, 1.0f);
	verticesData[1].uv = Vector2(0.0f, 0.0f);
	verticesData[2].uv = Vector2(1.0f, 1.0f);
	verticesData[3].uv = Vector2(1.0f, 1.0f);

	// Push vertices data to GPU
	//glGenBuffers(1, &vboId);
	//glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Init indices data
	unsigned int indicesData[] = { 0, 1, 2, 2, 3, 0 };

	// Push indices data to GPU
	//glGenBuffers(1, &iboId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indicesData, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Init shader
	shader = new Shaders();
	shader->Init("../Resources/Shaders/mObject.vs", "../Resources/Shaders/mObject.fs");

	shader_woman = new Shaders();
	shader_woman->Init("../Resources/Shaders/Object.vs", "../Resources/Shaders/Object.fs");

	texture_map = new Texture();
	texture_map->InitTexture2D("../Resources/Textures/Terrain_blendmap.tga");
	texture_grass = new Texture();
	texture_grass->InitTexture2D("../Resources/Textures/Grass.tga");
	texture_dirt = new Texture();
	texture_dirt->InitTexture2D("../Resources/Textures/Dirt.tga");
	texture_rock = new Texture();
	texture_rock->InitTexture2D("../Resources/Textures/Rock.tga");

	texture_woman_1 = new Texture();
	texture_woman_1->InitTexture2D("../Resources/Textures/Woman1.tga");
	texture_woman_2 = new Texture();
	texture_woman_2->InitTexture2D("../Resources/Textures/Woman2.tga");
	// Init WVP matrix
	W = Matrix().SetScale(5.0f, 5.0f, 5.0f)
		* Matrix().SetRotationY(TO_RAD(30))
		* Matrix().SetTranslation(-5.0f, -2.5f, 5.0f);
	W1 = Matrix().SetScale(5.0f, 5.0f, 5.0f)
		* Matrix().SetRotationY(TO_RAD(-30))
		* Matrix().SetTranslation(5.0f, -2.5f, 5.0f);
	W0 = Matrix().SetScale(5.0f, 5.0f, 5.0f)
		* Matrix().SetRotationY(TO_RAD(120))
		* Matrix().SetTranslation(-5.0f, -2.5f, 5.0f);
	Matrix V = gCamera->GetViewMatrix();
	Matrix P = gCamera->GetProjecttionMatrix();
	WVP = W * V * P;
	WVP1 = W1 * V * P;
	WVP0 = W1 * V * P;
	return 0;

}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Draw something here

	glUseProgram(shader->program);
	//glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBindBuffer(GL_ARRAY_BUFFER, model->GetVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetIboId());

	GLint positionAtrribute0 = glGetAttribLocation(shader->program, "a_posL");
	if (positionAtrribute0 != -1)
	{
		glEnableVertexAttribArray(positionAtrribute0);
		glVertexAttribPointer(positionAtrribute0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSET_POS);
	}

	GLint uvAtrribute0 = glGetAttribLocation(shader->program, "a_uv");
	if (uvAtrribute0 != -1)
	{
		glEnableVertexAttribArray(uvAtrribute0);
		glVertexAttribPointer(uvAtrribute0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSET_UV);
	}

	GLint wvpUniform0 = glGetUniformLocation(shader->program, "u_wvpMatrix");
	if (wvpUniform0 != -1)
	{
		glUniformMatrix4fv(wvpUniform0, 1, GL_FALSE, &WVP.m[0][0]);
	}

	GLint textureUniform0 = glGetUniformLocation(shader->program, "u_texture_map");
	if (textureUniform0 != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_map->GetTextureId());
		//glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(textureUniform0, 0);
	}

	GLint textureUniform01 = glGetUniformLocation(shader->program, "u_texture_grass");
	if (textureUniform01 != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_grass->GetTextureId());
		glUniform1i(textureUniform01, 1);
	}

	GLint textureUniform02 = glGetUniformLocation(shader->program, "u_texture_dirt");
	if (textureUniform02 != -1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_dirt->GetTextureId());
		glUniform1i(textureUniform02, 2);
	}

	GLint textureUniform03 = glGetUniformLocation(shader->program, "u_texture_rock");
	if (textureUniform03 != -1)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture_rock->GetTextureId());
		glUniform1i(textureUniform03, 3);
	}

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, model->GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, model_2->GetNumberOfIndices(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

	glUseProgram(shader_woman->program);
	//glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBindBuffer(GL_ARRAY_BUFFER, model_woman_1->GetVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_woman_1->GetIboId());

	GLint positionAtrribute = glGetAttribLocation(shader_woman->program, "a_posL");
	if (positionAtrribute != -1)
	{
		glEnableVertexAttribArray(positionAtrribute);
		glVertexAttribPointer(positionAtrribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSET_POS);
	}

	GLint uvAtrribute = glGetAttribLocation(shader_woman->program, "a_uv");
	if (uvAtrribute != -1)
	{
		glEnableVertexAttribArray(uvAtrribute);
		glVertexAttribPointer(uvAtrribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSET_UV);
	}

	GLint wvpUniform = glGetUniformLocation(shader_woman->program, "u_wvpMatrix");
	if (wvpUniform != -1)
	{
		glUniformMatrix4fv(wvpUniform, 1, GL_FALSE, &WVP0.m[0][0]);
	}

	GLint textureUniform = glGetUniformLocation(shader_woman->program, "u_texture");
	if (textureUniform != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_woman_1->GetTextureId());
		glUniform1i(textureUniform, 0);
	}

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, model_woman_1->GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
	//Draw second object
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

	glUseProgram(shader_woman->program);
	//glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBindBuffer(GL_ARRAY_BUFFER, model_woman_2->GetVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_woman_2->GetIboId());

	GLint positionAtrribute1 = glGetAttribLocation(shader_woman->program, "a_posL");
	if (positionAtrribute1 != -1)
	{
		glEnableVertexAttribArray(positionAtrribute1);
		glVertexAttribPointer(positionAtrribute1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSET_POS);
	}

	GLint uvAtrribute1 = glGetAttribLocation(shader_woman->program, "a_uv");
	if (uvAtrribute1 != -1)
	{
		glEnableVertexAttribArray(uvAtrribute1);
		glVertexAttribPointer(uvAtrribute1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSET_UV);
	}

	GLint wvpUniform1 = glGetUniformLocation(shader_woman->program, "u_wvpMatrix");
	if (wvpUniform1 != -1)
	{
		glUniformMatrix4fv(wvpUniform1, 1, GL_FALSE, &WVP1.m[0][0]);
	}

	GLint textureUniform1 = glGetUniformLocation(shader_woman->program, "u_texture");
	if (textureUniform1 != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_woman_2->GetTextureId());
		glUniform1i(textureUniform1, 0);
	}

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, model_woman_2->GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void Update(ESContext* esContext, float deltaTime)
{
	// TODO: Update scene objects here
	gCamera->Update(deltaTime);
	Matrix V = gCamera->GetViewMatrix();
	Matrix P = gCamera->GetProjecttionMatrix();
	WVP = W * V * P;
	WVP0 = W0 * V * P;
	WVP1 = W1 * V * P;
}

void OnKeyEvent(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	// TODO: Handle key events
	// printf("%d\n", bIsPressed);
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

void OnMouseEvent(ESContext* esContext, unsigned char type, int x, int y)
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

