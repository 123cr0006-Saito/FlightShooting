//----------------------------------------------------------------------
// @filename ModeTitle.cpp
// @author: saito ko
// @explanation
// オブジェクトを管理するクラス
//----------------------------------------------------------------------
#include "../../Header/Mode/ModeTitle.h"
#include "../AppFrame/source/CFile/CFile.h"
#include "../AppFrame/source/Mode/ModeServer.h"
#include "../../Header/Mode/ModeFadeComeBack.h"
#include "../../Header/Mode/ModeSelectPlayer.h"
#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Manager/UIManager.h"
#include "../AppFrame/MemoryLeak.h"
#include "../../Header/UI/Animation/IterationAnim.h"
#include "../../Header/UI/Animation/LocationAnim.h"
#include "../AppFrame/source/Application/Global.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
ModeTitle::ModeTitle(){

};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
ModeTitle::~ModeTitle(){

};
//----------------------------------------------------------------------
// @brief 初期化処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeTitle::Initialize(){
	_superManager = SuperManager::GetInstance();
	int loopCount = 0;
	CFile file("Data/UITitleParam.csv");
	// ファイルが開けた場合
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string name,handlePath,animPath;
			int handle;
			Vector3D pos, center, extrate;
			float angle,alpha;
			c += GetString(&p[c], &name); // タグを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetString(&p[c], &handlePath); // 画像ファイル名を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetString(&p[c], &animPath); // アニメーションファイル名を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.x); //x座標を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.y); //y座標を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &center.x); //中心座用を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &center.y); //中心座用を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &extrate.x); //x拡大率を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &extrate.y); //y拡大率を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &angle); //回転率を取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする

			handle = ResourceServer::LoadGraph(name,handlePath.c_str());
			alpha = 255;

			UIRotaBase* ui = NEW UIRotaBase(name,pos, center, extrate, angle, alpha, handle,100+loopCount);
			if(animPath != ""){
				IterationAnim* anim = NEW IterationAnim(ui, animPath,true);
				ui->SetAnimation(anim);
			}
			_superManager->GetManager("uiManager")->Add(ui);
			_uiName.push_back(name);
			loopCount++;
		}
	}
	else {
		DebugErrar();
	}

	
	//パッドの作成
	_input = NEW XInput(PLAYER_1);
	//BGMを設定
	global._soundServer->DirectPlay("BGM_Title");
	return true;
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeTitle::Terminate(){
	delete _input;
	// UIを削除
	for(auto&& name : _uiName){
		SuperManager::GetInstance()->GetManager("uiManager")->DeleteName(name);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ModeTitle::Process(){
	_input->Input();
	//Aボタンでセレクト画面に移動
	if(_input->GetTrg(XINPUT_BUTTON_A)){
		ModeServer::GetInstance()->Add(NEW ModeSelectPlayer(), 0, "ModeSelectPlayer");
		std::vector<std::string> modeName = {"ModeTitle"};
		ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(1000, modeName, "ModeSelectPlayer", 10, true), 100, "ModeFadeComeBack");
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ModeTitle::Render(){
	return true;
};
