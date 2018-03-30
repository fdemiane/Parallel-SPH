#pragma once
#include "Utils.h"



class Quad
{
	XMFLOAT3 position;
	XMMATRIX transformationMatrix;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	Vertex vertices[4];
	UINT indices[6];


	//XMFLOAT3 cube[];

public:
	Quad();
	~Quad();

	bool Init(ID3D11Device *md3dDevice);
	void Draw(ID3D11DeviceContext *md3dImmediateContext);
	void SetPosition(XMFLOAT3 position);
	XMFLOAT3 GetPosition();
};

