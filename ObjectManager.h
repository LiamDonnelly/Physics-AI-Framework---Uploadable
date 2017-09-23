#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>

#include "Structures.h"
#include "PlaneParticleModel.h"
#include "GameObject.h"
#include "OBJLoader.h"
#include "ParticleManager.h"

#include "Lighting.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "D3D.h"

using namespace DirectX;
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Update(float t);
	void Draw(ID3D11DeviceContext* _pI, ID3D11Buffer* _pConstantBuffer, ConstantBuffer cb);

	void InitObjects(ID3D11Device* _pd3dDevice, MaterialManager* _cMaterialManager, TextureManager* _cTextureManager);

	GameObject* skyBox;
	GameObject* ball;
	GameObject* house;
	GameObject* mountain;
	GameObject* tree;
	GameObject* raceTrack;
	GameObject* firePit;
	GameObject* groundPlane;

	vector<GameObject*> objects;
};

