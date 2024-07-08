//----------------------------------------------------------------------
// @filename RenderManager.cpp
// @author: saito ko
// @explanation
// 3Dモデルの描画を管理するクラス
//----------------------------------------------------------------------
#include "../../Header/Manager/RenderManager.h"
#include "../AppFrame/source/Application/UtilMacro.h"
#include "../../Header/Model/Base/ModelBase.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
RenderManager::RenderManager() {

};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
RenderManager::~RenderManager() {
	_modelList.clear();
	_addModelList.clear();
	_delModelList.clear();
	_delModelNameList.clear();
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool RenderManager::Terminate(){
	return true;
}
//----------------------------------------------------------------------
// @brief インスタンスの追加
// @param 追加するインスタンス
// @return 無し
//----------------------------------------------------------------------
void RenderManager::AddInput(void* value){
	ModelBase* model = static_cast<ModelBase*>(value);
	_addModelList.emplace_back(model);
};
//----------------------------------------------------------------------
// @brief 削除するインスタンスの名前を挿入
// @param 削除したいインスタンスの名前
// @return 無し
//----------------------------------------------------------------------
void RenderManager::DeleteName(std::string name){
	_delModelNameList.emplace_back(name);
};
//----------------------------------------------------------------------
// @brief インスタンスの削除
// @param 削除するインスタンス
// @return 無し
//----------------------------------------------------------------------
void RenderManager::DeleteInput(void* value){
	ModelBase* model = static_cast<ModelBase*>(value);
	_delModelList.emplace_back(model);
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスをすべて削除
// @return 無し
//----------------------------------------------------------------------
void RenderManager::DelAll() {
	for(auto&& list : _modelList){
		delete list;
	}
	for(auto&& list : _addModelList){
		delete list;
	}

	_modelList.clear();
	_addModelList.clear();
	_delModelList.clear();
	_delModelNameList.clear();
}
//----------------------------------------------------------------------
// @brief モデルクラスのレイヤー番号を参照し順番を入れ替える
// @return 無し
//----------------------------------------------------------------------
void RenderManager::Sort(){
	std::sort(_modelList.begin(), _modelList.end(), [](ModelBase* a,ModelBase* b) {
		return a->GetLayer() < b->GetLayer();
	});
};
//----------------------------------------------------------------------
// @brief 追加していたインスタンスの削除と追加
// @return 成功したか
//----------------------------------------------------------------------
bool RenderManager::UpdateInit(){
	for (auto list : _delModelList) {
		for (auto itr = _modelList.begin(); itr != _modelList.end();) {
			if ((*itr) == list) {
				delete (*itr);
				itr = _modelList.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

	// deleteListの中に値があるとき削除
	for (auto list : _delModelNameList) {
		for (auto itr = _modelList.begin(); itr != _modelList.end();) {
			if ((*itr)->GetName() == list) {
				delete (*itr);
				itr = _modelList.erase(itr);
			}
			else{
				++itr;
			}
		}
	}

	bool isSort = false; // ソートを行うかどうかのフラグ

	// addListの中に値があるとき追加
	for (auto list : _addModelList) {
		_modelList.push_back(list);
		isSort = true;
	}

	// モデルを追加した時にソート
	if (isSort) {
		Sort();
	}

	// 念のためリストのクリア
	_addModelList.clear();
	_delModelList.clear();
	_delModelNameList.clear();
	return true;
}
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool RenderManager::Update() {
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool RenderManager::Draw() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);
	for (auto list : _modelList) {
		list->Render();
	}
	return true;
};