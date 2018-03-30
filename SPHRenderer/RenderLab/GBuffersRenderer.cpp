#include "GBuffersRenderer.h"


GBuffersRenderer::~GBuffersRenderer()
{
	if(mDepthMapSRV)
		mDepthMapSRV->Release();
	if(mDepthMapDSV)
		mDepthMapDSV->Release();
	if(mColorMapSRV1)
		mColorMapSRV1->Release();
	if(mColorMapRTV1)
		mColorMapRTV1->Release();
	if (mColorMapSRV2)
		mColorMapSRV2->Release();
	if (mColorMapRTV2)
		mColorMapRTV2->Release();
	if (mColorMapSRV3)
		mColorMapSRV3->Release();
	if (mColorMapRTV3)
		mColorMapRTV3->Release();
	if (mColorMapSRV4)
		mColorMapSRV4->Release();
	if (mColorMapRTV4)
		mColorMapRTV4->Release();
}

bool GBuffersRenderer::Init(ID3D11Device* md3dDevice, UINT width, UINT height)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = (float)width;
	mViewport.Height = (float)height;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	// Descriptor for every texture
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D *depthMap = nullptr;
	if (FAILED(md3dDevice->CreateTexture2D(&texDesc, 0, &depthMap)))
	{
		MessageBox(0, 0, "Could not Create Null Depth Map", 0);
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;
	

	
	if (FAILED(md3dDevice->CreateDepthStencilView(depthMap, &dsvDesc, &mDepthMapDSV)))
	{
		MessageBox(0, 0, "Could not Create Null Depth Map", 0);
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	

	if (FAILED(md3dDevice->CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV)))
	{
		MessageBox(0, 0, "Could not Create Null Depth Map", 0);
		return false;
	}

	depthMap->Release();

	ID3D11Texture2D *colorMap1 = 0;
	ID3D11Texture2D *colorMap2 = 0;
	ID3D11Texture2D *colorMap3 = 0;
	ID3D11Texture2D *colorMap4 = 0;
	

	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;


	if (FAILED(md3dDevice->CreateTexture2D(&texDesc, 0, &colorMap1)))
	{
		MessageBox(0, "Could no create colormap for GBuffers", 0, 0);
		return false;
	}
	if (FAILED(md3dDevice->CreateTexture2D(&texDesc, 0, &colorMap2)))
	{
		MessageBox(0, "Could no create colormap for GBuffers", 0, 0);
		return false;
	}
	if (FAILED(md3dDevice->CreateTexture2D(&texDesc, 0, &colorMap3)))
	{
		MessageBox(0, "Could no create colormap for GBuffers", 0, 0);
		return false;
	}
	if (FAILED(md3dDevice->CreateTexture2D(&texDesc, 0, &colorMap4)))
	{
		MessageBox(0, "Could no create colormap for GBuffers", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateRenderTargetView(colorMap1, 0, &mColorMapRTV1)))
	{
		MessageBox(0, "Could no create RenderTargetView GBufferA", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateShaderResourceView(colorMap1, 0, &mColorMapSRV1)))
	{
		MessageBox(0, "Could no create ShaderResourceView GBufferA", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateRenderTargetView(colorMap2, 0, &mColorMapRTV2)))
	{
		MessageBox(0, "Could no create RenderTargetView GBufferB", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateShaderResourceView(colorMap2, 0, &mColorMapSRV2)))
	{
		MessageBox(0, "Could no create ShaderResourceView GBufferB", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateRenderTargetView(colorMap3, 0, &mColorMapRTV3)))
	{
		MessageBox(0, "Could no create RenderTargetView GBufferC", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateShaderResourceView(colorMap3, 0, &mColorMapSRV3)))
	{
		MessageBox(0, "Could no create ShaderResourceView GBufferC", 0, 0);
		return false;
	}
	if (FAILED(md3dDevice->CreateRenderTargetView(colorMap4, 0, &mColorMapRTV4)))
	{
		MessageBox(0, "Could no create RenderTargetView GBufferD", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateShaderResourceView(colorMap4, 0, &mColorMapSRV4)))
	{
		MessageBox(0, "Could no create ShaderResourceView GBufferD", 0, 0);
		return false;
	}

	colorMap1->Release();
	colorMap2->Release();
	colorMap3->Release();
	colorMap4->Release();


}

void GBuffersRenderer::Bind(ID3D11DeviceContext* md3dImmediateContext)
{
	ID3D11RenderTargetView* renderTargets[] = { mColorMapRTV1 , mColorMapRTV2 , mColorMapRTV3 , mColorMapRTV4 };
	
	md3dImmediateContext->RSSetViewports(1, &mViewport);
	md3dImmediateContext->OMSetRenderTargets(4, renderTargets, mDepthMapDSV);
}

void GBuffersRenderer::Clear(ID3D11DeviceContext *md3dImmediateContext)
{
	md3dImmediateContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 0.0f, 0);
	
	md3dImmediateContext->ClearRenderTargetView(mColorMapRTV1, DirectX::Colors::Black);
	md3dImmediateContext->ClearRenderTargetView(mColorMapRTV2, DirectX::Colors::Black);
	md3dImmediateContext->ClearRenderTargetView(mColorMapRTV3, DirectX::Colors::Black);
	md3dImmediateContext->ClearRenderTargetView(mColorMapRTV4, DirectX::Colors::Black);
}

ID3D11ShaderResourceView* GBuffersRenderer::GetGBufferA()
{
	return mColorMapSRV1;
}
ID3D11ShaderResourceView* GBuffersRenderer::GetGBufferB()
{
	return mColorMapSRV2;
}
ID3D11ShaderResourceView* GBuffersRenderer::GetGBufferC()
{
	return mColorMapSRV3;
}
ID3D11ShaderResourceView* GBuffersRenderer::GetGBufferD()
{
	return mColorMapSRV4;
}
ID3D11ShaderResourceView* GBuffersRenderer::GetDepthBuffer()
{
	return mDepthMapSRV;
}