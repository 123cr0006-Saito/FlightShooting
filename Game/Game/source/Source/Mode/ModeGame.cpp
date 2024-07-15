//----------------------------------------------------------------------
// @filename ModeGame.cpp
// @author: saito ko
// @explanation
// �Q�[���{�҂��Ǘ�����N���X
//----------------------------------------------------------------------
#include "../AppFrame/source/CFile/CFile.h"

#include "../../Header/Mode/ModeGame.h"

#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Manager/RenderManager.h"
#include "../../Header/Manager/ObjectManager.h"
#include "../../Header/Manager/RenderManager.h"
#include "../../Header/Manager/CollisionManager.h"
#include "../../Header/Manager/UIManager.h"


#include "../../Header/Other/Camera/Camera.h"

//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return ����
//----------------------------------------------------------------------
ModeGame::ModeGame() {
	// bgm�̍Đ�
	global._soundServer->DirectPlay("BGM_Game");
};
//----------------------------------------------------------------------
// @brief ����������
// @return ����
//----------------------------------------------------------------------
bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }
	// �}�l�[�W���[�̎擾
	_superManager = SuperManager::GetInstance();
	ChangeLightTypeDir(VGet(0,0,1));
	// �I�u�W�F�N�g�̐���
	LoadObject();
	// �J�����̐���
	//UI�̐���
	LoadUI();
	SetCameraNearFar(1.0f, 1000000.0f);

	jetModel = MV1LoadModel("Res/jet.mv1");
	skySphere = MV1LoadModel("Res/SkySphere/skysphere.mv1");
	MV1SetPosition(jetModel, VGet(0, 0, 0));
	MV1SetPosition(skySphere, VGet(0, 0, 0));
	MV1SetScale(skySphere, (Vector3D(1, 1, 1) * 3).toVECTOR());

	input = new XInput();
	isCameraBack = true;

	return true;
}
//----------------------------------------------------------------------
// @brief �I������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeGame::Terminate() {
	base::Terminate();
	
	return true;
}
//----------------------------------------------------------------------
// @brief �I�u�W�F�N�g�̓ǂݍ���
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeGame::LoadObject(){
	//�{�[���̐���
	
	return true;
};
//----------------------------------------------------------------------
// @brief UI�̓ǂݍ���
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeGame::LoadUI(){

	return true;
};
//----------------------------------------------------------------------
// @brief �t�@�C������I�u�W�F�N�g�̃p�����[�^��ǂݍ���
// @param �ǂݍ��ރI�u�W�F�N�g�̃t�@�C���̖��O
// @return �I�u�W�F�N�g�� ���W �T�C�Y ��z��ɂ��ĕԂ�
//----------------------------------------------------------------------
std::vector<std::tuple<std::string, Vector3D, Vector3D>> ModeGame::LoadObjectParam(std::string fileName) {
	std::vector<std::tuple<std::string, Vector3D, Vector3D>> nameList;
	// csv�t�@�C����ǂݍ���
	CFile file(fileName);
	// �t�@�C�����J�����ꍇ
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string name;
			Vector3D pos,param;
			c += GetString(&p[c],&name); // ���O���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.x); //x���W���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.y); //y���W���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.z); //z���W���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &param.x); //x�̒������擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &param.y); //y�̒������擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &param.z); //z�̒������擾
			c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
			nameList.push_back(std::make_tuple(name,pos,param));
		}
	}
	else {
		DebugErrar();
	}
	return nameList;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool ModeGame::Process() {
	base::Process();
	input->Input();
	// ���Ԑ����̏���
	float x, y, z;
	Quaternion qx, qy, qz;
	x = y = z = 0;
	x = (float)input->GetLy() / (MAXSHORT * 100);
	y = (float)input->GetLx() / (MAXSHORT * 100);
	z -= (float)input->GetLTrg() / (MAXCHAR * 100);
	z += (float)input->GetRTrg() / (MAXCHAR * 100);
	qx.SetToRotateX(x);
	qy.SetToRotateY(y);
	qz.SetToRotateZ(z);
	origin *= qx;
	origin *= qy;
	origin *= qz;
	MV1SetRotationXYZ(jetModel, origin.byEuler().toVECTOR());
	Vector3D forwardVec = RotateVectorByQuaternion(Vector3D(0, 0, 1), origin);
	pos += forwardVec*10;
	MV1SetPosition(jetModel, pos.toVECTOR());

	if (input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) { isCameraBack = !isCameraBack; }

	if (isCameraBack) {
		Vector3D upVec = RotateVectorByQuaternion(Vector3D(0, 1, 0), origin);
		SetCameraPositionAndTargetAndUpVec((pos - forwardVec * 1500).toVECTOR(), pos.toVECTOR(), upVec.toVECTOR());
	}
	else {
		x = (float)input->GetRy() / (MAXSHORT * 100);
		y = (float)input->GetRx() / (MAXSHORT * 100);
		qx.SetToRotateX(x);
		qy.SetToRotateY(y);
		cameraQuaternion *= qx;
		cameraQuaternion *= qy;
		forwardVec = RotateVectorByQuaternion(Vector3D(0, 0, 1), origin * cameraQuaternion);
		Vector3D framePos = MV1GetFramePosition(jetModel, MV1SearchFrame(jetModel,"Bone_end"));
		Vector3D upVec = RotateVectorByQuaternion(Vector3D(0, 1, 0), origin);
		SetCameraPositionAndTargetAndUpVec(framePos.toVECTOR(), (framePos + forwardVec).toVECTOR(), upVec.toVECTOR());
	}
	return true;
}
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool ModeGame::Render() {
	base::Render();
	MV1DrawModel(jetModel);
	MV1DrawModel(skySphere);
	return true;
}
