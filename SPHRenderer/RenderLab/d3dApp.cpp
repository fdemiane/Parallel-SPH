#include "d3dApp.h"
#define MSAALEVEL 1

namespace
{
	D3DApp* gd3dApp = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return gd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance)
{
	ghMainWnd = 0;
	mInstance = hInstance;
	mAppPaused = mMaximized = mMinimized = mResizing = false;
	gd3dApp = this;
	deltaMX = deltaMY = 0;

}

D3DApp::~D3DApp()
{
	md3dDevice->Release();
	md3dImmediateContext->Release();
	mSwapChain->Release();
	mRenderTargetView->Release();
	mDepthStencilBuffer->Release();
	mDepthStencilView->Release();



}

LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND TextBox;
	switch (msg)
	{
	
		 
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer.Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.Start();

		}

		return 0;

	case WM_SIZE:
		mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);

		if (md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMaximized = false;
				mMinimized = true;

			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMaximized = true;
				mMinimized = false;
				onResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					onResize();
				}
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					onResize();
				}
				else if (mResizing)
				{

				}
				else
				{
					onResize();
				}
			}
		}

		return 0;

	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		mTimer.Stop();
		return 0;

	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		mTimer.Start();
		onResize();
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		if (wParam == 'P')
		{
			mMaximized = false;
			mClientWidth = 800;
			mClientHeight = 600;
			onResize();
		}
		if (wParam == 'R')
		{
			mWireFrame = !mWireFrame;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}



bool D3DApp::Init()
{
	if (!initWindow())
		return false;
	if (!initd3dDevice())
		return false;


	return true;
}

int D3DApp::Run()
{
	MSG msg = { 0 };
	float AdditiveDeltaTime = 0;
	mTimer.Reset();

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (!mAppPaused)
		{
			mTimer.Tick();
			if (AdditiveDeltaTime >= (1.0f / 30.0f))
			{
				POINT cursorPos;
				cursorPos.x = cursorPos.y = 400;
				ClientToScreen(ghMainWnd, &cursorPos);
				//if (!GetAsyncKeyState('K'))
				//SetCursorPos(cursorPos.x, cursorPos.y);
				deltaMX = 400 - mouseX;
				deltaMY = 400 - mouseY;



				AdditiveDeltaTime = 0;
				CalcFrameStats();
				DrawScene();
				UpdateScene();




			}
			else
				AdditiveDeltaTime += mTimer.DeltaTime();

		}
		else
			Sleep(100);
	}



	return (int)msg.wParam;



}

bool D3DApp::initWindow()
{

	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "BasicWndClass";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, "REGISTER CLASS FAILED", 0, 0);
		return false;
	}

	ghMainWnd = CreateWindow(
		"BasicWndClass",
		"Win32Basic",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		mInstance,
		0);

	int g = GetLastError();
	std::cout << "  " << g << " ";
	if (ghMainWnd == 0)
	{
		MessageBox(0, "Window Creation Failed", 0, 0);
		return false;
	}


	ShowWindow(ghMainWnd, SW_SHOW);
	UpdateWindow(ghMainWnd);


	return true;
}

