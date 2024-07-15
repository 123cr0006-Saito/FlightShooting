//----------------------------------------------------------------------
// @filename CollisionManager.cpp
// @author: saito ko
// @explanation
// �����蔻��Ƃ��̃��A�N�V�������s���N���X
//----------------------------------------------------------------------
#include "../../Header/Manager/CollisionManager.h"
#include "../../AppFrame/source/Application/UtilMacro.h"
#include "../../AppFrame/source/System/Header/Collision/3DCollision.h"
#include "../AppFrame/source/Mode/ModeServer.h"
#include "../../Header/Mode/ModeGame.h"
#include "../AppFrame/MemoryLeak.h"
#include "../AppFrame/source/Application/Global.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return ����
//----------------------------------------------------------------------
CollisionManager::CollisionManager(){

};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
CollisionManager::~CollisionManager(){
	_addCollisionList.clear();
	_delCollisionNameList.clear();
	_delCollisionList.clear();
	_collisionList.clear();
};
//----------------------------------------------------------------------
// @brief �I������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool CollisionManager::Terminate() {
	return true;
}
//----------------------------------------------------------------------
// @brief �C���X�^���X�̒ǉ�
// @param �ǉ�����C���X�^���X
// @return ����
//----------------------------------------------------------------------
void CollisionManager::AddInput(void* value){
	CollisionBase* collision = static_cast<CollisionBase*>(value);
	_addCollisionList.emplace_back(collision);
};
//----------------------------------------------------------------------
// @brief �폜����C���X�^���X�̖��O��}��
// @param �폜�������C���X�^���X�̖��O
// @return ����
//----------------------------------------------------------------------
void  CollisionManager::DeleteName(std::string name){
	_delCollisionNameList.emplace_back(name);
};
//----------------------------------------------------------------------
// @brief �C���X�^���X�̍폜
// @param �폜����C���X�^���X
// @return ����
//----------------------------------------------------------------------
void CollisionManager::DeleteInput(void* value){
	CollisionBase* collision = static_cast<CollisionBase*>(value);
	_delCollisionList.emplace_back(collision);
};
//----------------------------------------------------------------------
// @brief �i�[���Ă���C���X�^���X�����ׂč폜
// @return ����
//----------------------------------------------------------------------
void CollisionManager::DelAll(){
	for(auto&& list : _addCollisionList){
		delete list;
	}
	for (auto&& list : _collisionList) {
		delete list;
	}
	_collisionList.clear();
	_addCollisionList.clear();
};
//----------------------------------------------------------------------
// @brief �ǉ����Ă����C���X�^���X�̍폜�ƒǉ�
// @return ����������
//----------------------------------------------------------------------
bool CollisionManager::UpdateInit(){
	// deleteList�̒��ɒl������Ƃ��폜
	for (auto list : _delCollisionList) {
		for (auto itr = _collisionList.begin(); itr != _collisionList.end();) {
			if ((*itr) == list) {
				delete (*itr);
				itr = _collisionList.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

	for (auto list : _delCollisionNameList) {
		for (auto itr = _collisionList.begin(); itr != _collisionList.end();) {
			if ((*itr)->GetName() == list) {
				delete (*itr);
				itr = _collisionList.erase(itr);
			}
			else{
				++itr;
			}
		}
	}

	// addList�̒��ɒl������Ƃ��ǉ�
	for (auto list : _addCollisionList) {
		_collisionList.emplace_back(list);
	}

	// addList��deleteList���N���A
	_addCollisionList.clear();
	_delCollisionNameList.clear();
	_delCollisionList.clear();
	return true;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CollisionManager::Update(){
	
	UpdateInit();


	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CollisionManager::Draw(){
#ifdef _DEBUG
	for (auto&& list : _collisionList) {
		list->Render(0xff0000);
	}
#endif
	return true;
};
