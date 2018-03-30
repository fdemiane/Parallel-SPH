#pragma once
#include "Utils.h"

class RenderTarget
{
public:
	D3D11_VIEWPORT mViewport;
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	ID3D11ShaderResourceView* mColorMapSRV;
	ID3D11RenderTargetView* mColorMapRTV;

public:
	~RenderTarget();
	bool Init(ID3D11Device *md3dDevice, UINT width, UINT height, bool HDR);
	void Bind(ID3D11DeviceContext *md3dImmediateContext);
	void BindWithoutDepthTest(ID3D11DeviceContext *md3dImmediateContext);
	void Clear(ID3D11DeviceContext *md3dImmediateContext);

public:
	ID3D11ShaderResourceView* getColorMapSRV();
	ID3D11ShaderResourceView* getDepthMapSRV();


	
};