bool D3DApp::initd3dDevice()
{
	UINT createDeviceFlags = 0;
	RECT rc;
	GetClientRect(ghMainWnd, &rc);
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3
	};

	D3D_FEATURE_LEVEL  g_featureLevel = D3D_FEATURE_LEVEL_11_0;

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&md3dDevice,
		&g_featureLevel,
		&md3dImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, "D3D11CreateDeviceFailed", 0, 0);
		return false;
	}




	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = MSAALEVEL;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = ghMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	IDXGIDevice *dxgiDevice = 0;
	md3dDevice->QueryInterface(_uuidof(IDXGIDevice), (void**)&dxgiDevice);
	IDXGIAdapter *dxgiAdapter = 0;
	dxgiDevice->GetParent(_uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	IDXGIFactory *dxgiFactory = 0;
	dxgiAdapter->GetParent(_uuidof(IDXGIFactory), (void**)&dxgiFactory);


	dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain);

	dxgiAdapter->Release();
	dxgiDevice->Release();
	dxgiFactory->Release();



	ID3D11Texture2D *backBuffer;
	mSwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	hr = md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(0, "Cannot Create BACKBUFFER", 0, 0);
		return false;
	}


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = MSAALEVEL;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, "Cannot Create depthStencilDesc", 0, 0);
		return false;

	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = depthStencilDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;


	hr = md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);

	if (FAILED(hr))
	{
		MessageBox(0, "Cannot Create DepthStencilView", 0, 0);
		return false;
	}

	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	backBuffer->Release();
	onResize();

	D3D11_RASTERIZER_DESC NoCUllRaserState;
	NoCUllRaserState.CullMode = D3D11_CULL_NONE;
	NoCUllRaserState.FillMode = D3D11_FILL_SOLID;
	NoCUllRaserState.FrontCounterClockwise = false;
	NoCUllRaserState.DepthBias = 0;
	NoCUllRaserState.DepthBiasClamp = 0.0f;
	NoCUllRaserState.SlopeScaledDepthBias = 0.0f;
	NoCUllRaserState.DepthClipEnable = true;
	NoCUllRaserState.MultisampleEnable = true;
	NoCUllRaserState.ScissorEnable = false;
	NoCUllRaserState.AntialiasedLineEnable = false;

	if (FAILED(md3dDevice->CreateRasterizerState(&NoCUllRaserState, &mNoCullRasterState)))
	{
		MessageBox(0, "Cannot Create RasterState", 0, 0);
		return false;
	}

	D3D11_RASTERIZER_DESC WireFrameRaserState;
	WireFrameRaserState.CullMode = D3D11_CULL_NONE;
	WireFrameRaserState.FillMode = D3D11_FILL_WIREFRAME;
	WireFrameRaserState.FrontCounterClockwise = false;
	WireFrameRaserState.DepthBias = 0;
	WireFrameRaserState.DepthBiasClamp = 0.0f;
	WireFrameRaserState.SlopeScaledDepthBias = 0.0f;
	WireFrameRaserState.DepthClipEnable = true;
	WireFrameRaserState.MultisampleEnable = true;
	WireFrameRaserState.ScissorEnable = false;
	WireFrameRaserState.AntialiasedLineEnable = false;

	if (FAILED(md3dDevice->CreateRasterizerState(&WireFrameRaserState, &mWireFrameRasterState)))
	{
		MessageBox(0, "Cannot Create RasterState", 0, 0);
		return false;
	}


	D3D11_RASTERIZER_DESC DefaultRaserState;
	DefaultRaserState.CullMode = D3D11_CULL_BACK;
	DefaultRaserState.FillMode = D3D11_FILL_SOLID;
	DefaultRaserState.FrontCounterClockwise = false;
	DefaultRaserState.DepthBias = 0;
	DefaultRaserState.DepthBiasClamp = 0.0f;
	DefaultRaserState.SlopeScaledDepthBias = 0.0f;
	DefaultRaserState.DepthClipEnable = false;
	DefaultRaserState.MultisampleEnable = true;
	DefaultRaserState.ScissorEnable = false;
	DefaultRaserState.AntialiasedLineEnable = false;

	if (FAILED(md3dDevice->CreateRasterizerState(&DefaultRaserState, &mDefaultRasterState)))
	{
		MessageBox(0, "Cannot Create RasterState", 0, 0);
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;

	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	
	if (FAILED(md3dDevice->CreateDepthStencilState(&dsDesc, &pDSState)))
	{
		MessageBox(0, "Cannot Create Depth Stencil state" ,0 ,0 );
		return 0;
	}

	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	if (FAILED(md3dDevice->CreateDepthStencilState(&dsDesc, &pDSState2)))
	{
		MessageBox(0, "Cannot Create Depth Stencil state", 0, 0);
		return 0;
	}

	md3dImmediateContext->OMSetDepthStencilState(pDSState, 1);

	md3dImmediateContext->RSSetState(mDefaultRasterState);


	ID3D11BlendState* g_pBlendStateNoBlend = NULL;

	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.RenderTarget[0].BlendEnable = FALSE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BlendState.AlphaToCoverageEnable = true;

	md3dDevice->CreateBlendState(&BlendState, &g_pBlendStateNoBlend);
	float blendFactor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	UINT sampleMask = 0xffffffff;
	md3dImmediateContext->OMSetBlendState(g_pBlendStateNoBlend, 0, sampleMask);


	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
void D3DApp::SetReversedDepthState()
{
	md3dImmediateContext->OMSetDepthStencilState(pDSState, 1);
}
void D3DApp::SetNormalDepthState()
{
	md3dImmediateContext->OMSetDepthStencilState(pDSState2, 1);
}
void D3DApp::SetDefaultRenderTarget()
{
	md3dImmediateContext->RSSetViewports(1, &vp);
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void D3DApp::EnableCulling()
{
	mCull = true;
	if (mWireFrame)
		md3dImmediateContext->RSSetState(mWireFrameRasterState);
	else
		md3dImmediateContext->RSSetState(mDefaultRasterState);
}

void D3DApp::DisableCulling()
{
	mCull = false;
	if (mWireFrame)
		md3dImmediateContext->RSSetState(mWireFrameRasterState);
	else
		md3dImmediateContext->RSSetState(mNoCullRasterState);
}


void D3DApp::SetNoDepthTestRenderTarget()
{
	md3dImmediateContext->RSSetViewports(1, &vp);
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);
}

void D3DApp::onResize()
{
	HRESULT hr;
	assert(md3dImmediateContext);
	assert(md3dDevice);
	assert(mSwapChain);

	mRenderTargetView->Release();
	mDepthStencilView->Release();
	mDepthStencilBuffer->Release();

	/*DXGI_MODE_DESC BufferDesc;
	BufferDesc.Width = mClientWidth;
	BufferDesc.Height = mClientHeight;
	BufferDesc.RefreshRate.Numerator = 60;
	BufferDesc.RefreshRate.Denominator = 1;
	BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;*/


	mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	//if (mMaximized)
	//	mSwapChain->SetFullscreenState(true, NULL);
	//else
		//mSwapChain->SetFullscreenState(false, NULL);

	//mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);


	ID3D11Texture2D *backBuffer;
	mSwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	hr = md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(0, "Cannot Create BACKBUFFER", 0, 0);

	}


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = MSAALEVEL;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, "Cannot Create depthStencilDesc", 0, 0);

	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = depthStencilDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;


	hr = md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);

	if (FAILED(hr))
	{
		MessageBox(0, "Cannot Create DepthStencilView", 0, 0);
	}

	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	backBuffer->Release();



	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<float>(mClientWidth);
	vp.Height = static_cast<float>(mClientHeight);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	md3dImmediateContext->RSSetViewports(1, &vp);
}

void D3DApp::SetViewportSize(int width, int height)
{
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
}

void D3DApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, DirectX::Colors::Black);
	mSwapChain->Present(0, 0);

}

void D3DApp::UpdateScene()
{

}

void D3DApp::CalcFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt * 4;
		float mspf = (float)1000.0f / fps;
		FrameRate = fps;
		DeltaTime = mspf;

		std::wostringstream outs;
		outs.precision(6);

		outs << " FPS  :  " << fps << "  FrameTime  :  " << mspf << " ms";

		SetWindowTextW(ghMainWnd, outs.str().c_str());
		

		frameCnt = 0;
		timeElapsed += 0.25f;

	}
}


