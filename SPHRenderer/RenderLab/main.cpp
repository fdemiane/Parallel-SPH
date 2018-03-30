#include <iostream>
#include <Windows.h>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include "d3dApp.h"
#include "TemplateScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	TemplateScene app(hInstance);
	app.Init();
	app.Run();
	return 0;

}