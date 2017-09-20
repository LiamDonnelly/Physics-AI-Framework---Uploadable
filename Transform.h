#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

using namespace DirectX;
using namespace std;

class Transform
{
public:
	Transform();
	~Transform();

	// Set/Get for position
	void SetPosition(XMFLOAT3 position) { _prevPosition = _position; _position = position; };
	void SetPosition(float x, float y, float z) { _position.x = x; _position.y = y; _position.z = z; }
	XMFLOAT3 GetPosition() const { return _position; }

	// Set/Get for previous position
	void SetPrevPosition(XMFLOAT3 prevPosition) { _prevPosition = prevPosition; }
	void SetPrevPosition(float _x, float y, float z) { _prevPosition.x = _x; _prevPosition.y = y; _prevPosition.z = z; }
	XMFLOAT3 GetPrevPosition() const { return _prevPosition; }

	// Set/Get for Scale
	void SetScale(XMFLOAT3 scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale.x = x; _scale.y = y; _scale.z = z; }
	XMFLOAT3 GetScale() const { return _scale; }

	// Set/Get for Rotation
	void SetRotation(XMFLOAT3 rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _rotation.x = x; _rotation.y = y; _rotation.z = z; }
	void AddRotation(float x, float y, float z) { _rotation.x += x; _rotation.y += y; _rotation.z += z; };
	XMFLOAT3 GetRotation() const { return _rotation; }

	// Get World Matrix
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	// Set Parent
	void SetParent(Transform* parent) { _parent = parent; }

	// Update
	void Update(float t);

private:
	XMFLOAT3 _rotation;
	XMFLOAT3 _position;
	XMFLOAT3 _prevPosition;
	
	XMFLOAT3 _scale;

	XMFLOAT4X4 _world;

	Transform* _parent;
};

