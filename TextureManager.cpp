#include "TextureManager.h"



TextureManager::TextureManager(ID3D11Device* _pd3dDevice)
{
	HRESULT hr;
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\stone.dds", nullptr, &_pTextureRV);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\CFAPlaneTexture.dds", nullptr, &_pAIPlaneTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\F35PlaneTexture.dds", nullptr, &_pPlaneTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\planeTex.dds", nullptr, &_pplaneTex);

	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\RunwayTrack.dds", nullptr, &_pRaceTrackTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\grass.dds", nullptr, &_pGrassTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\fire.dds", nullptr, &_pFireTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\mountain.dds", nullptr, &_pMountainTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\floor.dds", nullptr, &_pStoneTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\skyBox.dds", nullptr, &_pSkyTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\grass.dds", nullptr, &_pHouseTex);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Resources\\stone.dds", nullptr, &_pTreeTex);

}


TextureManager::~TextureManager()
{
}

