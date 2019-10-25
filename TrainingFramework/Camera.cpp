#include "Camera.h"
#include "Globals.h"

float Camera::currentPitchAngle = 0;

Camera::Camera() : m_position(0, 0, 0), m_target(0, 0, -1), m_up(0, 1, 0), m_fovy(1.0), m_near(0.1), m_far(500), m_moveSpeed(10), m_rotateSpeed(2), m_isChanged(true)
{
}

Camera::~Camera()
{
}

Camera::Camera(int id, Vector3 position, Vector3 target, Vector3 up, float fovy, float nearPlan, float farPlan, float moveSpeed, float rotateSpeed)
	: m_id(id), m_position(position), m_target(target), m_up(up), m_fovy(fovy), m_near(nearPlan), m_far(farPlan), m_moveSpeed(moveSpeed), m_rotateSpeed(rotateSpeed), m_isChanged(true)
{
	CalculateWorldMatrix();
	CalculateViewMatrix();
	m_projectionMatrix.SetPerspective(fovy, (float)Globals::screenWidth / Globals::screenHeight, nearPlan, farPlan);
}

void Camera::SetId(int id)
{
	m_id = id;
}

int Camera::GetId()
{
	return m_id;
}

void Camera::CalculateWorldMatrix()
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = m_up.Cross(zaxis).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis).Normalize());

	m_worldMatrix.m[0][0] = xaxis.x;		m_worldMatrix.m[0][1] = xaxis.y;		m_worldMatrix.m[0][2] = xaxis.z;		m_worldMatrix.m[0][3] = 0.0f;
	m_worldMatrix.m[1][0] = yaxis.x;		m_worldMatrix.m[1][1] = yaxis.y;		m_worldMatrix.m[1][2] = yaxis.z;		m_worldMatrix.m[1][3] = 0.0f;
	m_worldMatrix.m[2][0] = zaxis.x;		m_worldMatrix.m[2][1] = zaxis.y;		m_worldMatrix.m[2][2] = zaxis.z;		m_worldMatrix.m[2][3] = 0.0f;
	m_worldMatrix.m[3][0] = m_position.x;	m_worldMatrix.m[3][1] = m_position.y;	m_worldMatrix.m[3][2] = m_position.z;	m_worldMatrix.m[3][3] = 1.0f;
	m_isChanged = false;
}

void Camera::CalculateViewMatrix()
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = m_up.Cross(zaxis).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis).Normalize());

	m_viewMatrix.m[0][0] = xaxis.x;					m_viewMatrix.m[0][1] = yaxis.x;					m_viewMatrix.m[0][2] = zaxis.x;					m_viewMatrix.m[0][3] = 0.0f;
	m_viewMatrix.m[1][0] = xaxis.y;					m_viewMatrix.m[1][1] = yaxis.y;					m_viewMatrix.m[1][2] = zaxis.y;					m_viewMatrix.m[1][3] = 0.0f;
	m_viewMatrix.m[2][0] = xaxis.z;					m_viewMatrix.m[2][1] = yaxis.z;					m_viewMatrix.m[2][2] = zaxis.z;					m_viewMatrix.m[2][3] = 0.0f;
	m_viewMatrix.m[3][0] = -m_position.Dot(xaxis);	m_viewMatrix.m[3][1] = -m_position.Dot(yaxis);	m_viewMatrix.m[3][2] = -m_position.Dot(zaxis);	m_viewMatrix.m[3][3] = 1.0f;
	m_isChanged = false;
}

Matrix Camera::GetWorldMatrix()
{
	if (m_isChanged)
	{
		CalculateWorldMatrix();
		m_isChanged = false;
	}
	return m_worldMatrix;
}

Matrix Camera::GetViewMatrix()
{
	if (m_isChanged)
	{
		CalculateViewMatrix();
		m_isChanged = false;
	}
	return m_viewMatrix;
}

Matrix Camera::GetProjecttionMatrix()
{
	return m_projectionMatrix;
}

void Camera::Update(float deltaTime)
{
	if (Globals::keyPressed & MOVE_FORWARD)
	{
		MoveForward(deltaTime);
	}
	if (Globals::keyPressed & MOVE_BACK)
	{
		MoveForward(-deltaTime);
	}
	if (Globals::keyPressed & MOVE_LEFT)
	{
		MoveLeft(deltaTime);
	}
	if (Globals::keyPressed & MOVE_RIGHT)
	{
		MoveLeft(-deltaTime);
	}
	if (Globals::keyPressed & ROTATE_LEFT)
	{
		RotateLeft(deltaTime);
	}
	if (Globals::keyPressed & ROTATE_RIGHT)
	{
		RotateLeft(-deltaTime);
	}
	if (Globals::keyPressed & ROTATE_UP)
	{
		RotateUp(deltaTime);
	}
	if (Globals::keyPressed & ROTATE_DOWN)
	{
		RotateUp(-deltaTime);
	}
}

void Camera::MoveForward(float deltaTime)
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 deltaMove = -zaxis * deltaTime * m_moveSpeed;
	m_position += deltaMove;
	m_target += deltaMove;
	m_isChanged = true;
	CalculateWorldMatrix();
	CalculateViewMatrix();
}

void Camera::MoveLeft(float deltaTime)
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = m_up.Cross(zaxis).Normalize();
	Vector3 deltaMove = -xaxis * deltaTime * m_moveSpeed;
	m_position += deltaMove;
	m_target += deltaMove;
	m_isChanged = true;
	CalculateWorldMatrix();
	CalculateViewMatrix();
}

Matrix Camera::RotationMatrixAroundX(float angle)
{
	Matrix m;
	m.SetRotationX(angle);
	return m;
}

Matrix Camera::RotationMatrixAroundY(float angle)
{
	Matrix m;
	Vector4 rotateAxis = Vector4(0, 1, 0, 0) * m_viewMatrix;
	m.SetRotationAngleAxis(angle, rotateAxis.x, rotateAxis.y, rotateAxis.z);
	return m;
}

void Camera::RotateLeft(float deltaTime)
{
	Vector4 localTarget = Vector4(0, 0, -(m_position - m_target).Length(), 1);
	Vector4 localNewTarget = localTarget * RotationMatrixAroundY(deltaTime * m_rotateSpeed);
	Vector4 worldNewTarget = localNewTarget * m_worldMatrix;
	Vector4 worldOldTarget = localTarget * m_worldMatrix;
	m_target = Vector3(worldNewTarget.x, worldNewTarget.y, worldNewTarget.z);
	m_isChanged = true;
	CalculateWorldMatrix();
	CalculateViewMatrix();
}

void Camera::RotateUp(float deltaTime)
{
	Vector4 localTarget = Vector4(0, 0, -(m_position - m_target).Length(), 1);
	float angle = deltaTime * m_rotateSpeed;
	if ((angle > 0 && currentPitchAngle + angle > MAX_PITCH_ANGLE) || (angle < 0 && currentPitchAngle + angle < -MAX_PITCH_ANGLE))
	{
		return;
	}
	currentPitchAngle += angle;
	Vector4 localNewTarget = localTarget * RotationMatrixAroundX(angle);
	Vector4 worldNewTarget = localNewTarget * m_worldMatrix;
	m_target = Vector3(worldNewTarget.x, worldNewTarget.y, worldNewTarget.z);
	m_isChanged = true;
	CalculateWorldMatrix();
	CalculateViewMatrix();
}