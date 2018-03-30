#include "Quad.h"

Quad::Quad()
{
	vertices[0].Position = XMFLOAT3(1, 1, 1);
	vertices[1].Position = XMFLOAT3(-1, 1, 1);
	vertices[2].Position = XMFLOAT3(-1, -1, 1);
	vertices[3].Position = XMFLOAT3(1, -1, 1);

	vertices[0].UV = XMFLOAT2(1, 0);
	vertices[1].UV = XMFLOAT2(0, 0);
	vertices[2].UV = XMFLOAT2(0, 1);
	vertices[3].UV = XMFLOAT2(1, 1);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 3;
	indices[4] = 1;
	indices[5] = 2;

}

Quad::~Quad()
{
	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = 0;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}
}

bool Quad::Init(ID3D11Device *md3dDevice)
{

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex)*4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertices;

	if (FAILED(md3dDevice->CreateBuffer(&bd, &data, &vertexBuffer)))
	{
		MessageBox(0, "Failed to Create Vertex Buffer", 0, 0);
		return false;
	}

	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&data, sizeof(data));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(UINT) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	data.pSysMem = indices;

	if (FAILED(md3dDevice->CreateBuffer(&bd, &data, &indexBuffer)))
	{
		MessageBox(0, "Failed to Create Index Buffer", 0, 0);
		return false;
	}

	return true;
}

void Quad::Draw(ID3D11DeviceContext *md3dImmediateContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT , 0);
	md3dImmediateContext->DrawIndexed(6, 0, 0);
}