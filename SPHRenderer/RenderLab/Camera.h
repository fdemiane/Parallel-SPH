#pragma once
#include"Utils.h"

using namespace DirectX;

class Camera
{
	float xAngle;
	float yAngle;
	
	XMMATRIX ViewMatrix;

public:

	float translationX, translationY, translationZ;
	

	Camera();
	~Camera();
	void setControls(float deltaTime);
	void Update();

	XMFLOAT3 getPositon();
	XMMATRIX getViewMatrix();


};
