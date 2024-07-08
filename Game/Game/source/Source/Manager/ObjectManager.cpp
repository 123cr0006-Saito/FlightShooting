//----------------------------------------------------------------------
// @filename ObjectManager.cpp
// @author: saito ko
// @explanation
// オブジェクトを管理するクラス
//----------------------------------------------------------------------
#include "../../Header/Manager/ObjectManager.h"
#include "../../Header/Object/Base/ObjectBase.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
ObjectManager::ObjectManager(){

};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
ObjectManager::~ObjectManager(){
	
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ObjectManager::Terminate(){
	DelAll();
	return true;
}
//----------------------------------------------------------------------
// @brief インスタンスの追加
// @param 追加するインスタンス
// @return 無し
//----------------------------------------------------------------------
void ObjectManager::AddInput(void* value){
	ObjectBase* object = static_cast<ObjectBase*>(value);
	_addObjectList.emplace_back(object);
}
//----------------------------------------------------------------------
// @brief 削除するインスタンスの名前を挿入
// @param 削除したいインスタンスの名前
// @return 無し
//----------------------------------------------------------------------
void ObjectManager::DeleteName(std::string name){
	_delObjectNameList.emplace_back(name);
};
//----------------------------------------------------------------------
// @brief インスタンスの削除
// @param 削除するインスタンス
// @return 無し
//----------------------------------------------------------------------
void ObjectManager::DeleteInput(void* value){
	ObjectBase* object = static_cast<ObjectBase*>(value);
	_delObjectList.emplace_back(object);
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスをすべて削除
// @return 無し
//----------------------------------------------------------------------
void ObjectManager::DelAll(){
	for (auto&& list : _addObjectList) {
		delete list;
	}
	for(auto&& list : _objectList){
		delete list;
	}
	_objectList.clear();
	_addObjectList.clear();
};
//----------------------------------------------------------------------
// @brief 追加していたインスタンスの削除と追加
// @return 成功したか
//----------------------------------------------------------------------
bool ObjectManager::UpdateInit(){
	for (auto list : _delObjectList) {
		for (auto itr = _objectList.begin(); itr != _objectList.end();) {
			if ((*itr) == list) {
				delete (*itr);
				itr = _objectList.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

	// deleteListの中に値があるとき削除
	for (auto list : _delObjectNameList) {
		for (auto itr = _objectList.begin(); itr != _objectList.end();) {
			if ((*itr)->GetName() == list) {
				delete (*itr);
				itr = _objectList.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

	// addListの中に値があるとき追加
	for (auto list : _addObjectList) {
		_objectList.emplace_back(list);
	}

	_delObjectList.clear();
	_delObjectNameList.clear();
	_addObjectList.clear();
	return true;
}
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ObjectManager::Update(){
	for (auto&& list : _objectList) {
		list->Update();
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 更新終了後に行う処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ObjectManager::UpdateEnd() 
{
	for (auto&& list : _objectList) {
		list->UpdateEnd();
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ObjectManager::Draw() {
#ifdef _DEBUG
	for (auto&& list : _objectList) {
		list->DebugDraw();
	}
#endif
	return true;
};
//----------------------------------------------------------------------
// @brief オブジェクトを名前で検索し取得する
// @param 取得したいインスタンスの名前
// @return 取得できた場合インスタンスを返す　できなかった場合はnullptr
//----------------------------------------------------------------------
ObjectBase* ObjectManager::Get(std::string name){
	for (auto&& list : _objectList) {
		if (list->GetName() == name) {
			return list;
		}
	}
	return nullptr;
};