//----------------------------------------------------------------------
// @filename CollisionManager.cpp
// @author: saito ko
// @explanation
// 当たり判定とそのリアクションを行うクラス
//----------------------------------------------------------------------
#include "../../Header/Manager/CollisionManager.h"
#include "../../AppFrame/source/Application/UtilMacro.h"
#include "../../AppFrame/source/System/Header/Collision/3DCollision.h"
#include "../AppFrame/source/Mode/ModeServer.h"
#include "../../Header/Mode/ModeGame.h"
#include "../AppFrame/MemoryLeak.h"
#include "../AppFrame/source/Application/Global.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
CollisionManager::CollisionManager(){

};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
CollisionManager::~CollisionManager(){
	_addCollisionList.clear();
	_delCollisionNameList.clear();
	_delCollisionList.clear();
	_collisionList.clear();
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool CollisionManager::Terminate() {
	return true;
}
//----------------------------------------------------------------------
// @brief インスタンスの追加
// @param 追加するインスタンス
// @return 無し
//----------------------------------------------------------------------
void CollisionManager::AddInput(void* value){
	CollisionBase* collision = static_cast<CollisionBase*>(value);
	_addCollisionList.emplace_back(collision);
};
//----------------------------------------------------------------------
// @brief 削除するインスタンスの名前を挿入
// @param 削除したいインスタンスの名前
// @return 無し
//----------------------------------------------------------------------
void  CollisionManager::DeleteName(std::string name){
	_delCollisionNameList.emplace_back(name);
};
//----------------------------------------------------------------------
// @brief インスタンスの削除
// @param 削除するインスタンス
// @return 無し
//----------------------------------------------------------------------
void CollisionManager::DeleteInput(void* value){
	CollisionBase* collision = static_cast<CollisionBase*>(value);
	_delCollisionList.emplace_back(collision);
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスをすべて削除
// @return 無し
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
// @brief 追加していたインスタンスの削除と追加
// @return 成功したか
//----------------------------------------------------------------------
bool CollisionManager::UpdateInit(){
	// deleteListの中に値があるとき削除
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

	// addListの中に値があるとき追加
	for (auto list : _addCollisionList) {
		_collisionList.emplace_back(list);
	}

	// addListとdeleteListをクリア
	_addCollisionList.clear();
	_delCollisionNameList.clear();
	_delCollisionList.clear();
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CollisionManager::Update(){
	
	UpdateInit();


	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CollisionManager::Draw(){
#ifdef _DEBUG
	for (auto&& list : _collisionList) {
		list->Render(0xff0000);
	}
#endif
	return true;
};
