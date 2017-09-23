#include "Application.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool Application::HandleKeyboard(MSG msg)
{
	XMFLOAT3 cameraPosition = _camera->GetPosition();

	switch (msg.wParam)
	{
	case VK_UP:
		_cameraOrbitRadius = max(_cameraOrbitRadiusMin, _cameraOrbitRadius - (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_DOWN:
		_cameraOrbitRadius = min(_cameraOrbitRadiusMax, _cameraOrbitRadius + (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_RIGHT:
		_cameraOrbitAngleXZ -= _cameraSpeed;
		return true;
		break;

	case VK_LEFT:
		_cameraOrbitAngleXZ += _cameraSpeed;
		return true;
		break;
	}

	return false;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;

	DSLessEqual = nullptr;
	RSCullNone = nullptr;
	_pPlaneVertexBuffer = nullptr;
	_pPlaneIndexBuffer = nullptr;
	CCWcullMode = nullptr;
}

Application::~Application()
{
	Cleanup();
}

// ----------------- Initialisation ------------------ //

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
		return E_FAIL;
	}

	RECT rc;
	GetClientRect(_hWnd, &rc);
	_WindowWidth = rc.right - rc.left;
	_WindowHeight = rc.bottom - rc.top;

	_cDirectx = new D3D(_hWnd);
	_cDirectx->InitDevice();
	_cLightingManager = new Lighting();
	_cMaterialManager = new MaterialManager();
	_cTextureManager = new TextureManager(_cDirectx->GetDevice());
	_cObjectManager = new ObjectManager();

	// Setup Camera
	XMFLOAT3 eye = XMFLOAT3(0.0f, 2.0f, -1.0f);
	XMFLOAT3 at = XMFLOAT3(0.0f, 2.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	_camera = new Camera(eye, at, up, (float)_renderWidth, (float)_renderHeight, 0.01f, 100.0f);

	InitObjects();

	return S_OK;
}

void Application::InitObjects()
{

	// Appearance, Transform, ParticleModel
	Appearance* appearance;
	Transform* transform;
	ParticleModel* particleModel;

	ID3D11Device* _pd3dDevice = _cDirectx->GetDevice();
	

	_cObjectManager->InitObjects(_pd3dDevice ,_cMaterialManager, _cTextureManager);

	// Init Plane collection Objects
	InitPlaneObjects();

	// Camera
	XMFLOAT3 planePos = myPlane->GetPlanePosition();

	myPlane->CalculateForwardVector();
	XMFLOAT3 planeDirection = myPlane->GetForwardVector();

	camNum = 2;

	eyeX = -10.0f;
	eyeY = 4.0f;
	eyeZ = -8.5f;

	// Main Camera
	XMFLOAT3 Eye = XMFLOAT3(eyeX, eyeY, eyeZ);
	XMFLOAT3 At = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	cameraMain = new Camera(Eye, At, Up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 10000.0f);
	cameraMain->CalculateViewProjection();

	// Top View Camera
	Eye = XMFLOAT3(planePos.x + 0.01f, planePos.y + 15.0f, planePos.z + 0.01f);
	At = XMFLOAT3(planePos.x, planePos.y, planePos.z);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	cameraTopView = new Camera(Eye, At, Up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 10000.0f);
	cameraTopView->CalculateViewProjection();

	// Plane Main Camera
	Eye = XMFLOAT3(-cameraPlanePos.x, -cameraPlanePos.y + 10.0f, -cameraPlanePos.z);
	At = XMFLOAT3(planeDirection.x, planeDirection.y, planeDirection.z);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	cameraPlaneMain = new Camera(Eye, At, Up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 10000.0f);
	cameraPlaneMain->CalculateViewProjection();

	// Plane Alternative Camera
	Eye = XMFLOAT3(cameraPlanePos.x, cameraPlanePos.y + 10.0f, cameraPlanePos.z);
	At = XMFLOAT3(planeDirection.x, planeDirection.y, planeDirection.z);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	cameraPlaneAlternate = new Camera(Eye, At, Up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 10000.0f);
	cameraPlaneAlternate->CalculateViewProjection();

	// Plane Perspective Camera calculation
	Eye = XMFLOAT3(planePos.x + 10.0f, planePos.y + 10.0f, planePos.z + 10.0f);
	At = XMFLOAT3(planePos.x, planePos.y, planePos.z);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	cameraPlanePerspective = new Camera(Eye, At, Up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 5000.0f);
	cameraPlanePerspective->CalculateViewProjection();

	particleManager = new ParticleManager();
	particleManager->Init(_pd3dDevice);
}

void Application::InitPlaneObjects()
{
	Appearance* appearance;
	Transform* transform;
	ParticleModel* particleModel;

	// Plane Objects

	// Plane Body
	Geometry planeGeometry = OBJLoader::Load("Objects/Plane Objects/Plane.obj", _cDirectx->GetDevice());

	XMFLOAT3 planePos = XMFLOAT3(0.0f, 5.0f, -40.0f);
	//XMFLOAT3 planePos = XMFLOAT3(-415.415f, 5.0f, 35.63f);

	appearance = new Appearance(planeGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pPlaneTex);

	transform = new Transform();
	transform->SetScale(1.5f, 1.5f, 1.5f);
	transform->SetPosition(planePos);
	particleModel = new PlaneParticleModel(transform, 1.0f);
	particleModel->SetCollisionRadius(10.0f);

	GameObject* planeBody = new GameObject("Plane", appearance, transform, particleModel);
	// Plane Tyre Front Right
	Geometry planeTyreFrontRGeometry = OBJLoader::Load("Objects/Plane Objects/planeTyreFrontR.obj", _cDirectx->GetDevice());

	appearance = new Appearance(planeTyreFrontRGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pplaneTex);

	transform = new Transform();
	transform->SetScale(0.01f, 0.01f, 0.01f);
	transform->SetPosition(2.0f, -2.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 1.0f);

	GameObject* planeTyreFrontR = new GameObject("plane Tyre", appearance, transform, particleModel);

	// plane Tyre Front Left
	Geometry planeTyreFrontLGeometry = OBJLoader::Load("Objects/Plane Objects/planeTyreFrontL.obj", _cDirectx->GetDevice());

	appearance = new Appearance(planeTyreFrontLGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pplaneTex);

	transform = new Transform();
	transform->SetScale(0.01f, 0.01f, 0.01f);
	transform->SetPosition(-3.0f, -2.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 1.0f);

	GameObject* planeTyreFrontL = new GameObject("plane Tyre", appearance, transform, particleModel);

	// Add Wheels to Plane Scene Graph
	vector < GameObject* > planeWheels;
	planeWheels.push_back(planeTyreFrontR);
	planeWheels.push_back(planeTyreFrontL);

	planeBody->AddChild(planeTyreFrontR);
	planeBody->AddChild(planeTyreFrontL);

	myPlane = new Plane(planeBody, planeWheels);
	Objects.push_back(planeBody);

	/////////////////	AI	/////////////// 
	planePos = XMFLOAT3(0.0f, 5.0f, 0.0f);
	PlaneParticleModel* pParticleModel;
	appearance = new Appearance(planeGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pPlaneTex);

	transform = new Transform();
	transform->SetScale(1.5f, 1.5f, 1.5f);
	transform->SetPosition(planePos);
	pParticleModel = new PlaneParticleModel(transform, 1.0f);
	pParticleModel->SetCollisionRadius(10.0f);

	GameObject* aplaneBody = new GameObject("Plane", appearance, transform, pParticleModel);
	// Plane Tyre Front Right

	appearance = new Appearance(planeTyreFrontRGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pplaneTex);

	transform = new Transform();
	transform->SetScale(0.01f, 0.01f, 0.01f);
	transform->SetPosition(2.0f, -2.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 1.0f);

	planeTyreFrontR = new GameObject("plane Tyre", appearance, transform, particleModel);

	// plane Tyre Front Left

	appearance = new Appearance(planeTyreFrontLGeometry, _cMaterialManager->shinyMaterial);
	appearance->SetTextureRV(_cTextureManager->_pplaneTex);

	transform = new Transform();
	transform->SetScale(0.01f, 0.01f, 0.01f);
	transform->SetPosition(-3.0f, -2.0f, 0.0f);

	particleModel = new PlaneParticleModel(transform, 1.0f);

	planeTyreFrontL = new GameObject("plane Tyre", appearance, transform, particleModel);

	// Add Wheels to Plane Scene Graph
	vector < GameObject* > planeWheel;
	planeWheel.push_back(planeTyreFrontR);
	planeWheel.push_back(planeTyreFrontL);

	aplaneBody->AddChild(planeTyreFrontR);
	aplaneBody->AddChild(planeTyreFrontL);

	comPlane = new AIPlane(aplaneBody, planeWheel);
	Objects.push_back(aplaneBody);

}



HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	_hInst = hInstance;
	RECT rc = { 0, 0, 960, 540 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"TutorialWindowClass", L"FGGC Semester 2 Framework", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_hWnd)
		return E_FAIL;

	ShowWindow(_hWnd, nCmdShow);

	return S_OK;
}

// -------------- Cleanup -------------- //

void Application::Cleanup()
{

	if (_pPlaneVertexBuffer) _pPlaneVertexBuffer->Release();
	if (_pPlaneIndexBuffer) _pPlaneIndexBuffer->Release();

	if (DSLessEqual) DSLessEqual->Release();
	if (RSCullNone) RSCullNone->Release();

	if (CCWcullMode) CCWcullMode->Release();

	if (_camera)
	{
		delete _camera;
		_camera = nullptr;
	}

}

void Application::CameraInput()
{
	// If Camera 1 then Check Free Camera input
	if (camNum == 1)
	{
		float cameraMoveSpeed = 0.1f;

		// Zoom Out
		if (GetAsyncKeyState(VK_DOWN))
		{
			eyeX -= cameraMoveSpeed;
		}

		// Zoom In
		if (GetAsyncKeyState(VK_UP))
		{
			eyeX += cameraMoveSpeed; // or call camera setX function appropriately
		}

		// Pan Up
		if (GetAsyncKeyState(0x54))
		{
			eyeY += cameraMoveSpeed;
		}

		// Pan Down
		if (GetAsyncKeyState(0x47))
		{
			eyeY -= cameraMoveSpeed;
		}

		// Pan Left
		if (GetAsyncKeyState(0x46))
		{
			eyeZ += cameraMoveSpeed;
		}
		// Pan right
		if (GetAsyncKeyState(0x48))
		{
			eyeZ -= cameraMoveSpeed;
		}
	}

	// If Camera 1, 2 or 3 then Check Plane Input
	if (camNum == 1 || camNum == 2 || camNum == 3)
	{
		myPlane->Input();
	}

	// Camera Input Check
	if (GetAsyncKeyState('1'))
	{
		camNum = 1;
	}
	else if (GetAsyncKeyState('2'))
	{
		camNum = 2;
	}
	else if (GetAsyncKeyState('3'))
	{
		camNum = 3;
	}
	else if (GetAsyncKeyState('4'))
	{
		camNum = 4;
	}
	else if (GetAsyncKeyState('5'))
	{
		camNum = 5;
	}

	XMFLOAT3 Eye;
	XMFLOAT3 At;

	XMFLOAT3 planePos = myPlane->GetPlanePosition();
	XMFLOAT3 planeDirection = myPlane->GetForwardVector();

	// Check Camera Number
	if (camNum == 1)
	{
		Eye = XMFLOAT3(eyeX, eyeY, eyeZ);
		At = XMFLOAT3(eyeX + 1.0f, eyeY, eyeZ);

		cameraMain->SetEye(Eye);
		cameraMain->SetAt(At);
		cameraMain->CalculateViewProjection();

		cameraCurrent = cameraMain;
	}
	if (camNum == 1 || camNum == 2)
	{
		cameraCurrent = cameraPlaneMain;

		cameraPlanePos.x = planePos.x;
		cameraPlanePos.y = planePos.y;
		cameraPlanePos.z = planePos.z;

		Eye = XMFLOAT3(cameraPlanePos.x - (planeDirection.x * 15.0f), cameraPlanePos.y + 10.0f, cameraPlanePos.z - (planeDirection.z * 15.0f));
		At = XMFLOAT3(planePos.x + (planeDirection.x * 10.0f), planePos.y + (planeDirection.y * 10.0f), planePos.z + (planeDirection.z * 10.0f));

		cameraPlaneMain->SetEye(Eye);
		cameraPlaneMain->SetAt(At);
		cameraPlaneMain->CalculateViewProjection();
	}
	else if (camNum == 3)
	{
		cameraCurrent = cameraPlaneAlternate;

		cameraPlanePos.x = planePos.x - planeDirection.x;
		cameraPlanePos.y = planePos.y - planeDirection.y;
		cameraPlanePos.z = planePos.z - planeDirection.z;

		Eye = XMFLOAT3(cameraPlanePos.x + (planeDirection.x * 15.0f), cameraPlanePos.y + 10.0f, cameraPlanePos.z + (planeDirection.z * 15.0f));
		At = XMFLOAT3(cameraPlanePos.x - (planeDirection.x * 15.0f), cameraPlanePos.y - 4.0f, cameraPlanePos.z - (planeDirection.z * 15.0f));

		cameraPlaneAlternate->SetEye(Eye);
		cameraPlaneAlternate->SetAt(At);
		cameraPlaneAlternate->CalculateViewProjection();
	}
	else if (camNum == 4)
	{
		cameraCurrent = cameraTopView;

		Eye = XMFLOAT3(planePos.x + 0.01f, planePos.y + 15.0f, planePos.z + 0.01f);
		At = XMFLOAT3(planePos.x, planePos.y, planePos.z);

		cameraTopView->SetEye(Eye);
		cameraTopView->SetAt(At);
		cameraTopView->CalculateViewProjection();
	}
	else if (camNum == 5)
	{
		cameraCurrent = cameraPlanePerspective;

		Eye = XMFLOAT3(planePos.x + 10.0f, planePos.y + 10.0f, planePos.z + 10.0f);
		At = XMFLOAT3(planePos.x, planePos.y, planePos.z);

		cameraPlanePerspective->SetEye(Eye);
		cameraPlanePerspective->SetAt(At);
		cameraPlanePerspective->CalculateViewProjection();
	}
}

void Application::ManageCollisions()
{
	float kinEnergy = 0.0f;
	for (int i = 0; i < Objects.size(); i++)
	{
		for (int j =i+1; j < Objects.size(); j++)
		{
			if (Objects.at(i)->GetParticleModel()->CollisionCheck(Objects.at(j)->GetTransform()->GetPosition(), Objects.at(j)->GetParticleModel()->GetCollisionRadius()))
			{
				kinEnergy = Objects.at(i)->GetParticleModel()->ResolveCollision(Objects.at(j)->GetParticleModel());
				if (kinEnergy > 5000 || GetAsyncKeyState('H'))
					myPlane->setExploaded(true);
			}
		}
	}
	
}

// -------------- Updates ------------- //

void Application::PlaneUpdate(float t)
{
	CameraInput();

	/*if (myPlane->GetPlaneBody()->GetParticleModel()->BaseCollisionCheck(groundPlane->GetTransform()->GetPosition()))
	{
		myPlane->setFlying(false);
	}
		

	if (comPlane->GetPlaneBody()->GetParticleModel()->BaseCollisionCheck(groundPlane->GetTransform()->GetPosition()))
	{comPlane->setFlying(false);
	}*/

	// Plane Body Updates
	myPlane->Update(t);
	comPlane->Update(t);

}

void Application::Update(float t)
{
	// Update our time
	static float timeSinceStart = 0.0f;
	static DWORD dwTimeStart = 0;

	DWORD dwTimeCur = GetTickCount();

	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;

	timeSinceStart = (dwTimeCur - dwTimeStart) / 1000.0f;

	if (myPlane->isExploaded())
	{
		
		ParticleSystem* explosionPar = particleManager->GetParticalSystems().at(1);
		float count = explosionPar->GetAge();
		explosionPar->SetStartPos(myPlane->GetPlaneBody()->GetTransform()->GetPosition());
		explosionPar->ResetAll();
		while (count > 0)
		{
			explosionPar->Update(t);
			
			count -= t;
			Draw();
		}
		myPlane->GetPlaneBody()->GetTransform()->SetPosition(0.0f, 5.0f, 0.0f);
		PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)myPlane->GetPlaneBody()->GetParticleModel();
		planeBodyModel->SetEngineSpeed(0.0f);
		planeBodyModel->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		myPlane->SetPlaneYawRotation(0.0f);
		myPlane->setExploaded(false);
		
	}
	else
	{
		ManageCollisions();

		PlaneUpdate(t);

		_cObjectManager->skyBox->Update(t);
		
		_cObjectManager->Update(t);

		particleManager->SetCameraPos(cameraCurrent->GetPosition());
		particleManager->Update(t);
	}

}

// --------------- Draw --------------- //

void Application::Draw()
{
	ID3D11DeviceContext* _pImmediateContext = _cDirectx->GetDeviceContext();
	ID3D11Buffer* _pConstantBuffer = _cDirectx->GetCB();
	_cDirectx->ClearRenderTarget();


	
	// Setup View and Projection
	XMMATRIX view;
	XMMATRIX projection;

	if (camNum == 1)
	{
		XMFLOAT4X4 cameraView = cameraMain->GetView();
		view = XMLoadFloat4x4(&cameraView);

		XMFLOAT4X4 cameraProjection = cameraMain->GetProjection();
		projection = XMLoadFloat4x4(&cameraProjection);
	}
	else
	{
		XMFLOAT4X4 cameraView = cameraCurrent->GetView();
		view = XMLoadFloat4x4(&cameraView);

		XMFLOAT4X4 cameraProjection = cameraCurrent->GetProjection();
		projection = XMLoadFloat4x4(&cameraProjection);
	}

	// Render Objects
	ConstantBuffer cb;

	cb.View = XMMatrixTranspose(view);
	cb.Projection = XMMatrixTranspose(projection);

	cb.light = _cLightingManager->basicLight;
	cb.EyePosW = _camera->GetPosition();

	Material material;

	_cObjectManager->Draw(_pImmediateContext, _pConstantBuffer, cb);
	

	// ------------- Draw Plane Body ------------- //
	if (!myPlane->isExploaded())
	{
		GameObject* planeBody = myPlane->GetPlaneBody();
		material = planeBody->GetAppearance()->GetMaterial();

		// Copy material to shader
		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		// Set world matrix
		cb.World = XMMatrixTranspose(planeBody->GetTransform()->GetWorldMatrix());

		// Set plane texture
		if (planeBody->GetAppearance()->HasTexture())
		{
			ID3D11ShaderResourceView* textureRV = planeBody->GetAppearance()->GetTextureRV();
			_pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
			cb.HasTexture = 1.0f;
		}
		else
		{
			cb.HasTexture = 0.0f;
		}

		// Update constant buffer
		_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		// Draw object
		planeBody->Draw(_pImmediateContext);

		planeBody = comPlane->GetPlaneBody();
		cb.World = XMMatrixTranspose(planeBody->GetTransform()->GetWorldMatrix());

		// Set plane texture
		if (planeBody->GetAppearance()->HasTexture())
		{
			ID3D11ShaderResourceView* textureRV = planeBody->GetAppearance()->GetTextureRV();
			_pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
			cb.HasTexture = 1.0f;
		}
		else
		{
			cb.HasTexture = 0.0f;
		}

		// Update constant buffer
		_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		// Draw object
		planeBody->Draw(_pImmediateContext);


		// ------------ Draw Plane Wheels ------------ //

		vector < GameObject* > planeWheels = planeBody->GetChildren();

		for each (GameObject* wheel in planeWheels)
		{
			material = wheel->GetAppearance()->GetMaterial();

			// Copy material to shader
			cb.surface.AmbientMtrl = material.ambient;
			cb.surface.DiffuseMtrl = material.diffuse;
			cb.surface.SpecularMtrl = material.specular;

			// Set world matrix
			cb.World = XMMatrixTranspose(wheel->GetTransform()->GetWorldMatrix());

			// Set Wheel texture
			if (wheel->GetAppearance()->HasTexture())
			{
				ID3D11ShaderResourceView* textureRV = wheel->GetAppearance()->GetTextureRV();
				_pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
				cb.HasTexture = 1.0f;
			}
			else
			{
				cb.HasTexture = 0.0f;
			}

			// Update constant buffer
			_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

			// Draw object
			wheel->Draw(_pImmediateContext);
		}
		material = particleManager->GetAppearance()->GetMaterial();
		//_pImmediateContext->OMSetBlendState(_pTransparent, blendFactor, 0xffffffff);
		// Copy material to shader
		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		ParticleSystem* firePar = particleManager->GetParticalSystems().at(0);

		for each (Particle* par in firePar->GetParticalSystems())
		{
			cb.World = XMMatrixTranspose(par->GetTransform()->GetWorldMatrix());

			// Set texture
			if (particleManager->GetAppearance()->HasTexture())
			{
				ID3D11ShaderResourceView* textureRV = particleManager->GetAppearance()->GetTextureRV();
				_pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
				cb.HasTexture = 1.0f;
			}
			else
			{
				cb.HasTexture = 0.0f;
			}

			// Update constant buffer
			_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
			par->Draw(_pImmediateContext);
		}
	}

	else
	{
		//_pImmediateContext->OMSetBlendState(_pTransparent, blendFactor, 0xffffffff);
		ParticleSystem* explosionPar = particleManager->GetParticalSystems().at(1);
		for each (Particle* par in explosionPar->GetParticalSystems())
		{
			cb.World = XMMatrixTranspose(par->GetTransform()->GetWorldMatrix());

			// Set texture
			if (particleManager->GetAppearance()->HasTexture())
			{
				ID3D11ShaderResourceView* textureRV = particleManager->GetAppearance()->GetTextureRV();
				_pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
				cb.HasTexture = 1.0f;
			}
			else
			{
				cb.HasTexture = 0.0f;
			}

			// Update constant buffer
			_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
			par->Draw(_pImmediateContext);
		}
	}

	// Present our back buffer to our front buffer
	//_pSwapChain->Present(0, 0);
	_cDirectx->SwapBuffer();
	}