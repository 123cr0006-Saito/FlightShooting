#include "../../Header/Object/Target.h"
#include "../../Header/Manager/SuperManager.h"
#include "math.h"
Target::Target(float time , float speed , int handle ,std::string type, std::vector<Vector3D>* point) : ObjectBase("target") {
	_time = time;
	_point = point;
	_speed = speed;
	_model = handle;
	_sphere = new Sphere("target",_pos, 100.0f,this);
	SuperManager::GetInstance()->GetManager("collisionManager")->AddInput(_sphere);

	if (type == "Catmull"){
		_moveType = SplineMoveType::Catmull;
		_nextPos = Spline::CatmullRomSpline((*_point), _time);
	}
	else{
		_moveType = SplineMoveType::BSpline;
		_nextPos = Spline::BSpline((*_point), (*_point).size(), _time);
	}
};

Target::~Target(){
	SuperManager::GetInstance()->GetManager("collisionManager")->Delete(_sphere);
};

bool Target::Update(){
	_pos = _nextPos;

	_time += _speed;
	if(_time > 1.0f){
		_time = 0.0f;
	}
	switch(_moveType){
		case SplineMoveType::Catmull :
			_nextPos = Spline::CatmullRomSpline((*_point), _time);
		break;
		case SplineMoveType::BSpline :
			_nextPos = Spline::BSpline((*_point), (*_point).size(),_time);
		break;
	}
	MV1SetPosition(_model, _pos.toVECTOR());
	_sphere->Update(_pos);

	// ƒ‚ƒfƒ‹‚ÌŒü‚«‚ğ•ÏX
	Vector3D temp = _nextPos - _pos;
	temp = temp.Normalize();
	float yaw = atan2(temp.x,-temp.z);
	float pitch = atan2(temp.y,sqrt(temp.x * temp.x + temp.z * temp.z));
	MV1SetRotationXYZ(_model, VGet(pitch, yaw, 0));

	return true;
};

bool Target::Draw(){
	float radius = 100;
	MV1DrawModel(_model);
	DrawLine3D(_pos.toVECTOR(), (_pos + (_nextPos - _pos) * 5).toVECTOR(), GetColor(255, 0, 0));
	return true;
};


