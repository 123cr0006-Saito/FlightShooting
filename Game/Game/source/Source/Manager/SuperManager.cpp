//----------------------------------------------------------------------
// @filename SuperManager.cpp
// @author: saito ko
// @explanation
// マネージャークラスを管理するクラス
//----------------------------------------------------------------------
#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Manager/ManagerBase.h"
#include "../AppFrame/source/Application/UtilMacro.h"
//----------------------------------------------------------------------
SuperManager* SuperManager::_instance = nullptr;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
SuperManager::SuperManager() {
	if (_instance != nullptr) {
		DebugErrar();
		return;
	}
	_instance = this;
	_isSort = false;
	_isProcessSkip = false;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
SuperManager::~SuperManager() {
	DelAll();
	_instance = nullptr;
};
//----------------------------------------------------------------------
// @brief インスタンスの追加
// @param 追加するインスタンスの名前
// @param layer番号
// @param 追加するインスタンス
// @return 無し
//----------------------------------------------------------------------
bool SuperManager::Add(std::string name,int layer, ManagerBase* manager){
	_superManager.emplace_back(std::make_tuple(name,layer,manager));
	_isSort = true;
	return true;
};
//----------------------------------------------------------------------
// @brief 削除するインスタンスの名前を挿入
// @param 削除したいインスタンスの名前
// @return 無し
//----------------------------------------------------------------------
bool SuperManager::DeleteName(std::string name){
	_delSuperManager.emplace_back(name);
	return true;
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスをすべて削除
// @return 無し
//----------------------------------------------------------------------
bool SuperManager::DelAll(){
	for (auto&& list : _superManager) {
		std::get<2>(list)->Terminate();
	}
	for (auto&& list : _superManager) {
		delete std::get<2>(list);
	}
	_superManager.clear();
	return true;
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスを名前で検索し取得
// @param インスタンスの名前
// @return 取得したインスタンス　取得できなかった場合、nullptr
//----------------------------------------------------------------------
ManagerBase* SuperManager::GetManager(std::string name){
	for (auto&& list : _superManager) {
		if (std::get<0>(list) == name) {
			return std::get<2>(list);
		}
	}
	return nullptr;
};
//----------------------------------------------------------------------
// @brief レイヤー番号を参照し順番を入れ替える
// @return 無し
//----------------------------------------------------------------------
void SuperManager::Sort(){
	std::sort(_superManager.begin(), _superManager.end(), [](const std::tuple<std::string, int, ManagerBase*>& a, const std::tuple<std::string, int, ManagerBase*>& b) {
		return std::get<1>(a) < std::get<1>(b);
	});
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SuperManager::Update() {

	for (auto&& list : _delSuperManager) {
		for (auto itr = _superManager.begin(); itr != _superManager.end(); ++itr) {
			if (std::get<0>((*itr)) == list) {
				std::get<2>((*itr))->Terminate();
				delete std::get<2>((*itr));
				_superManager.erase(itr);
				break;
			}
		}
	}

	_delSuperManager.clear();
	_isProcessSkip = false;
	if (_isSort) {
		Sort();
		_isSort = false;
	}

	for(auto itr = _superManager.rbegin() ; itr != _superManager.rend(); ++itr){
		std::get<2>((*itr))->UpdateInit();
		if(!_isProcessSkip){
			//スキップが選択されていたら処理をスキップ
			std::get<2>((*itr))->Update();
		}
		std::get<2>((*itr))->UpdateEnd();
	}
	
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SuperManager::Draw() {
	for (auto&& list : _superManager) {
		std::get<2>(list)->Draw();
	}
	return true;
};
