#include "../../Header/Object/Target.h"
#include "../../Header/Manager/SuperManager.h"
#include "math.h"
Target::Target(float time, float speed, int handle, std::string type, std::vector<Vector3D>* point) : 
	ObjectBase("target"),
	_time(time),
	_speed(speed),
	_model(handle),
	_point(point),
	_sphere(nullptr),
	_moveType(SplineMoveType::Catmull),
	_frameSize(0),
	_isAlive(true),
	_alpha(1.0f)
{
	_sphere = new Sphere("target", _pos, 100.0f, this);
	SuperManager::GetInstance()->GetManager("collisionManager")->AddInput(_sphere);

	//MV1SetScale(_model, (Vector3D(1, 1, 1) * 3).toVECTOR());

	if (type == "Catmull") {
		_moveType = SplineMoveType::Catmull;
		_nextPos = Spline::CatmullRomSpline((*_point), _time);
	}
	else {
		_moveType = SplineMoveType::BSpline;
		_nextPos = Spline::BSpline((*_point), (*_point).size(), _time);
	}

	_frameSize = MV1GetFrameNum(_model);

	for (int i = 0; i < _frameSize; i++) {
		Vector3D dirVec = MV1GetFramePosition(_model, i);
		_moveVec.push_back(dirVec.Normalize() * 10);
		Vector3D rotVec;
		rotVec.x = rand() % 100 - 50;
		rotVec.y = rand() % 100 - 50;
		rotVec.z = rand() % 100 - 50;
		rotVec = rotVec * 0.0001f;
		_rotVec.push_back(rotVec);
	}
};

Target::~Target(){
	SuperManager::GetInstance()->GetManager("collisionManager")->Delete(_sphere);
	_rotVec.clear();
	_moveVec.clear();
};

void Target::MoveProcess(){
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

	if (_sphere != nullptr) {
		_sphere->Update(_pos);
	}
	
	// ÉÇÉfÉãÇÃå¸Ç´ÇïœçX
	Vector3D temp = _nextPos - _pos;
	temp = temp.Normalize();
	float yaw = atan2(temp.x,-temp.z);
	float pitch = atan2(temp.y,sqrt(temp.x * temp.x + temp.z * temp.z));
	MV1SetRotationXYZ(_model, VGet(pitch, yaw, 0));
};

void Target::ExplosionProcess(){
	_alpha -= 0.02;
	MV1SetOpacityRate(_model,_alpha);
	for (int i = 0; i < _frameSize; i++) {
		MATRIX matrix = MGetIdent();
		matrix = MGetRotX(_rotVec[i].x);
		matrix = MMult(matrix, MGetRotY(_rotVec[i].y));
		matrix = MMult(matrix, MGetRotZ(_rotVec[i].z));
		matrix = MMult(matrix, MGetTranslate(_moveVec[i].toVECTOR()));
		_moveVec[i].y -= 0.5f;
		matrix = MMult(MV1GetFrameLocalMatrix(_model, i), matrix);
		MV1SetFrameUserLocalMatrix(_model, i, matrix);
	}

	
	if (_alpha < 0) {
		SuperManager::GetInstance()->GetManager("objectManager")->DeleteInput(this);
	}
};

bool Target::Update(){

	if (_isAlive) {
		MoveProcess();
	}
	else {
		ExplosionProcess();
	}

	return true;
};

bool Target::Draw(){
	float radius = 100;
	MV1DrawModel(_model);
	return true;
};


