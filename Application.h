#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>

#include "D3D.h";

#include "ParticleManager.h"
#include "Lighting.h";
#include "ObjectManager.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "Camera.h"

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
	InputManager* _cInputManager;
	// Camera
	Camera* _camera;

	UINT _WindowHeight;
	UINT _WindowWidth;

	// Render dimensions - Change here to alter screen resolution
	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;


private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

	void Cleanup();
	
	void InitObjects();


public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update(float t);
	void Draw();
};

