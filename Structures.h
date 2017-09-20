#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>

using namespace DirectX;
using namespace std;

struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	float specularPower;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	}
};

struct SurfaceInfo
{
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 SpecularMtrl;
};

struct Light
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 SpecularLight;

	float SpecularPower;
	XMFLOAT3 LightVecW;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;

	SurfaceInfo surface;

	Light light;

	XMFLOAT3 EyePosW;
	float HasTexture;
};

//struct ConstantBuffer
//{
//	XMMATRIX mWorld;
//	XMMATRIX mView;
//	XMMATRIX mProjection;
//
//	XMFLOAT4 diffuseMaterial;
//	XMFLOAT4 diffuseLight;
//	XMFLOAT4 ambientMtrl;
//	XMFLOAT4 ambientColour;
//	XMFLOAT4 gSpecularMtrl;
//	XMFLOAT4 gSpecularLight;
//
//	float    gSpecularPower;
//	XMFLOAT3 gEyePosW;
//	XMFLOAT3 lightVecW;
//};

//struct CubeTransformSet
//{
//	CubeTransformSet()
//	{
//		position = XMFLOAT3(0.0f, 0.0f, 0.0f);
//		scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
//		rotationSpeed = 1;
//	}
//
//	CubeTransformSet(float posX, float posY, float posZ,
//		float scaleX, float scaleY, float scaleZ,
//		float rotSpeed)
//	{
//		position = XMFLOAT3(posX, posY, posZ);
//		scale = XMFLOAT3(scaleX, scaleY, scaleZ);
//		rotationSpeed = rotSpeed;
//	}
//
//	XMFLOAT3 position;
//	XMFLOAT3 scale;
//	float rotationSpeed;
//};
