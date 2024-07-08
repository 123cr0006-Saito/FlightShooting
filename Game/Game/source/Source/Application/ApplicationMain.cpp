#include "../../Header/Application/ApplicationMain.h"
#include "../../Header/Mode/ModeGame.h"
#include "../../Header/Mode/ModeSelectPlayer.h"
#include "../../Header/Mode/ModeTitle.h"
#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Manager/RenderManager.h"
#include "../../Header/Manager/CollisionManager.h"
#include "../../Header/Manager/ObjectManager.h"
#include "../../Header/Manager/UIManager.h"
// 実体
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }
	// 音声の読み込み
	global.SoundLoad();
	
	// 前の処理から今の処理の計算時間を得るクラス
	_timer = NEW Timer();
	_timer->Start();
	// FPSを安定させるためのクラスを初期化
	_fpsController = NEW Fps();

	// 使用するマネージャークラスを登録
	_superManager = NEW SuperManager();
	RenderManager* renderManager = NEW RenderManager();
	CollisionManager* collisionManager = NEW CollisionManager();
	ObjectManager* objectManager = NEW ObjectManager();
	UIManager* uiManager = NEW UIManager();
	_superManager->Add("renderManager", 100, renderManager);
	_superManager->Add("collisionManager", 1, collisionManager);
	_superManager->Add("objectManager", 10, objectManager);
	_superManager->Add("uiManager", 1000, uiManager);

	// モードの登録
	ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "ModeTitle");
	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	delete _timer; _timer = nullptr;
	delete _fpsController; _fpsController = nullptr;
	delete _superManager; _superManager = nullptr;
	ResourceServer::DeleteResourceAll();
	return true;
}

bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	_superManager->Update();
	_fpsController->WaitFps();
	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	_superManager->Draw();
#ifdef _DEBUG
	_fpsController->DrawFps(0,0);
#endif
	return true;
}