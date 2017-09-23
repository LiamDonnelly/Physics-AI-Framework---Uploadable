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
