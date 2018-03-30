#include "RenderTarget.h"

RenderTarget::~RenderTarget()
{
	mDepthMapDSV->Release();
	mDepthMapSRV->Release();
	mColorMapRTV->Release();
	mColorMapSRV->Release();

}

bool RenderTarget::Init(ID3D11Device *md3dDevice, UINT width, UINT height, bool HDR)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = (float)width;
	mViewport.Height = (float)height;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

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

	ID3D11Texture2D *colorMap = 0;

	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (HDR)
		texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;


	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;


	if (FAILED(md3dDevice->CreateTexture2D(&texDesc, 0, &colorMap)))
	{
		MessageBox(0, "Could no create colormap", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateRenderTargetView(colorMap, 0, &mColorMapRTV)))
	{
		MessageBox(0, "Could no create RenderTargetView", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateShaderResourceView(colorMap, 0, &mColorMapSRV)))
	{
		MessageBox(0, "Could no create ShaderResourceView", 0, 0);
		return false;
	}


	colorMap->Release();

	return true;


}

void RenderTarget::Bind(ID3D11DeviceContext *md3dImmediateContext)
{
	ID3D11RenderTargetView * renderTargets[] = { mColorMapRTV };
	md3dImmediateContext->RSSetViewports(1, &mViewport);
	md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);

}
void RenderTarget::BindWithoutDepthTest(ID3D11DeviceContext *md3dImmediateContext)
{
	ID3D11RenderTargetView * renderTargets[] = { mColorMapRTV };
	md3dImmediateContext->RSSetViewports(1, &mViewport);
	md3dImmediateContext->OMSetRenderTargets(1, renderTargets, 0);

}

void RenderTarget::Clear(ID3D11DeviceContext *md3dImmediateContext)
{
	md3dImmediateContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	md3dImmediateContext->ClearRenderTargetView(mColorMapRTV, DirectX::Colors::Black);
}

ID3D11ShaderResourceView* RenderTarget::getColorMapSRV()
{
	return mColorMapSRV;
}

ID3D11ShaderResourceView* RenderTarget::getDepthMapSRV()
{
	return mDepthMapSRV;
}