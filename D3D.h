#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Structures.h"

using namespace DirectX;
class D3D
{
public:
	D3D(HWND hwnd);
	~D3D();

	
	HRESULT InitDevice();
	HRESULT InitShaders();

	ID3D11DeviceContext* GetDeviceContext() { return _pImmediateContext; };
	ID3D11Device* GetDevice() { return _pd3dDevice; };
	ID3D11Buffer* GetCB() { return _pConstantBuffer; };

	void ClearRenderTarget();
	void UpdateConstantBuffer(ConstantBuffer cb);
	void SwapBuffer();

	ID3D11Debug* pDebug;
	ID3DUserDefinedAnnotation* pUserDefinedAnnotation;
private:
	HWND _hwnd;

	float clientWidth;
	float clientHeight;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;
	ID3D11SamplerState*		_pSamplerLinear;
	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState*	RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11Texture2D*		_depthStencilBuffer = nullptr;
	ID3D11BlendState*		_pTransparent;
	ID3D11Buffer*           _pConstantBuffer;

	
	

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};

