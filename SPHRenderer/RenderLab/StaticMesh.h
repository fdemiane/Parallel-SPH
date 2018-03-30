#pragma once
#include "Utils.h"
#include "MeshData.h"

class StaticMesh
{
	XMFLOAT3 position;
	XMMATRIX transformationMatrix;
	ID3D11Buffer *vertexBuffer;
	MeshData* meshData;

	
	//XMFLOAT3 cube[];

public:
	StaticMesh();
	~StaticMesh();

	bool Init(ID3D11Device *md3dDevice, MeshData *meshData);
	void draw(ID3D11DeviceContext *md3dImmediateContext);
	void Scale(float x, float y, float z);
	void SetPosition(float x, float y , float z);
	XMMATRIX GetTransformationMatrix();
};
