#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

#include "Structures.h"

using namespace DirectX;
using namespace std;

class Appearance
{
public:
	Appearance(Geometry geometry, Material material);
	~Appearance();

	Geometry GetGeometryData() const { return _geometry; }

	Material GetMaterial() const { return _material; }

	void SetTextureRV(ID3D11ShaderResourceView * textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView * GetTextureRV() const { return _textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	void Draw(ID3D11DeviceContext* pImmediateContext);

private:
	ID3D11ShaderResourceView* _textureRV;

	Geometry _geometry;
	Material _material;
};