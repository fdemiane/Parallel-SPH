#pragma once
#include "d3dApp.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "StaticMesh.h"
#include "MeshData.h"
#include "Camera.h"
#include "Samplers.h"
#include "ShaderResource.h"
#include "Quad.h"
#include "RenderTarget.h"
#include "GBuffersRenderer.h"
#include "SPHSim.h"

class TemplateScene : public D3DApp
{
public:
	TemplateScene(HINSTANCE hInstance);
	~TemplateScene();

	bool Init();
	void DrawScene();
	void UpdateScene();

private:
	Shader shader,shader2;
	ConstantBuffer cb;
	StaticMesh mesh;
	vector<StaticMesh> particles;
	MeshData meshData;
	Camera camera;
	ShaderResource texture;
	Quad quad;
	RenderTarget rTV;
	GBuffersRenderer gBuffers;
	SPHSimReader simReader;
	int frame;
	
	

private:

	AnisotropySampler mAnisotripySampler;
	LinearSampler mLinearSampler;
	PointSampler mPointSampler;
	
	

private:
	void Load();
	void onResize();

	
};
