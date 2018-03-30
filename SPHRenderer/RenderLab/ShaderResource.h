#pragma once
#include "Utils.h"

class ShaderResource
{

	ID3D11ShaderResourceView *mTextureRV;

public:
	ShaderResource();
	~ShaderResource();
	bool CreateDDSFromFile(ID3D11Device *md3dDevice, WCHAR *path);
	void SetPS(ID3D11DeviceContext *md3dImmediateContext, UINT Slot);
	void SetVS(ID3D11DeviceContext *md3dImmediateContext, UINT Slot);
	void GetTexture2D(ID3D11Texture2D * texture);

public:
	ID3D11ShaderResourceView* GetShaderResourceView();
	
	
};
