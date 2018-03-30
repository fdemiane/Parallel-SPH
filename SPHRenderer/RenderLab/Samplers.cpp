#include "Samplers.h"

AnisotropySampler::~AnisotropySampler()
{
	mSampler->Release();
}

bool AnisotropySampler::Create(ID3D11Device *md3dDevice, UINT MaxAnisotropy)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = MaxAnisotropy;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(md3dDevice->CreateSamplerState(&sampDesc, &mSampler)))
	{
		MessageBox(0, "Cannot create sampler state", 0, 0);
		return false;
	}

	return true;
}

void AnisotropySampler::Set(ID3D11DeviceContext *md3dImmediateContext, UINT Slot)
{
	md3dImmediateContext->PSSetSamplers(Slot, 1, &mSampler);
}

LinearSampler::~LinearSampler()
{
	mSampler->Release();
}

bool LinearSampler::Create(ID3D11Device *md3dDevice)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(md3dDevice->CreateSamplerState(&sampDesc, &mSampler)))
	{
		MessageBox(0, "Cannot create sampler state", 0, 0);
		return false;
	}

	return true;
}

void LinearSampler::Set(ID3D11DeviceContext *md3dImmediateContext, UINT Slot)
{
	md3dImmediateContext->PSSetSamplers(Slot, 1, &mSampler);
}

PointSampler::~PointSampler()
{
	mSampler->Release();
}

bool PointSampler::Create(ID3D11Device *md3dDevice)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(md3dDevice->CreateSamplerState(&sampDesc, &mSampler)))
	{
		MessageBox(0, "Cannot create sampler state", 0, 0);
		return false;
	}

	return true;
}

void PointSampler::Set(ID3D11DeviceContext *md3dImmediateContext, UINT Slot)
{
	md3dImmediateContext->PSSetSamplers(Slot, 1, &mSampler);
}