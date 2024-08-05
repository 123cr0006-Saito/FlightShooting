//----------------------------------------------------------------------
// @filename ModeTitle.cpp
// @author: saito ko
// @explanation
// �I�u�W�F�N�g���Ǘ�����N���X
//----------------------------------------------------------------------
#include "../../Header/Mode/ModeTitle.h"
#include "../../Header/Mode/ModeGame.h"
#include "../AppFrame/source/CFile/CFile.h"
#include "../AppFrame/source/Mode/ModeServer.h"
#include "../../Header/Mode/ModeFadeComeBack.h"
#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Manager/UIManager.h"
#include "../AppFrame/MemoryLeak.h"
#include "../../Header/UI/Animation/IterationAnim.h"
#include "../../Header/UI/Animation/LocationAnim.h"
#include "../AppFrame/source/Application/Global.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return ����
//----------------------------------------------------------------------
ModeTitle::ModeTitle(){
	_fontHandle = CreateFontToHandle("���C���I",128,3, DX_FONTTYPE_EDGE);
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
ModeTitle::~ModeTitle(){

};
//----------------------------------------------------------------------
// @brief ����������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeTitle::Initialize(){
	_superManager = SuperManager::GetInstance();
	//�p�b�h�̍쐬
	_input = XInput::GetInstance();
	//BGM��ݒ�
	return true;
};
//----------------------------------------------------------------------
// @brief �I������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeTitle::Terminate(){
	// UI���폜
	for(auto&& name : _uiName){
		SuperManager::GetInstance()->GetManager("uiManager")->DeleteName(name);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool ModeTitle::Process(){
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "ModeGame");
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool ModeTitle::Render(){
	DrawFormatStringToHandle(1920 / 2 - 450, 1080 / 2 - 300, GetColor(255, 255, 255), _fontHandle, "Flite Shooting");
	DrawFormatStringToHandle(1920 / 2 - 400, 1080/2+100, GetColor(255, 255, 255), _fontHandle,"Game Start");
	return true;
};
