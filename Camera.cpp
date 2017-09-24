#include "Camera.h"

Camera::Camera(XMFLOAT3 eye, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_position = eye;

	DEFAULTFORWARD = XMFLOAT3(0.0f, 0.0f, 1.0f);
	DEFAULTRIGHT = XMFLOAT3(1.0f, 0.0f, 0.0f);
	_forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	_right = XMFLOAT3(1.0f,0.0f,0.0f);

	_yaw = 0.0f;
	_pitch = 0.0f;

	_eye = eye;
	_at = at; 
	_up = up; 
	

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	 _keyboard->SetDataFormat(&c_dfDIKeyboard);
	
	_mouseX = 0.0f;
	_mouseY = 0.0f;

	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	_nearDepth = nearDepth;
	_farDepth = farDepth;

	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / _windowHeight, _nearDepth, _farDepth));
}

Camera::~Camera()
{
}

void Camera::Input(float t)
{
	
	DIMOUSESTATE mouseCurrState;
	
	BYTE keyboardState[256];
	_keyboard->Acquire();
	_mouse->Acquire();

	_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	_keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);


	float speed = 15.0f * t;

	if (keyboardState[DIK_A] & 0x80)
	{
		moveLeftRight -= speed;
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		moveLeftRight += speed;
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		moveBackForward += speed;
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		moveBackForward -= speed;
	}
	if ((mouseCurrState.lX != _mouseX) || (mouseCurrState.lY != _mouseY))
	{
		_yaw += _mouseX * 0.001f;

		_pitch += mouseCurrState.lY * 0.001f;

		_mouseX = mouseCurrState.lX;
		_mouseY = mouseCurrState.lY;
	}


	Update();
}

void Camera::Update()
{
	XMStoreFloat4x4(&_rotate, XMMatrixRotationRollPitchYaw(_pitch, _yaw, 0.0f));

	XMStoreFloat3(&_at, XMVector3TransformCoord(XMLoadFloat3(&DEFAULTFORWARD), XMLoadFloat4x4(&_rotate)));
	XMStoreFloat3(&_at, XMVector3Normalize(XMLoadFloat3(&_at)));

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(_yaw);

	XMStoreFloat3(&_right, XMVector3TransformCoord(XMLoadFloat3(&DEFAULTRIGHT), RotateYTempMatrix));
	XMStoreFloat3(&_up, XMVector3TransformCoord(XMLoadFloat3(&_up), RotateYTempMatrix));
	XMStoreFloat3(&_forward, XMVector3TransformCoord(XMLoadFloat3(&DEFAULTFORWARD), RotateYTempMatrix));

	XMVECTOR eyePos = XMLoadFloat3(&_eye);
	eyePos += moveLeftRight*XMLoadFloat3(&_right);
	eyePos += moveBackForward*XMLoadFloat3(&_forward);

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	XMStoreFloat3(&_eye, eyePos);

	XMVECTOR target = eyePos + XMLoadFloat3(&_at);
	XMStoreFloat3(&_at, target);

	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_at), XMLoadFloat3(&_up)));
}

void Camera::CalculateViewProjection()
{
	// Initiliase the view matrix
	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_at), XMLoadFloat3(&_up)));

	// Initialize the projection matrix
	
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
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / _windowHeight, _nearDepth, _farDepth));

}

XMFLOAT4X4 Camera::GetViewProjection() const
{
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);

	XMFLOAT4X4 viewProj;

	XMStoreFloat4x4(&viewProj, view * projection);

	return viewProj;
}