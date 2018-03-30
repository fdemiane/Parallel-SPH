#include "TemplateScene.h"

struct cbInput {
	XMMATRIX projMat;
	XMMATRIX viewMat;
	XMMATRIX modelMat;
}cbInput;

TemplateScene::TemplateScene(HINSTANCE hInstance) : D3DApp(hInstance)
{
	frame = 0;
}

TemplateScene::~TemplateScene()
{

}

void TemplateScene::Load()
{
	mAnisotripySampler.Create(md3dDevice, 1);
	mLinearSampler.Create(md3dDevice);
	mPointSampler.Create(md3dDevice);

	meshData.importFromOBJ("SPHParticle.obj");
	mesh.Init(md3dDevice, &meshData);
	
	simReader.loadSimulation("output.txt");
	for (int i = 0; i < simReader.sim.frame[0].size(); i++)
		particles.push_back(StaticMesh());

	for (int i = 0; i < simReader.sim.frame[0].size(); i++)
	{


		particles[i].Init(md3dDevice, &meshData);
		particles[i].Scale(0.1f, 0.1f, 0.1f);

		
	}
	
	cb.Create(md3dDevice, sizeof(cbInput));
	shader.Create(md3dDevice, L"GBuffers.hlsl");
	shader2.Create(md3dDevice, L"Lighting.hlsl");

	texture.CreateDDSFromFile(md3dDevice, L"rocks.dds");

	rTV.Init(md3dDevice, mClientWidth, mClientHeight, 0);
	gBuffers.Init(md3dDevice, mClientWidth, mClientHeight);

	quad.Init(md3dDevice);
		

	
	
}

bool TemplateScene::Init()
{
	if (!D3DApp::Init())
		return false;
	Load();
	
}

void TemplateScene::DrawScene()
{
	mLinearSampler.Set(md3dImmediateContext, 0);
	mPointSampler.Set(md3dImmediateContext, 1);
	mAnisotripySampler.Set(md3dImmediateContext, 2);
	EnableCulling();

	camera.setControls(DeltaTime);
	camera.Update();
	SetReversedDepthState();
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(70.0f), (FLOAT)mClientWidth / (FLOAT)mClientHeight, 50.0f, 0.1f);
	cbInput.projMat = XMMatrixTranspose(projectionMatrix);
	cbInput.viewMat =XMMatrixTranspose(camera.getViewMatrix());
	cbInput.modelMat = XMMatrixIdentity();
	cb.Update(md3dImmediateContext, &cbInput);
	cb.SetVS(md3dImmediateContext, 0);
	cb.SetPS(md3dImmediateContext, 0);

	// TODO switch to deferred 
	// shader is OK
	gBuffers.Clear(md3dImmediateContext);
	gBuffers.Bind(md3dImmediateContext);
	
	shader.SetShader(md3dImmediateContext);
	texture.SetPS(md3dImmediateContext, 0);

	for (int i = 0; i < simReader.sim.frame[0].size(); i++) {
		cbInput.modelMat = XMMatrixTranspose(particles[i].GetTransformationMatrix());
		cb.Update(md3dImmediateContext, &cbInput);
		cb.SetVS(md3dImmediateContext, 0);
		cb.SetPS(md3dImmediateContext, 0);
		particles[i].draw(md3dImmediateContext);
	}
	//mesh.draw(md3dImmediateContext);
	
	SetDefaultRenderTarget();
	SetNormalDepthState();
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, Colors::MidnightBlue);
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0);


	DisableCulling();
	md3dImmediateContext->PSSetShaderResources(0, 1, &gBuffers.mColorMapSRV1);
	md3dImmediateContext->PSSetShaderResources(1, 1, &gBuffers.mColorMapSRV2);
	md3dImmediateContext->PSSetShaderResources(2, 1, &gBuffers.mDepthMapSRV);
	
	cbInput.projMat = XMMatrixInverse(nullptr,cbInput.projMat);
	cb.Update(md3dImmediateContext, &cbInput);
	shader2.SetShader(md3dImmediateContext);
	quad.Draw(md3dImmediateContext);
	
	

	mSwapChain->Present(0, 0);
	
}

void TemplateScene::UpdateScene()
{
	if (frame >= simReader.sim.frame.size()-2)
		frame = 0;
	for (int i = 0; i < simReader.sim.frame[0].size(); i++) 
		particles[i].SetPosition(simReader.sim.frame[frame][i].x, simReader.sim.frame[frame][i].y, simReader.sim.frame[frame][i].z);


	frame++;
	
	
}

void TemplateScene::onResize()
{
	D3DApp::onResize();
}