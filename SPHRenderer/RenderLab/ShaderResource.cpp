#include "ShaderResource.h"
#include "DDSTextureLoader.h"


ShaderResource::ShaderResource()
{
	mTextureRV = nullptr;
}

ShaderResource::~ShaderResource()
{
	mTextureRV->Release();
}

bool ShaderResource::CreateDDSFromFile(ID3D11Device *md3dDevice, WCHAR *path)
{
	
	if (DirectX::CreateDDSTextureFromFile(md3dDevice, path, NULL, &mTextureRV))
	{
		MessageBox(0, "Cannot Find or Create DDS :", 0, 0);
		return false;
	}

	return true;
}

void ShaderResource::SetPS(ID3D11DeviceContext *md3dImmediateContext, UINT Slot)
{
	md3dImmediateContext->PSSetShaderResources(Slot, 1, &mTextureRV);
}

void ShaderResource::SetVS(ID3D11DeviceContext *md3dImmediateContext, UINT Slot)
{
	md3dImmediateContext->VSSetShaderResources(Slot, 1, &mTextureRV);
}

void ShaderResource::GetTexture2D(ID3D11Texture2D *texture)
{
	ID3D11Resource *resource;
	mTextureRV->GetResource(&resource);
	texture = static_cast<ID3D11Texture2D *>(resource);
	if (texture == 0)
	{
		MessageBox(0, "Could Not Get texture shader resource", 0, 0);
	}
}

ID3D11ShaderResourceView* ShaderResource::GetShaderResourceView()
{
	return mTextureRV;
}