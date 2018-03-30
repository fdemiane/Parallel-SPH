#pragma once

#include"Utils.h"

class Shader
{
	ID3D11VertexShader *mVertexShader;
	ID3D11PixelShader *mPixelShader;
	ID3D11InputLayout *mInputLayout;

public:
	Shader();
	~Shader();

	bool Create(ID3D11Device *md3dDevice, WCHAR *FileName);
	void SetInputLayout(ID3D11DeviceContext *md3dImmediateContext);
	void SetShader(ID3D11DeviceContext *md3dImmediateContext);

};