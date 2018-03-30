#pragma once

#include "Utils.h"

class ConstantBuffer
{
	ID3D11Buffer *mConstantBuffer;
public:
	~ConstantBuffer();
	bool Create(ID3D11Device *md3dDevice, UINT size);
	void Update(ID3D11DeviceContext *md3dImmediateContext, const void *srcData);
	void SetVS(ID3D11DeviceContext *md3dImmediateContext, UINT slot);
	void SetPS(ID3D11DeviceContext *md3dImmediateContext, UINT slot);

};
