#include "ObjectManager.h"



ObjectManager::ObjectManager()
{

}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::Update(float t)
{

}

void ObjectManager::Draw(ID3D11DeviceContext * _pI)
{

}

void ObjectManager::InitObjects(ID3D11Device* _pd3dDevice, MaterialManager* _cMaterialManager, TextureManager* _cTextureManager)
{
	// Sky Box Initialisation
	Geometry skyBoxGeometry = OBJLoader::Load("Objects/sphere.obj", _pd3dDevice);

	Appearance* appearance = new Appearance(skyBoxGeometry, _cMaterialManager->skyBoxMat);
	appearance->SetTextureRV(_cTextureManager->_pSkyTex);

	Transform* transform = new Transform();
	transform->SetPosition(0.0f, 0.0f, 0.0f);
	transform->SetScale(-10000.0f, -10000.0f, -10000.0f);
	transform->SetRotation(XMConvertToRadians(0.0f), 0.0f, 0.0f);

	PlaneParticleModel* particleModel = new PlaneParticleModel(transform, 0.1f);

	skyBox = new GameObject("Sky Box", appearance, transform, particleModel);
}
