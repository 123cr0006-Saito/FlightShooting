//----------------------------------------------------------------------
// @filename PlayerManager.cpp
// @author: saito ko
// @explanation
// プレイヤーを管理するクラス
//----------------------------------------------------------------------
#include "../../Header/Manager/PlayerManager.h"
#include "../../Header/Object/Player/Player.h"
#include "../AppFrame/source/System/Header/Input/XInput.h"
#include "../AppFrame/source/Application/UtilMacro.h"
#include "../AppFrame/source/CFile/CFile.h"
#include "../../Header/UI/UIPlayerParam.h"
#include "../AppFrame/source/System/Header/Input/XInput.h"
#include "../MemoryLeak.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
PlayerManager::PlayerManager() {
	
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
PlayerManager::~PlayerManager(){
	_player.clear();
	_originPos.clear();
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool PlayerManager::Terminate(){
	DelAll();
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool PlayerManager::Update(){
	for (auto&& list : _player) {
		list->Update();
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 更新終了後に行う処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool PlayerManager::UpdateEnd(){
	for (auto&& list : _player) {
		list->UpdateEnd();
	}
	return true;
};
//----------------------------------------------------------------------
// @brief プレイヤーの追加　それに合わせてプレイヤーキャラに対応したUIを追加
// @param プレイヤーの名前
// @param パッドの入力
// @param プレイヤーのモデルハンドル
// @return 無し
//----------------------------------------------------------------------
void PlayerManager::Add(std::vector<std::tuple<std::string, XInput*, int>> param) {

	float uiX = 1920.0f / (param.size() + 1);
	for (int i = 0; i < param.size(); i++) {
		_player.emplace_back(NEW Player(std::get<0>(param[i]), std::get<1>(param[i]), std::get<2>(param[i])));
		UIPlayerParam* ui = NEW UIPlayerParam(_player[i], std::get<0>(param[i]), Vector3D(uiX * (i + 1), 900, 0));
	}
	InitParam();
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスをすべて削除
// @return 無し
//----------------------------------------------------------------------
void PlayerManager::DelAll(){
	for (auto&& list : _player) {
		delete list;
	}
	_player.clear();
	_originPos.clear();
};
//----------------------------------------------------------------------
// @brief プレイヤーの位置　スタミナ　シュート力を初期化
// @return 無し
//----------------------------------------------------------------------
void PlayerManager::InitParam(){
	for(int i = 0; i < _player.size(); i++){
		_player[i]->SetPos(Vector3D(1500.0f * pow(-1, i+1), 0,i / 2 * 2000.0f));
		_player[i]->SetStamina(100);
		_player[i]->SetPower(0);
	}
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool PlayerManager::Draw(){
#ifdef _DEBUG
	for(auto&& list : _player){
		list->DebugDraw();
	}
#endif
	return true;
}