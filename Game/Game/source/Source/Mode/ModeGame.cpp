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

#include "../../Header/ModeSpline.h"

#include "../../Header/Object/Target.h"
#include "../../Header/Object/Jet.h"

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
	for(int i = 1; i <= 3; i++ ){
		LoadObject("LineData/CatmullSpline_0" + std::to_string(i) +".txt");
	}
	// �J�����̐���
	SetCameraNearFar(1.0f, 1000000.0f);
	int ufo = MV1LoadModel("res/UFO.mv1");
	for(int i = 0; i < 3 ; i++){
		for(float j = 0; j < 50; j+=1){
			int speed = rand() % 100;
			int handle = MV1DuplicateModel(ufo);
			Target* target = new Target((float)speed / 100,a[i].first,handle,"Catmull",&a[i].second);
			_superManager->GetManager("objectManager")->AddInput(target);
			
		}
	}

	_superManager->GetManager("objectManager")->AddInput(new Jet());
	
	skySphere = MV1LoadModel("Res/SkySphere/skysphere.mv1");
	
	MV1SetPosition(skySphere, VGet(0, 0, 0));
	MV1SetScale(skySphere, (Vector3D(1, 1, 1) * 2).toVECTOR());
	SetCameraNearFar(1.0f, 1000000.0f);



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
// @brief Object�̓ǂݍ���
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeGame::LoadObject(std::string name){
	// csv�t�@�C����ǂݍ���
	CFile file(name);
	// �t�@�C�����J�����ꍇ
	if (file.Success()) {

		float speed;
		std::vector<Vector3D> b;
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		c += GetFloatNum(&p[c], &speed); // ���O���擾
		c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
		while (c < size) {
			Vector3D pos;
			c += GetFloatNum(&p[c], &pos.x); //x���W���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.y); //y���W���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.z); //z���W���擾
			c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
			b.push_back(pos);
		}
		a.push_back(std::make_pair(speed,b));
	}
	else {
		DebugErrar();
	}
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

	_superManager->Update();
	
	return true;
}
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool ModeGame::Render() {
	base::Render();
	MV1DrawModel(skySphere);
	_superManager->Draw();

	return true;
}
