#include"Camera.h"
#include"Utils.h"

Camera::Camera()
{
	translationX = translationY = translationZ = 0;
	xAngle = 0;
	yAngle = 0;
	
	
	ViewMatrix = XMMatrixIdentity();

};


Camera::~Camera()
{

}

void Camera::setControls(float deltaTime)
{
	deltaTime *= 0.003f;
	if (GetAsyncKeyState('Q') & 0x8000)

	{
		if (GetAsyncKeyState('W') & 0x8000)
		{
			translationX += -deltaTime*sinf(-yAngle);
			translationZ -= deltaTime*cosf(-yAngle);
			translationY += -deltaTime*sinf(-xAngle);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			translationX += deltaTime*sinf(-yAngle);
			translationZ -= -deltaTime*cosf(-yAngle);
			translationY += deltaTime*sinf(-xAngle);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			translationX += -deltaTime*cosf(-yAngle);
			translationZ -= -deltaTime*sinf(-yAngle);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			translationX += deltaTime*cosf(-yAngle);
			translationZ -= deltaTime*sinf(-yAngle);
		}
	}

	else
	{
		if (GetAsyncKeyState('W') & 0x8000)
		{
			
			translationX += -deltaTime*sinf(-yAngle);
			translationZ -= deltaTime*cosf(-yAngle);
			
		}


		if (GetAsyncKeyState('S') & 0x8000)
		{
			translationX += deltaTime*sinf(-yAngle);
			translationZ -= -deltaTime*cosf(-yAngle);
		}


		if (GetAsyncKeyState('D') & 0x8000)
		{
			translationX += -deltaTime*cosf(-yAngle);
			translationZ -= -deltaTime*sinf(-yAngle);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			translationX += deltaTime*cosf(-yAngle);
			translationZ -= deltaTime*sinf(-yAngle);
		}
	}


	if (GetAsyncKeyState(VK_UP) & 0x8000)
		xAngle -= deltaTime ;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		xAngle += deltaTime ;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		yAngle += deltaTime ;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		yAngle -= deltaTime ;



	

	if (GetAsyncKeyState('G') & 0x8000)
	{
		MessageBox(0, FloatToString(translationX).c_str(), 0, 0);
		MessageBox(0, FloatToString(translationY).c_str(), 0, 0);
		MessageBox(0, FloatToString(translationZ).c_str(), 0, 0);
		
	}
	
	

}


void Camera::Update()
{
	XMMATRIX trans = XMMatrixTranslation(translationX, translationY, translationZ);
	XMMATRIX rotx = XMMatrixRotationX(-xAngle);
	XMMATRIX roty = XMMatrixRotationY(yAngle);

	ViewMatrix = rotx*roty*trans;
	ViewMatrix = XMMatrixMultiply(trans, XMMatrixMultiply(roty, rotx));
}

XMFLOAT3 Camera::getPositon()
{
	return XMFLOAT3(-translationX, -translationY, -translationZ);
}

XMMATRIX Camera::getViewMatrix()
{
	
	return ViewMatrix;
}