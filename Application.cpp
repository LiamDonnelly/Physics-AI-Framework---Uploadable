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

	return true;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;

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

	_camera = new Camera(eye, at, up, (float)_renderWidth, (float)_renderHeight, 0.01f, 10000.0f);
	_camera->CalculateViewProjection();
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

	particleManager = new ParticleManager();
	particleManager->Init(_pd3dDevice);
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
	if (_camera)
	{
		delete _camera;
		_camera = nullptr;
	}
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

	_cObjectManager->Update(t);
	_camera->Input(t);

}

// --------------- Draw --------------- //

void Application::Draw()
{
	ID3D11DeviceContext* _pImmediateContext = _cDirectx->GetDeviceContext();
	ID3D11Buffer* _pConstantBuffer = _cDirectx->GetCB();
	_cDirectx->ClearRenderTarget();

	// Setup View and Projection
	XMMATRIX view = XMLoadFloat4x4(&_camera->GetView());
	XMMATRIX projection = XMLoadFloat4x4(&_camera->GetProjection());

	// Render Objects
	ConstantBuffer cb;
	cb.View = XMMatrixTranspose(view);
	cb.Projection = XMMatrixTranspose(projection);
	cb.light = _cLightingManager->basicLight;
	cb.EyePosW = _camera->GetPosition();

	//Draw Objects
	_cObjectManager->Draw(_pImmediateContext, _pConstantBuffer, cb);
	
	// Present our back buffer to our front buffer
	_cDirectx->SwapBuffer();

}