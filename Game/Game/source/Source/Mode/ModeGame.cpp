//----------------------------------------------------------------------
// @filename ModeGame.cpp
// @author: saito ko
// @explanation
// ゲーム本編を管理するクラス
//----------------------------------------------------------------------
#include "../AppFrame/source/CFile/CFile.h"

#include "../../Header/Mode/ModeGame.h"
#include "../../Header/Mode/ModeGameEnd.h"

#include "../../Header/Object/Player/Player.h"
#include "../../Header/Object/Stage/Stage.h"
#include "../../Header/Object/Stage/Ball.h"
#include "../../Header/Object/Stage/Goal.h"
#include "../../Header/Object/Stage/Wall.h"

#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Manager/RenderManager.h"
#include "../../Header/Manager/ObjectManager.h"
#include "../../Header/Manager/RenderManager.h"
#include "../../Header/Manager/CollisionManager.h"
#include "../../Header/Manager/UIManager.h"
#include "../../Header/Manager/PlayerManager.h"

#include "../../Header/Other/Score.h"
#include "../../Header/Other/Camera/Camera.h"
#include "../../Header/Other/TimeLimit.h"

#include "../../Header/UI/Score/UIScoreBoard.h"
#include "../../Header/UI/UIStartCount.h"
#include "../../Header/UI/UITimer.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
ModeGame::ModeGame() {
	// bgmの再生
	global._soundServer->DirectPlay("BGM_Game");
};
//----------------------------------------------------------------------
// @brief 初期化処理
// @return 無し
//----------------------------------------------------------------------
bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }
	// マネージャーの取得
	_superManager = SuperManager::GetInstance();
	ChangeLightTypeDir(VGet(0,0,1));
	// オブジェクトの生成
	LoadObject();
	// カメラの生成
	_camera = Camera::GetInstance();
	_camera->SetIsGame(true);
	// スコアクラスをsカウ性
	_score = NEW Score();
	// 時間制限を管理するクラスを作成
	_timeLimit = NEW TimeLimit();
	// 時間制限を設定
	_timeLimit->SetTimeLimit(2, 0);
	_timeLimit->Stop();
	//UIの生成
	LoadUI();

	_isAddBall = false;
	_currentTime = GetNowCount();
	return true;
}
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGame::Terminate() {
	base::Terminate();
	// オブジェクトの削除
	for(auto&& list : _objectName){
		_superManager->GetManager("objectManager")->DeleteName(list);
	}

	// UIの削除
	std::vector< Player*> player = dynamic_cast<PlayerManager*>(_superManager->GetManager("playerManager"))->GetList();
	for(auto&& list : player){
		_superManager->GetManager("uiManager")->DeleteName(list->GetName() + "Frame");
		_superManager->GetManager("uiManager")->DeleteName(list->GetName() + "Icon");
	}
	_superManager->GetManager("uiManager")->DeleteName("scoreBoard");
	_superManager->GetManager("uiManager")->DeleteName("scoreNum");
	_superManager->GetManager("uiManager")->DeleteName("Time");

	// カメラをゲーム以外状態に設定　カメラの位置を設定
	_camera->SetIsGame(false);
	_camera->SetPos(Vector3D(0, 800, -2000));
	_camera->SetTarget(Vector3D(0, 0, 0));

	delete _timeLimit;
	return true;
}
//----------------------------------------------------------------------
// @brief ボールとプレイヤーの位置を初期化　ボールが二つある場合は削除
// @return 無し
//----------------------------------------------------------------------
void ModeGame::ReSetGame(){
	// ボールの位置を設定
	ObjectManager* objectManager = dynamic_cast<ObjectManager*>(_superManager->GetManager("objectManager"));
	Ball* ball = dynamic_cast<Ball*>(objectManager->Get("Ball"));
	ball->SetPos(Vector3D(0, 350, 0));
	ball->SetSpeed(0);
	objectManager->DeleteName("Ball_2");
	PlayerManager* playerManager = dynamic_cast<PlayerManager*>(_superManager->GetManager("playerManager"));
	playerManager->InitParam();
	UIStartCount* uiStartCount = NEW UIStartCount();
	_isAddBall = false;
	_currentTime = GetNowCount();
};
//----------------------------------------------------------------------
// @brief オブジェクトの読み込み
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGame::LoadObject(){
	//ボールの生成
	_superManager->GetManager("objectManager")->Add(NEW Ball("Ball"));
	_objectName.push_back("Ball");
	_objectName.push_back("Ball_2");
	//ゴールの生成
	std::vector<std::tuple<std::string, Vector3D, Vector3D>> goalList = LoadObjectParam("Data/GoalParam.csv");
	int count = 1;
	for (auto&& list : goalList) {
		std::string name = std::get<0>(list) + std::to_string(count);
		Goal* goal = new Goal(name, std::get<1>(list), std::get<2>(list).Radian());
		_superManager->GetManager("objectManager")->Add(goal);
		_objectName.push_back(name);
		count++;
	}
	//ゴールネットコリジョンの生成
	std::vector<std::tuple<std::string, Vector3D, Vector3D>> goalNetList = LoadObjectParam("Data/GoalNetParam.csv");
	for (auto&& list : goalNetList) {
		Wall* wall = NEW Wall(std::get<0>(list), std::get<1>(list), std::get<2>(list));
		_superManager->GetManager("objectManager")->Add(wall);
		_objectName.push_back(std::get<0>(list));
	}
	//壁コリジョンの生成
	std::vector<std::tuple<std::string, Vector3D, Vector3D>> wallList = LoadObjectParam("Data/WallParam.csv");
	for (auto&& list : wallList) {
		Wall* wall = NEW Wall(std::get<0>(list), std::get<1>(list), std::get<2>(list));
		_superManager->GetManager("objectManager")->Add(wall);
		_objectName.push_back(std::get<0>(list));
	}

	return true;
};
//----------------------------------------------------------------------
// @brief UIの読み込み
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGame::LoadUI(){
	Vector3D scoreBoardPos[2] = { Vector3D(600,100,0),Vector3D(1300,100,0) };

	for (int i = 0; i < 2; i++) {
		UIScoreBoard* uiScore = NEW UIScoreBoard(scoreBoardPos[i], "Goal_" + std::to_string(i + 1), _score);
	}

	std::vector<Player*> player = dynamic_cast<PlayerManager*>(_superManager->GetManager("playerManager"))->GetList();
	int size = player.size();
	int shouldMoveIcon = size > 2 ? 1 : 0;
	for(int i = 0; i < size; i++){
		std::string name = player[i]->GetName();
		int handle = ResourceServer::LoadGraph(name + "Icon", ("Res/UI/Icon/" + name + ".png").c_str());
		float iconOfset = i >= 2 ? 50 : -50;
		UIBase* uiIcon = NEW UIBase(name + "Icon", scoreBoardPos[i % 2] + Vector3D(iconOfset * shouldMoveIcon, 150, 0), 0.2f, 255, handle,1000+i);
		SuperManager::GetInstance()->GetManager("uiManager")->Add(uiIcon);
	}

	UIStartCount* uiStartCount = NEW UIStartCount();
	UITimer* uiTimer = NEW UITimer(_timeLimit);
	return true;
};
//----------------------------------------------------------------------
// @brief ファイルからオブジェクトのパラメータを読み込む
// @param 読み込むオブジェクトのファイルの名前
// @return オブジェクト名 座標 サイズ を配列にして返す
//----------------------------------------------------------------------
std::vector<std::tuple<std::string, Vector3D, Vector3D>> ModeGame::LoadObjectParam(std::string fileName) {
	std::vector<std::tuple<std::string, Vector3D, Vector3D>> nameList;
	// csvファイルを読み込む
	CFile file(fileName);
	// ファイルが開けた場合
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string name;
			Vector3D pos,param;
			c += GetString(&p[c],&name); // 名前を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.x); //x座標を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.y); //y座標を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.z); //z座標を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &param.x); //xの長さを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &param.y); //yの長さを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &param.z); //zの長さを取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			nameList.push_back(std::make_tuple(name,pos,param));
		}
	}
	else {
		DebugErrar();
	}
	return nameList;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ModeGame::Process() {
	base::Process();
	// カメラの処理
	_camera->Update();
	// 時間制限の処理
	_timeLimit->Update();

	int ballCount = GetNowCount() - _currentTime ;

	if (ballCount > 20000 && !_isAddBall) {
		// 20秒たったら2つめのボールの生成
		Ball* ball = NEW Ball("Ball_2");
		ball->SetPos(Vector3D(0,5000,0));
		_superManager->GetManager("objectManager")->Add(ball);
		_isAddBall = true;
	}
	// 時間が0になったらゲームエンドクラスを作成
	ModeServer* modeServer = ModeServer::GetInstance();
	if (_timeLimit->GetTimeLimit() <= 0 && !modeServer->Search("ModeGameEnd")) {
		ModeServer::GetInstance()->Add(NEW ModeGameEnd(),10,"ModeGameEnd");
	}

	return true;
}
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ModeGame::Render() {
	base::Render();
	return true;
}
