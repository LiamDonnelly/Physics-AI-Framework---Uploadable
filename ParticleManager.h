#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "Appearance.h"
#include "ParticleModel.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "OBJLoader.h"
#include "DDSTextureLoader.h"
using namespace DirectX;

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void Init(ID3D11Device* _pd3dDevice);
	void Release();
	void Update(float t);

	vector<ParticleSystem*> GetParticalSystems() { return particalSystems; }
	Appearance* GetAppearance() const { return appearance; };

	vector<ParticleSystem*> particalSystems;
	ParticleSystem* fireSmoke;
	ParticleSystem* explosionSmoke;

	XMFLOAT3 GetCameraPos() { return camPos; }
	void SetCameraPos(XMFLOAT3 value) { camPos = value; }
private:

	ID3D11ShaderResourceView* _pFireTex = nullptr;

	Appearance* appearance;
	XMFLOAT3 camPos;
};

