#include "ObjectManager.h"



ObjectManager::ObjectManager()
{

}


ObjectManager::~ObjectManager()
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

	objects.push_back(skyBox);


	//Ball Init
	Geometry ballGeometry = OBJLoader::Load("Objects/sphere.obj", _pd3dDevice);

	appearance = new Appearance(ballGeometry, _cMaterialManager->shinyMaterial);
	//appearance->SetTextureRV(_cTextureManager->_pSkyTex);

	transform = new Transform();
	transform->SetPosition(-50.0f, 6.0f, 0.0f);
	transform->SetScale(3.0f, 3.0f, 3.0f);
	transform->SetRotation(XMConvertToRadians(0.0f), 0.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 1.0f);
	particleModel->SetCollisionRadius(1.0f);
	ball = new GameObject("Ball", appearance, transform, particleModel);
	objects.push_back(ball);

	Geometry houseG = OBJLoader::Load("Objects/House/house.obj", _pd3dDevice);

	appearance = new Appearance(houseG, _cMaterialManager->noSpecMaterial);
	//appearance->SetTextureRV(_cTextureManager->_pHouseTex);

	transform = new Transform();
	transform->SetPosition(-300.0f, 0.0f, 50.0f);
	transform->SetScale(0.2f, 0.2f, 0.2f);
	transform->SetRotation(XMConvertToRadians(0.0f), XMConvertToRadians(-30.0f), 0.0f);

	particleModel = new PlaneParticleModel(transform, 10.0f);
	particleModel->SetCollisionRadius(50.0f);
	house = new GameObject("House", appearance, transform, particleModel);
	objects.push_back(house);

	Geometry mountainGeometry = OBJLoader::Load("Objects/mountain.obj", _pd3dDevice);

	appearance = new Appearance(mountainGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pMountainTex);

	transform = new Transform();
	transform->SetPosition(0.0f, 0.0f, 0.0f);
	transform->SetScale(0.2f, 0.2f, 0.2f);
	transform->SetRotation(XMConvertToRadians(0.0f), 0.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 100.0f);

	mountain = new GameObject("Mountain", appearance, transform, particleModel);
	objects.push_back(mountain);

	//tree Init
	Geometry treeGeo = OBJLoader::Load("Objects/Tree/lowTree.obj", _pd3dDevice);

	appearance = new Appearance(treeGeo, _cMaterialManager->noSpecMaterial);
	appearance->SetTextureRV(_cTextureManager->_pTreeTex);

	transform = new Transform();
	transform->SetPosition(-160.0f, 0.0f, 20.0f);
	transform->SetScale(0.3f, 0.3f, 0.3f);
	transform->SetRotation(XMConvertToRadians(0.0f), 0.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 2.0f);
	particleModel->SetCollisionRadius(1.0f);
	tree = new GameObject("tree", appearance, transform, particleModel);
	objects.push_back(tree);

	Geometry raceTrackGeometry = OBJLoader::Load("Objects/Runway.obj", _pd3dDevice);

	appearance = new Appearance(raceTrackGeometry, _cMaterialManager->noSpecMaterial);
	appearance->SetTextureRV(_cTextureManager->_pRaceTrackTex);

	transform = new Transform();
	transform->SetPosition(0.0f, 0.5f, 0.0f);
	transform->SetScale(1.0f, 1.0f, 1.0f);
	transform->SetRotation(XMConvertToRadians(0.0f), 0.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 1.0f);

	raceTrack = new GameObject("Race Track", appearance, transform, particleModel);
	objects.push_back(raceTrack);

	Geometry firepitGeo = OBJLoader::Load("Objects/Tree/firePit.obj", _pd3dDevice);

	appearance = new Appearance(firepitGeo, _cMaterialManager->noSpecMaterial);
	appearance->SetTextureRV(_cTextureManager->_pTreeTex);

	transform = new Transform();
	transform->SetPosition(-260.0f, 0.0f, 10.0f);
	transform->SetScale(0.3f, 0.3f, 0.3f);
	transform->SetRotation(XMConvertToRadians(0.0f), 0.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 1.0f);
	particleModel->SetCollisionRadius(1.0f);
	firePit = new GameObject("Firepit", appearance, transform, particleModel);
	objects.push_back(firePit);

	Geometry groundGeometry = OBJLoader::Load("Objects/ground.obj", _pd3dDevice);

	appearance = new Appearance(groundGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pGrassTex);

	transform = new Transform();
	transform->SetPosition(0.0f, -1.0f, 0.0f);
	transform->SetScale(0.5f, 0.5f, 0.5f);
	transform->SetRotation(XMConvertToRadians(0.0f), 0.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, -1.0f);

	groundPlane = new GameObject("Ground Plane", appearance, transform, particleModel);
	objects.push_back(groundPlane);
}

void ObjectManager::Update(float t)
{
	for (GameObject* var : objects)
	{
		var->Update(t);
	}
}

void ObjectManager::Draw(ID3D11DeviceContext * _pI, ID3D11Buffer* _pConstantBuffer, ConstantBuffer cb)
{

	// --------------- Draw Sky Box ---------------- //
	for (GameObject* var : objects)
	{
		Material material = var->GetAppearance()->GetMaterial();

		// Copy material to shader
		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		// Set world matrix
		cb.World = XMMatrixTranspose(var->GetTransform()->GetWorldMatrix());

		// Set texture
		if (var->GetAppearance()->HasTexture())
		{
			ID3D11ShaderResourceView* textureRV = var->GetAppearance()->GetTextureRV();
			_pI->PSSetShaderResources(0, 1, &textureRV);
			cb.HasTexture = 1.0f;
		}
		else
		{
			cb.HasTexture = 0.0f;
		}

		// Update constant buffer
		_pI->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		// Draw object
		var->Draw(_pI);
	}
	
}
