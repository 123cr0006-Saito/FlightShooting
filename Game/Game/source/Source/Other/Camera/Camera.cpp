#include "../../../Header/Other/Camera/Camera.h"
#include "../.../../AppFrame/source/System/Header/Function/Timer.h"
#include "../../../Header/Manager/SuperManager.h"

#include "../AppFrame/source/Application/UtilMacro.h"
#include "../AppFrame/source/System/Header/Function/mymath.h"
Camera* Camera::_instance = nullptr;

Camera::Camera() :
	_isGame(false),
	_currentTime(GetNowCount())
{
	if (_instance != nullptr) {
		DebugErrar();
		return;
	}
	_instance = this;

	
};

Camera::Camera(Vector3D pos, Vector3D target) :
_isGame(false),
_currentTime(GetNowCount())
{
	if (_instance != nullptr) {
		DebugErrar();
		return;
	}
	_instance = this;
	_pos.first = pos;
	_pos.second = target;
};

Camera::~Camera(){
	_instance = nullptr;
};

bool Camera::Update(Vector3D pos, Vector3D target, Vector3D up){

	double elapsedTime = 1.0 / 60.0;
	SpringDamperSystem(elapsedTime, _pos.first, pos, _posSpeed, 100, 20);
	SpringDamperSystem(elapsedTime,_pos.second, target, _targetSpeed,50,20);
	
	
	SetCameraPositionAndTargetAndUpVec(_pos.first.toVECTOR(), _pos.second.toVECTOR(), up.toVECTOR());
	return true;
};

void Camera::UpdateSelectAndResult(){
	int moveTime = 1000;
	float nowTimeRate = Math::Clamp(0.0f,1.0f,(float)(GetNowCount() - _currentTime) / moveTime);

	Vector3D pos = Lerp(_holdPos.first, _pos.first, nowTimeRate);
	Vector3D target = Lerp(_holdPos.second, _pos.second, nowTimeRate);

	SetCameraPositionAndTarget_UpVecY(pos.toVECTOR(), target.toVECTOR());
};

void Camera::UpdateGame(){


	//SpringDamperSystem(targetPos);

	//SetCameraPositionAndTarget_UpVecY(_pos.first.toVECTOR(), (_pos.second + (targetPos - _pos.second) / 1.3f).toVECTOR());
};

void Camera::SetIsGame(bool isGame) {
	_isGame = isGame; 
	_holdPos.first = _pos.first;
	_holdPos.second = _pos.second;
	_currentTime = GetNowCount();
};

bool Camera::SpringDamperSystem(double time ,Vector3D& nowPos, Vector3D targetPos, Vector3D& speed, float spling, float danpa){
	float springConstant = spling;
	float dampingConstant = danpa;
	
	Vector3D pos = nowPos - targetPos;
	Vector3D springForce = pos * -springConstant; // バネの力 -kx 弾性力
	Vector3D dampingForce = speed * -dampingConstant; // ダンパの力 -cv 抵抗力
	Vector3D force = springForce + dampingForce; // 合力

	speed += force * time;
	nowPos += speed * time;

	return true;
};

bool Camera::Draw(){
	printfDx("x:%f y:%f z:%f\n",_pos.first.x, _pos.first.y, _pos.first.z);
	printfDx("x:%f y:%f z:%f\n", _pos.second.x, _pos.second.y, _pos.second.z);
	return true;
};
