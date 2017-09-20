#include "ParticleManager.h"


ParticleManager::ParticleManager()
{
	
}


ParticleManager::~ParticleManager()
{
}

void ParticleManager::Init(ID3D11Device* _pd3dDevice)
{
	CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\fire.dds", nullptr, &_pFireTex);
	Material noSpecMaterial;
	noSpecMaterial.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	noSpecMaterial.specularPower = 0.0f;
	
	Geometry planeGeometry = OBJLoader::Load("Objects/EffectsGeo.obj", _pd3dDevice);
	appearance = new Appearance(planeGeometry, noSpecMaterial);
	appearance->SetTextureRV(_pFireTex);
	

	fireSmoke = new ParticleSystem(appearance);
	fireSmoke->SetStartPos(XMFLOAT3(-259.0f, 2.0f, 10.0f));
	fireSmoke->SetCamPos(camPos);
	fireSmoke->SetMaxParticles(10);
	fireSmoke->SetAge(1000);
	fireSmoke->Init();
	particalSystems.push_back(fireSmoke);

	explosionSmoke = new ParticleSystem(appearance);
	explosionSmoke->SetStartPos(XMFLOAT3(0.0f,0.0f,0.0f));
	explosionSmoke->SetCamPos(camPos);
	explosionSmoke->SetMaxParticles(50);
	explosionSmoke->SetAge(10000);
	explosionSmoke->Init();
	particalSystems.push_back(explosionSmoke);

}

void ParticleManager::Release()
{

}

void ParticleManager::Update(float t)
{
	fireSmoke->SetCamPos(camPos);
	fireSmoke->Update(t);
	explosionSmoke->SetCamPos(camPos);
	explosionSmoke->Update(t);

}
