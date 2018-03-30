#include "MeshData.h"

bool MeshData::importFromOBJ(string path)
{
	std::vector<XMFLOAT3> pos;
	std::vector<XMFLOAT3>normals;
	std::vector<XMFLOAT2>UVs;
	std::vector<unsigned int> indices;
	std::vector<XMFLOAT3>mtangents;
	std::vector<XMFLOAT3>mbitangents;

	if (!loadOBJ(path, pos, normals, UVs))
	{
		MessageBox(0, "Cannot find obj file", 0, 0);
		return false;
	}
	
	for (UINT i = 0; i < pos.size() - 2; i += 3)
	{
		XMFLOAT3 v0 = pos[i];
		XMFLOAT3 v1 = pos[i + 1];
		XMFLOAT3 v2 = pos[i + 2];

		XMFLOAT2 uv0 = UVs[i];
		uv0.y = 1 - uv0.y;
		XMFLOAT2 uv1 = UVs[i + 1];
		uv1.y = 1 - uv1.y;
		XMFLOAT2 uv2 = UVs[i + 2];
		uv2.y = 1 - uv2.y;

		XMFLOAT3 D1, D2;
		XMFLOAT2 U1, U2;

		D1.x = v1.x - v0.x;
		D1.y = v1.y - v0.y;
		D1.z = v1.z - v0.z;

		D2.x = v2.x - v0.x;
		D2.y = v2.y - v0.y;
		D2.z = v2.z - v0.z;

		U1.x = uv1.x - uv0.x;
		U1.y = uv1.y - uv0.y;

		U2.x = uv2.x - uv0.x;
		U2.y = uv2.y - uv0.y;

		float r = 1 / (U1.x*U2.y - U1.y*U2.x);

		XMFLOAT3 tangent, bitangent;

		tangent.x = (D1.x*U2.y - D2.x*U1.y)*r;
		tangent.y = (D1.y*U2.y - D2.y*U1.y)*r;
		tangent.z = (D1.z*U2.y - D2.z*U1.y)*r;

		bitangent.x = (D2.x * U1.x - D1.x*U2.x)*r;
		bitangent.y = (D2.y * U1.x - D1.y*U2.x)*r;
		bitangent.z = (D2.z * U1.x - D1.z*U2.x)*r;

		Vertex temp1;
		Vertex temp2;
		Vertex temp3;
		temp1.Position = pos[i];
		temp2.Position = pos[i + 1];
		temp3.Position = pos[i + 2];
		temp1.UV = UVs[i];
		temp2.UV = UVs[i + 1];
		temp3.UV = UVs[i + 2];

		temp1.Normal = normals[i];
		temp2.Normal = normals[i + 1];
		temp3.Normal = normals[i + 2];

		temp1.Tangent = tangent;
		temp2.Tangent = tangent;
		temp3.Tangent = tangent;
		temp1.Bitangent = bitangent;
		temp2.Bitangent = bitangent;
		temp3.Bitangent = bitangent;

		mtangents.push_back(tangent);
		mtangents.push_back(tangent);
		mtangents.push_back(tangent);
		mbitangents.push_back(bitangent);
		mbitangents.push_back(bitangent);
		mbitangents.push_back(bitangent);



		vertices.push_back(temp1);
		vertices.push_back(temp2);
		vertices.push_back(temp3);


	}

	

	return true;
	/*pos.clear();
	normals.clear();
	UVs.clear();*/

}

vector<Vertex>* MeshData::getVertices()
{
	return &vertices;
}