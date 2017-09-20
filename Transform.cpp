#include "Transform.h"

Transform::Transform()
{
	_parent = nullptr;
	_position = XMFLOAT3();
	_rotation = XMFLOAT3();
	_scale = XMFLOAT3();
}

Transform::~Transform()
{
}

void Transform::Update(float t)
{
	_prevPosition = _position;
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z);
	XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&_position));

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
}