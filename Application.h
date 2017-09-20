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
#include "MaterialManager.h";
#include "TextureManager.h";
#include "D3D.h";

using namespace DirectX;

class Application
{
private:
	bool temp = true;
	
	D3D* _cDirectx;
	Lighting* _cLightingManager;
	MaterialManager* _cMaterialManager;
	TextureManager* _cTextureManager;

	HINSTANCE               _hInst;
	HWND                    _hWnd;
	

	ID3D11Buffer*           _pPlaneVertexBuffer;
	ID3D11Buffer*           _pPlaneIndexBuffer;



	ParticleManager* particleManager;

	ID3D11BlendState* _pTransparent;

	// plane Texture for wheels
	ID3D11ShaderResourceView* _pplaneTex = nullptr;

	//aircraft textures
	ID3D11ShaderResourceView* _pAIPlaneTex = nullptr;
	ID3D11ShaderResourceView* _pPlaneTex = nullptr;



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

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;


	// Game Objects

	vector<GameObject*> Objects;
	Plane* myPlane;
	AIPlane* comPlane;

	// Other Objects
	GameObject* raceTrack;
	GameObject* groundPlane;
	GameObject* mountain;
	GameObject* skyBox;
	GameObject* house;
	GameObject* firePit;
	GameObject* tree;
	GameObject* ball;
	//Effects
	// Cameras
	Camera* cameraMain;
	Camera* cameraPlaneMain;
	Camera* cameraPlaneAlternate;
	Camera* cameraTopView;
	Camera* cameraPlanePerspective;
	Camera* normPerspective;

	XMFLOAT3 cameraPlanePos;

	Camera* cameraCurrent;

	int camNum;

	float eyeX;
	float eyeY;
	float eyeZ;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	void Cleanup();
	

	// From Semester 1
	void InitObjects();
	void InitPlaneObjects();
	void PlaneUpdate(float t);
	void CameraInput();
	void ManageCollisions();


public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update(float t);
	void Draw();
};

