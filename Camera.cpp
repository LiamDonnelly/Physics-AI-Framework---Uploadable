#include "Camera.h"

Camera::Camera(XMFLOAT3 eye, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_position = eye;

	_eye = eye;
	_at = at; 
	_up = up; 

	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	_nearDepth = nearDepth;
	_farDepth = farDepth;
}

Camera::~Camera()
{
}

void Camera::Input()
{

}

void Camera::CalculateViewProjection()
{
	// Initiliase the view matrix
	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_at), XMLoadFloat3(&_up)));

	// Initialize the projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / _windowHeight, _nearDepth, _farDepth));
}

void Camera::SetEye(XMFLOAT3 eye)
{
	_position = eye;
	_eye = eye;
}

void Camera::SetAt(XMFLOAT3 at)
{
	_at = at;
}

void Camera::SetRotation(float x, float y, float z)
{
	XMStoreFloat4x4(&_rotate, XMMatrixRotationX(x) * XMMatrixRotationY(y) * XMMatrixRotationZ(z));
}

void Camera::Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = nearDepth;
	_farDepth = farDepth;
}

XMFLOAT4X4 Camera::GetViewProjection() const
{
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);

	XMFLOAT4X4 viewProj;

	XMStoreFloat4x4(&viewProj, view * projection);

	return viewProj;
}