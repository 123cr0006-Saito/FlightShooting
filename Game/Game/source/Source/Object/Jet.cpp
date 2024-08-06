#include "../../Header/Object/Jet.h"
#include "../../Header/Object/Bullet.h"
#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Other/Camera/Camera.h"
namespace {
	constexpr auto ROTATION_MOVE_SPEED_MAX = 0.015f;
	constexpr auto ROTATION_MOVE_SPEED_MIN = -0.015f;
	constexpr auto ROTATION_ATTENUATION_AMOUNT = 0.0005f;
	constexpr auto MOVE_SPEED = 10;
	constexpr auto MAX_LENGTH = 9500;
	constexpr auto MAX_BULLET = 100;
	constexpr auto BULLET_RELOAD = 3000;
}

Jet::Jet() : ObjectBase("Jet") {
	_input = XInput::GetInstance();
	_model = MV1LoadModel("Res/jet.mv1");
	isCameraBack = true;
	_bulletCoolTime = 0;
	_currentTime = 0;
	_camera = new Camera((_pos - Vector3D(0,0,1) * 1500), _pos);
	isReload = false;
	_forwardVec = Vector3D(0, 0, 1);
	_bulletNum = MAX_BULLET;
	_fontHandle = CreateFontToHandle("メイリオ", 64, 3, DX_FONTTYPE_EDGE);
};

Jet::~Jet(){
	delete _camera;
	DeleteFontToHandle(_fontHandle);
	MV1DeleteModel(_model);
};

void Jet::SetAcceleration(float& param, float input, int max){
	int coefficient = 100;
	if(input != 0.0f){
		float temp = input / (max * coefficient);
		param = Math::Clamp(ROTATION_MOVE_SPEED_MIN, ROTATION_MOVE_SPEED_MAX,param+temp);
	}
	else{
		if (param > 0.0f) {
			param = Math::Max(0.0f,param - ROTATION_ATTENUATION_AMOUNT);
		}
		else if (param < 0.0f) {
			param = Math::Min(0.0f,param + ROTATION_ATTENUATION_AMOUNT);
		}
	}
};

bool Jet::Update(){

	_input->Input();

	if(_bulletCoolTime > 0){
		_bulletCoolTime--;
	}

	if (isReload) {
		if (GetNowCount() - _currentTime > _reloadCoolTime) {
			isReload = false;
			_bulletNum = MAX_BULLET;
		}
	}

	if (_input->GetKey(XINPUT_BUTTON_B) && _bulletNum < MAX_BULLET) {
		if (!isReload) {
			_currentTime = GetNowCount();
			_reloadCoolTime = BULLET_RELOAD / 2;
			isReload = true;
		}
	}

	if (_input->GetKey(XINPUT_BUTTON_A) && _bulletCoolTime <= 0 && _bulletNum > 0 && !isReload) {
		_bulletNum -= 2;
		_bulletCoolTime = 10;
		// 左翼の弾の生成
		MATRIX transPos = MGetTranslate(VScale(VGet(-160, -214, -137), -1));
		MATRIX MixMat = MMult(transPos, MV1GetFrameLocalWorldMatrix(_model, 0));
		SuperManager::GetInstance()->GetManager("objectManager")->Add(new Bullet(Vector3D(MixMat.m[3][0], MixMat.m[3][1], MixMat.m[3][2]), _forwardVec));
		// 右翼の弾の生成
		transPos = MGetTranslate(VScale(VGet(160, -214, 137), -1));
		MixMat = MMult(transPos, MV1GetFrameLocalWorldMatrix(_model, 0));
		SuperManager::GetInstance()->GetManager("objectManager")->Add(new Bullet(Vector3D(MixMat.m[3][0], MixMat.m[3][1], MixMat.m[3][2]), _forwardVec));
		global._soundServer->DirectPlay("SE_Gun");

		if (_bulletNum <= 0) {
			isReload = true;
			_currentTime = GetNowCount();
			_reloadCoolTime = BULLET_RELOAD;
		}
	}

	// 時間制限の処理
	Quaternion temp;
	// 飛行機の移動速度の処理
	SetAcceleration(_airplaneMoveSpeed.x, _input->GetLy(), MAXSHORT);
	SetAcceleration(_airplaneMoveSpeed.y, _input->GetLx(), MAXSHORT);
	SetAcceleration(_airplaneMoveSpeed.z, _input->GetLTrg(), MAXCHAR);
	SetAcceleration(_airplaneMoveSpeed.z, -_input->GetRTrg(), MAXCHAR);
	SetAcceleration(_perspectiveMoveSpeed.x, _input->GetRy(), MAXSHORT);
	SetAcceleration(_perspectiveMoveSpeed.y, _input->GetRx(), MAXSHORT);

	// 飛行機の回転の処理
	Quaternion qx,qy,qz;

	qx.SetToRotateX(_airplaneMoveSpeed.x);
	qy.SetToRotateY(_airplaneMoveSpeed.y);
	qz.SetToRotateZ(_airplaneMoveSpeed.z);

	origin *= qz * qy *  qx;

	// 飛行機モデルの処理
	
	_forwardVec = RotateVectorByQuaternion(Vector3D(0, 0, 1), origin);
	_pos += _forwardVec * MOVE_SPEED;

	if(_pos.Len() > MAX_LENGTH){
		Vector3D arrowVec =  Vector3D(0,0,0) - _pos;
		arrowVec = arrowVec.Normalize();
		_pos = _pos + arrowVec*(_pos.Len()- MAX_LENGTH);
	}

	// カメラの処理

	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) { isCameraBack = !isCameraBack; }

	if (isCameraBack) {
		//	三人称視点の処理
		Vector3D upVec = RotateVectorByQuaternion(Vector3D(0, 1, 0), origin);
		_camera->Update((_pos - _forwardVec * 1500), _pos,upVec);
	}
	else {
		//	一人称視点の処理
		qx.SetToRotateX(_perspectiveMoveSpeed.x);
		qy.SetToRotateY(_perspectiveMoveSpeed.y);
		cameraQuaternion *= qx * qy;
		_forwardVec = RotateVectorByQuaternion(Vector3D(0, 0, 1), origin * cameraQuaternion);
		Vector3D framePos = MV1GetFramePosition(_model, MV1SearchFrame(_model, "Bone_end"));
		Vector3D upVec = RotateVectorByQuaternion(Vector3D(0, 1, 0), origin);
		SetCameraPositionAndTargetAndUpVec(framePos.toVECTOR(), (framePos + _forwardVec).toVECTOR(), upVec.toVECTOR());
	}

	MV1SetRotationXYZ(_model, origin.byEuler().toVECTOR());
	MV1SetPosition(_model, _pos.toVECTOR());

	return true;
}

bool Jet::Draw() {
	MV1DrawModel(_model);
	DrawFormatStringToHandle(1500, 900, GetColor(255, 255, 255),_fontHandle, "Bullet:%d", _bulletNum);
	if(isReload){
		DrawFormatStringToHandle(1500, 800, GetColor(255, 255, 255), _fontHandle, "Reloading...");
	}
	return true;
}