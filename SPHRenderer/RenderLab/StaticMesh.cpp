#include "StaticMesh.h"

StaticMesh::StaticMesh() 
{

}

StaticMesh::~StaticMesh()
{
	if(vertexBuffer)
	vertexBuffer->Release();
}

bool StaticMesh::Init(ID3D11Device *md3dDevice, MeshData *meshData)
{
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex)*meshData->getVertices()->size(); 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = &(*meshData->getVertices())[0];
	
	
	if (FAILED(md3dDevice->CreateBuffer(&bd, &data, &vertexBuffer)))
	{
		MessageBox(0, "Failed to Create Vertex Buffer", 0, 0);
		return false;
	}
	transformationMatrix = XMMatrixIdentity();
	this->meshData = meshData;

	return true;
}

void StaticMesh::draw(ID3D11DeviceContext *md3dImmediateContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	md3dImmediateContext->Draw(meshData->getVertices()->size() , 0);
}

void StaticMesh::Scale(float x, float y, float z)
{
	XMMATRIX scale = XMMatrixScaling(x, y, z);
	transformationMatrix = XMMatrixMultiply(transformationMatrix, scale);

}

void StaticMesh::SetPosition(float x, float y, float z)
{
	XMFLOAT3 translation = XMFLOAT3(x - position.x, y - position.y, z - position.z);
	position = XMFLOAT3(x, y, z);
	transformationMatrix = XMMatrixMultiply(transformationMatrix, XMMatrixTranslation(translation.x , translation.y , translation.z));
}

XMMATRIX StaticMesh::GetTransformationMatrix()
{
	return transformationMatrix;
}
