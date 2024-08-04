#include "../../Header/Object/Bullet.h"
#include "../../Header/Manager/SuperManager.h"
Bullet::Bullet(const Vector3D& pos, const Vector3D& forward) : ObjectBase("bullet") {
	_pos = pos;
	_forwardVec = forward;
	_currentTime = GetNowCount();
	_sphere = new Sphere("bullet",_pos, 30.0f,this);
	SuperManager::GetInstance()->GetManager("collisionManager")->AddInput(_sphere);
	_isAlive = true;
};

Bullet::~Bullet(){
	SuperManager::GetInstance()->GetManager("collisionManager")->Delete(_sphere);
	_sphere = nullptr;
	_isAlive = false;
};

bool Bullet::Update(){

	float speed = 100.0f;
	_pos += _forwardVec * speed;
	int nowTime = GetNowCount() - _currentTime;
	if(_sphere != nullptr){
		_sphere->Update(_pos);
	}
	if(nowTime > 1000){
		SuperManager::GetInstance()->GetManager("objectManager")->DeleteInput(this);	
		_isAlive = false;
	}
	return true;
};

bool Bullet::Draw(){
	DrawSphere3D(_pos.toVECTOR(), 30.0f, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
	return true;
};
