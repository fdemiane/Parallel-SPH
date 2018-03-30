#pragma once

#include "Utils.h"

class AnisotropySampler
{
	ID3D11SamplerState* mSampler;

public:

	~AnisotropySampler();
	bool Create(ID3D11Device *md3dDevice, UINT MaxAnisotropy);
	void Set(ID3D11DeviceContext *md3dImmediateContext, UINT Slot);

};

class LinearSampler
{
	ID3D11SamplerState* mSampler;

public:

	~LinearSampler();
	bool Create(ID3D11Device *md3dDevice);
	void Set(ID3D11DeviceContext *md3dImmediateContext, UINT Slot);

};

class PointSampler
{
	ID3D11SamplerState* mSampler;

public:

	~PointSampler();
	bool Create(ID3D11Device *md3dDevice);
	void Set(ID3D11DeviceContext *md3dImmediateContext, UINT Slot);

};
