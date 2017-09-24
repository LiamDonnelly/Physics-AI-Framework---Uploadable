#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <dinput.h>
using namespace DirectX;

class Camera
{
public:
	Camera(XMFLOAT3 eye, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	// Check Camera Input
	void Input(float t);
	void Update();

	// Calculate the View Projection
	void CalculateViewProjection();

	// Get View
	XMFLOAT4X4 GetView() const { return _view; }

	// Get Projection
	XMFLOAT4X4 GetProjection() const { return _projection; }

	// Get View/Projection
	XMFLOAT4X4 GetViewProjection() const;

	// Get/Set Eye
	XMVECTOR GetEye() const { return XMLoadFloat3(&_eye); }
	void SetEye(XMFLOAT3 eye);

	// Get/Set At
	XMVECTOR GetAt() const { return XMLoadFloat3(&_at); }
	void SetAt(XMFLOAT3 at);

	// Get/Set Up
	XMVECTOR GetUp() const { return XMLoadFloat3(&_up); }
	void SetUp(XMVECTOR up) { XMStoreFloat3(&_up, up); }

	// Get/Set Position
	XMFLOAT3 GetPosition() const { return _position; }
	void SetPosition(XMFLOAT3 position) { _position = position; };

	// Get/Set Rotation
	void SetRotation(float x, float y, float z);

	// Alter Window height and width values. As well as distance sight
	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);

private:
	// Camera Properties
	XMFLOAT3 _eye;
	XMFLOAT3 _at;
	XMFLOAT3 _up;

	XMFLOAT3 DEFAULTFORWARD;
	XMFLOAT3 DEFAULTRIGHT;
	XMFLOAT3 _forward;
	XMFLOAT3 _right;
	XMFLOAT3 _position;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	FLOAT _windowWidth;
	FLOAT _windowHeight;

	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;
	float _mouseX;
	float _mouseY;

	FLOAT _nearDepth;
	FLOAT _farDepth;

	XMFLOAT4X4 _rotate;
	float _yaw;
	float _pitch;
	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;

	
};

