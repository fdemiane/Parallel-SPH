#include "ConstantBuffer.h"

ConstantBuffer::~ConstantBuffer()
{
	mConstantBuffer->Release();
}

bool ConstantBuffer::Create(ID3D11Device *md3dDevice, UINT size)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	if (FAILED(md3dDevice->CreateBuffer(&bd, nullptr, &mConstantBuffer)))
	{
		MessageBox(0, "Cannot create constant buffer ", 0, 0);
		return false;
	}

	return true;
}

void ConstantBuffer::Update(ID3D11DeviceContext *md3dImmediateContext, const void * srcData)
{
	md3dImmediateContext->UpdateSubresource(mConstantBuffer, 0, nullptr, srcData, 0, 0);
}

void ConstantBuffer::SetVS(ID3D11DeviceContext *md3dImmediateContext, UINT slot)
{
	md3dImmediateContext->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::SetPS(ID3D11DeviceContext *md3dImmediateContext, UINT slot)
{
	md3dImmediateContext->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}