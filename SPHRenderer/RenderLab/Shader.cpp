#pragma once

#include "Shader.h"

Shader::Shader()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mInputLayout = nullptr;
}

Shader::~Shader()
{
	mVertexShader->Release();
	mPixelShader->Release();
	mInputLayout->Release();
}

bool Shader::Create(ID3D11Device *md3dDevice, WCHAR *FileName)
{
	ID3DBlob *pVSBlob;
	pVSBlob = nullptr;
	if (FAILED(CompileShaderFromFile(FileName, "VS", "vs_4_0", &pVSBlob)))
	{
		MessageBox(0, "Could not find Vertex fx File", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mVertexShader)))
	{
		MessageBox(0, "Could not Create Vertex Shader", 0, 0);
		pVSBlob->Release();
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(md3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mInputLayout)))
	{
		MessageBox(0, "Could not Create Input Layout", 0, 0);
		return false;
	}

	pVSBlob->Release();

	ID3DBlob *pPSBlob;
	pPSBlob = nullptr;
	if (FAILED(CompileShaderFromFile(FileName, "PS", "ps_4_0", &pPSBlob)))
	{
		MessageBox(0, "Could not find Pixel fx File", 0, 0);
		return false;
	}

	if (FAILED(md3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mPixelShader)))
	{
		MessageBox(0, "Could not Create Vertex Shader", 0, 0);
		pPSBlob->Release();
		return false;
	}
	pPSBlob->Release();

	return true;
}

void Shader::SetInputLayout(ID3D11DeviceContext *md3dImmediateContext)
{
	md3dImmediateContext->IASetInputLayout(mInputLayout);
}

void Shader::SetShader(ID3D11DeviceContext *md3dImmediateContext)
{
	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->VSSetShader(mVertexShader, nullptr, 0);
	md3dImmediateContext->PSSetShader(mPixelShader, nullptr, 0);
}