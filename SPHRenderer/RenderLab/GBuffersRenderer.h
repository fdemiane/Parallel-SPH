#pragma once
#include "Utils.h"

class GBuffersRenderer
{
public :
	D3D11_VIEWPORT mViewport;
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	

	ID3D11ShaderResourceView* mColorMapSRV1;
	ID3D11RenderTargetView* mColorMapRTV1;
	ID3D11ShaderResourceView* mColorMapSRV2;
	ID3D11RenderTargetView* mColorMapRTV2;
	ID3D11ShaderResourceView* mColorMapSRV3;
	ID3D11RenderTargetView* mColorMapRTV3;
	ID3D11ShaderResourceView* mColorMapSRV4;
	ID3D11RenderTargetView* mColorMapRTV4;

public:
	~GBuffersRenderer();

	bool Init(ID3D11Device* md3dDevice , UINT width , UINT height);
	void Bind(ID3D11DeviceContext* md3dImmediateContext);
	void Clear(ID3D11DeviceContext* md3dImmediateContext);

public :
	ID3D11ShaderResourceView* GetGBufferA();
	ID3D11ShaderResourceView* GetGBufferB();
	ID3D11ShaderResourceView* GetGBufferC();
	ID3D11ShaderResourceView* GetGBufferD();
	ID3D11ShaderResourceView* GetDepthBuffer();
};