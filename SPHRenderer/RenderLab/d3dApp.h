#pragma once

#include <iostream>
#include<sstream>
#include <Windows.h>
#include <windowsx.h>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include <fstream>
#include "Timer.h"


class D3DApp
{
protected:
	HWND ghMainWnd;
	HINSTANCE mInstance;

public:

	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	virtual bool Init();
	virtual int Run();
	virtual void onResize();

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	virtual bool initd3dDevice();
	virtual bool initWindow();
	virtual void DrawScene();
	virtual void UpdateScene();
	void CalcFrameStats();
	void SetDefaultRenderTarget();
	void SetReversedDepthState();
	void SetNormalDepthState();
	void SetNoDepthTestRenderTarget();
	void DisableCulling();
	void EnableCulling();
	void EnableWireFrame();
	void DisableWireFrame();


protected:

	UINT mClientWidth, mClientHeight;
	ID3D11Device *md3dDevice;
	ID3D11DeviceContext *md3dImmediateContext;
	IDXGISwapChain *mSwapChain;
	ID3D11RenderTargetView *mRenderTargetView;
	ID3D11DepthStencilView *mDepthStencilView;
	ID3D11RasterizerState *mDefaultRasterState, *mNoCullRasterState, *mWireFrameRasterState;
	ID3D11DepthStencilState * pDSState, *pDSState2;
	ID3D11Texture2D *mDepthStencilBuffer;
	D3D11_VIEWPORT vp;

	UINT mAAQuality;
	Timer mTimer;
	float FrameRate;
	float DeltaTime;
	bool mAppPaused, mMinimized, mMaximized, mResizing, mWireFrame, mCull;
	float mouseX, mouseY;
	float deltaMX, deltaMY;

protected:

	void SetViewportSize(int width , int height);




};



