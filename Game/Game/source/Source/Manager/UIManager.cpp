//----------------------------------------------------------------------
// @filename UIManager.cpp
// @author: saito ko
// @explanation
// オブジェクトを管理するクラス
//----------------------------------------------------------------------
#include "../../Header/Manager/UIManager.h"
#include "../../Header/UI/Base/UIBase.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
UIManager::UIManager(){

};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
UIManager::~UIManager(){
	_uiList.clear();
	_addUiList.clear();
	_delUiNameList.clear();
	_delUiList.clear();
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool UIManager::Terminate(){
	DelAll();
	return true;
};
//----------------------------------------------------------------------
// @brief インスタンスの追加
// @param 追加するインスタンス
// @return 無し
//----------------------------------------------------------------------
void UIManager::AddInput(void* value){
	UIBase* ui = static_cast<UIBase*>(value);
	_addUiList.emplace_back(ui);
};
//----------------------------------------------------------------------
// @brief 削除するインスタンスの名前を挿入
// @param 削除したいインスタンスの名前
// @return 無し
//----------------------------------------------------------------------
void UIManager::DeleteName(std::string ui){
	_delUiNameList.emplace_back(ui);
};
//----------------------------------------------------------------------
// @brief インスタンスの削除
// @param 削除するインスタンス
// @return 無し
//----------------------------------------------------------------------
void UIManager::DeleteInput(void* value){
	UIBase* ui = static_cast<UIBase*>(value);
	_delUiList.emplace_back(ui);
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスをすべて削除
// @return 無し
//----------------------------------------------------------------------
void UIManager::DelAll(){
	for (auto&& list : _uiList) {
		delete list;
	}
	for (auto&& list : _addUiList) {
		delete list;
	}
	_uiList.clear();
	_addUiList.clear();
	_delUiNameList.clear();
};
//----------------------------------------------------------------------
// @brief レイヤー番号を参照し順番を入れ替える
// @return 無し
//----------------------------------------------------------------------
void UIManager::Sort() {
	std::sort(_uiList.begin(), _uiList.end(), []( UIBase* &a, UIBase* &b) {
		return a->GetLayer() < b->GetLayer();
	});
};
//----------------------------------------------------------------------
// @brief 追加していたインスタンスの削除と追加
// @return 成功したか
//----------------------------------------------------------------------
bool UIManager::UpdateInit(){
	// deleteListの中に値があるとき削除

	for (auto list : _delUiList) {
		for (auto itr = _uiList.begin(); itr != _uiList.end();) {
			if ((*itr) == list) {
				delete (*itr);
				itr = _uiList.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

	for (auto list : _delUiNameList) {
		for (auto itr = _uiList.begin(); itr != _uiList.end();) {
			if ((*itr)->GetName() == list) {
				delete (*itr);
				itr = _uiList.erase(itr);
			}
			else{
				++itr;
			}
		}
	}

	bool isSort = false; // ソートを行うかどうかのフラグ

	// addListの中に値があるとき追加
	for (auto list : _addUiList) {
		_uiList.emplace_back(list);
		isSort = true;
	}

	// モデルを追加した時にソート
	if (isSort) {
		Sort();
	}

	// addListとdeleteListをクリア
	_addUiList.clear();
	_delUiList.clear();
	_delUiNameList.clear();
	return true;
}
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool UIManager::Update(){
	// UIの更新
	for (auto&& ui : _uiList) {
		ui->Update();
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool UIManager::Draw(){
	for (auto&& ui : _uiList) {
		ui->Draw();
	}
	return true;
};