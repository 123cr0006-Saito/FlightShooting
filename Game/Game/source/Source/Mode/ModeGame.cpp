//----------------------------------------------------------------------
// @filename ModeGame.cpp
// @author: saito ko
// @explanation
// ゲーム本編を管理するクラス
//----------------------------------------------------------------------
#include "../AppFrame/source/CFile/CFile.h"

#include "../../Header/Mode/ModeGame.h"

#include "../../Header/Manager/SuperManager.h"
#include "../../Header/Manager/RenderManager.h"
#include "../../Header/Manager/ObjectManager.h"
#include "../../Header/Manager/RenderManager.h"
#include "../../Header/Manager/CollisionManager.h"
#include "../../Header/Manager/UIManager.h"


#include "../../Header/Other/Camera/Camera.h"

#include "../../Header/ModeSpline.h"

#include "../../Header/Object/Target.h"
#include "../../Header/Object/Jet.h"

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
	for(int i = 1; i <= 3; i++ ){
		LoadObject("LineData/CatmullSpline_0" + std::to_string(i) +".txt");
	}
	// カメラの生成
	SetCameraNearFar(1.0f, 1000000.0f);
	int ufo = MV1LoadModel("res/UFO.mv1");
	for(int i = 0; i < 3 ; i++){
		for(float j = 0; j < 50; j+=1){
			int speed = rand() % 100;
			int handle = MV1DuplicateModel(ufo);
			Target* target = new Target((float)speed / 100,a[i].first,handle,"Catmull",&a[i].second);
			_superManager->GetManager("objectManager")->AddInput(target);
			
		}
	}

	_superManager->GetManager("objectManager")->AddInput(new Jet());
	
	skySphere = MV1LoadModel("Res/SkySphere/skysphere.mv1");
	
	MV1SetPosition(skySphere, VGet(0, 0, 0));
	MV1SetScale(skySphere, (Vector3D(1, 1, 1) * 2).toVECTOR());
	SetCameraNearFar(1.0f, 1000000.0f);



	return true;
}
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGame::Terminate() {
	base::Terminate();
	
	return true;
}
//----------------------------------------------------------------------
// @brief Objectの読み込み
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGame::LoadObject(std::string name){
	// csvファイルを読み込む
	CFile file(name);
	// ファイルが開けた場合
	if (file.Success()) {

		float speed;
		std::vector<Vector3D> b;
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		c += GetFloatNum(&p[c], &speed); // 名前を取得
		c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
		while (c < size) {
			Vector3D pos;
			c += GetFloatNum(&p[c], &pos.x); //x座標を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.y); //y座標を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &pos.z); //z座標を取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			b.push_back(pos);
		}
		a.push_back(std::make_pair(speed,b));
	}
	else {
		DebugErrar();
	}
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

	_superManager->Update();
	
	return true;
}
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ModeGame::Render() {
	base::Render();
	MV1DrawModel(skySphere);
	_superManager->Draw();

	return true;
}
