#pragma once
#include "Base/ObjectBase.h"
#include "appframe.h"
class Jet : public ObjectBase
{
public:
	Jet();
	~Jet();
	void SetAcceleration(float& param, float input, int max);
	bool Update() override;
	bool Draw() override;
protected:
	XInput* _input;
	int _model;
	int _coolTime;
	Quaternion cameraQuaternion;
	Quaternion origin;
	Vector3D _airplaneMoveSpeed;
	Vector3D _perspectiveMoveSpeed;
	bool isCameraBack;
};

