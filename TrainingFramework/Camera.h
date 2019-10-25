#pragma once
#include "../Utilities/MyMath.h"

class Camera
{
public:
	Camera();
	~Camera();
	Camera(int id, Vector3 position, Vector3 target, Vector3 up, float fovy, float nearPlan, float farPlan, float moveSpeed, float rotateSpeed);

	void SetId(int id);
	int GetId();
	Matrix GetWorldMatrix();
	Matrix GetViewMatrix();
	Matrix GetProjecttionMatrix();
	void Update(float deltaTime);
	void MoveForward(float deltaTime);
	void MoveLeft(float deltaTime);
	void RotateLeft(float deltaTime);
	void RotateUp(float deltaTime);
	static float currentPitchAngle;

private:
	int m_id;
	Vector3 m_position;
	Vector3 m_target;
	Vector3 m_up;
	float m_fovy;
	float m_near;
	float m_far;
	float m_moveSpeed;
	float m_rotateSpeed;
	Matrix m_worldMatrix;
	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;
	bool m_isChanged;

	Matrix RotationMatrixAroundX(float angle);
	Matrix RotationMatrixAroundY(float angle);
	void CalculateWorldMatrix();
	void CalculateViewMatrix();
};