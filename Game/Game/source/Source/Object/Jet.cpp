#include "../../Header/Object/Jet.h"
#include "../../Header/Object/Bullet.h"
#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Other/Camera/Camera.h"
namespace {
	constexpr auto ROTATION_MOVE_SPEED_MAX = 0.015f;
	constexpr auto ROTATION_MOVE_SPEED_MIN = -0.015f;
	constexpr auto ROTATION_ATTENUATION_AMOUNT = 0.0005f;
	constexpr auto MOVE_SPEED = 10;
}

Jet::Jet() : ObjectBase("Jet") {
	_input = new XInput();
	_model = MV1LoadModel("Res/jet.mv1");
	isCameraBack = true;
	_coolTime = 0;
	_camera = new Camera((_pos - Vector3D(0,0,1) * 1500), _pos);
};

Jet::~Jet(){

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

	if(_coolTime > 0){
		_coolTime--;
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

	origin *= qx * qy *  qz;

	// 飛行機モデルの処理
	MV1SetRotationXYZ(_model, origin.byEuler().toVECTOR());
	_forwardVec = RotateVectorByQuaternion(Vector3D(0, 0, 1), origin);
	_pos += _forwardVec * MOVE_SPEED;
	MV1SetPosition(_model, _pos.toVECTOR());

	if(_input->GetKey(XINPUT_BUTTON_A) && _coolTime <= 0){
		_coolTime = 10;
		// 左翼の弾の生成
		MATRIX transPos = MGetTranslate(VScale(VGet(-160,-214,-137),-1));
		MATRIX MixMat = MMult(transPos,MV1GetFrameLocalWorldMatrix(_model,0));
		SuperManager::GetInstance()->GetManager("objectManager")->Add(new Bullet(Vector3D(MixMat.m[3][0], MixMat.m[3][1], MixMat.m[3][2]), _forwardVec));
		// 右翼の弾の生成
		transPos = MGetTranslate(VScale(VGet(160, -214, 137), -1));
		MixMat = MMult(transPos, MV1GetFrameLocalWorldMatrix(_model, 0));
		SuperManager::GetInstance()->GetManager("objectManager")->Add(new Bullet(Vector3D(MixMat.m[3][0], MixMat.m[3][1], MixMat.m[3][2]), _forwardVec));
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
	return true;
}

bool Jet::Draw() {
	MV1DrawModel(_model);
	return true;
}