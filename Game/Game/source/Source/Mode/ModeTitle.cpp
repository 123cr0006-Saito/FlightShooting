//----------------------------------------------------------------------
// @filename ModeTitle.cpp
// @author: saito ko
// @explanation
// �I�u�W�F�N�g���Ǘ�����N���X
//----------------------------------------------------------------------
#include "../../Header/Mode/ModeTitle.h"
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
	_input = NEW XInput(PLAYER_1);
	//BGM��ݒ�
	return true;
};
//----------------------------------------------------------------------
// @brief �I������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeTitle::Terminate(){
	delete _input;
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
	_input->Input();
	//A�{�^���ŃZ���N�g��ʂɈړ�


	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool ModeTitle::Render(){
	return true;
};
