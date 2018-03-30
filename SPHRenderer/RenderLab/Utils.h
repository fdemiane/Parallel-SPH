#pragma once

#define PI 3.14159265359
#define LOG(x) MessageBox(0, x, 0, 0)
#define LOGI(x) MessageBox(0, FloatToString(x).c_str(), 0, 0);
#include <iostream>
#include <Windows.h>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

struct SHcoeff
{
	DirectX::XMFLOAT4 L00;
	DirectX::XMFLOAT4 L1m1;
	DirectX::XMFLOAT4 L10;
	DirectX::XMFLOAT4 L11;
	DirectX::XMFLOAT4 L2m2;
	DirectX::XMFLOAT4 L2m1;
	DirectX::XMFLOAT4 L20;
	DirectX::XMFLOAT4 L21;
	DirectX::XMFLOAT4 L22;

};

struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT3 Bitangent;
	
	
};





HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
float StringToFloat(std::string x);
std::string FloatToString(float x);
bool IsNumber(char x);





