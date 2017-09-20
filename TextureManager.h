#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Structures.h"
#include "DDSTextureLoader.h"

using namespace DirectX;
class TextureManager
{
public:
	TextureManager(ID3D11Device* _pd3dDevice);
	~TextureManager();

	// Textures
	ID3D11ShaderResourceView* _pTextureRV = nullptr;
	ID3D11ShaderResourceView* _pRaceTrackTex = nullptr;
	ID3D11ShaderResourceView* _pGrassTex = nullptr;
	ID3D11ShaderResourceView* _pFireTex = nullptr;
	ID3D11ShaderResourceView* _pMountainTex = nullptr;
	ID3D11ShaderResourceView* _pStoneTex = nullptr;
	ID3D11ShaderResourceView* _pSkyTex = nullptr;
	ID3D11ShaderResourceView* _pHouseTex = nullptr;
	ID3D11ShaderResourceView* _pTreeTex = nullptr;

	// plane Texture for wheels
	ID3D11ShaderResourceView* _pplaneTex = nullptr;

	//aircraft textures
	ID3D11ShaderResourceView* _pAIPlaneTex = nullptr;
	ID3D11ShaderResourceView* _pPlaneTex = nullptr;
};

