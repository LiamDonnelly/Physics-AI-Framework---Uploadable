#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>

#include "Structures.h"
#include "AIPlane.h"
#include "Plane.h"
#include "Camera.h"
#include "PlaneParticleModel.h"
#include "DDSTextureLoader.h"
#include "GameObject.h"
#include "OBJLoader.h"
#include "ParticleManager.h"

#include "Lighting.h";
#include "ObjectManager.h"
#include "MaterialManager.h";
#include "TextureManager.h";
#include "D3D.h";

using namespace DirectX;

class Application
{
private:
	
	HINSTANCE _hInst;
	HWND _hWnd;
	
	D3D* _cDirectx;
	Lighting* _cLightingManager;
	MaterialManager* _cMaterialManager;
	TextureManager* _cTextureManager;
	ObjectManager* _cObjectManager;
	ParticleManager* particleManager;

	// Camera
	Camera* _camera;
	float _cameraOrbitRadius = 7.0f;
	float _cameraOrbitRadiusMin = 2.0f;
	float _cameraOrbitRadiusMax = 50.0f;
	float _cameraOrbitAngleXZ = -90.0f;
	float _cameraSpeed = 5.0f;

	UINT _WindowHeight;
	UINT _WindowWidth;

	// Render dimensions - Change here to alter screen resolution
	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;


private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

	void Cleanup();
	
	void InitObjects();
	void CameraInput();


public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update(float t);
	void Draw();
};

