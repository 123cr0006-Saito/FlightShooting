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
	//UIの生成
	LoadUI();
	SetCameraNearFar(1.0f, 1000000.0f);

	jetModel = MV1LoadModel("Res/jet.mv1");
	skySphere = MV1LoadModel("Res/SkySphere/skysphere.mv1");
	MV1SetPosition(jetModel, VGet(0, 0, 0));
	MV1SetPosition(skySphere, VGet(0, 0, 0));
	MV1SetScale(skySphere, (Vector3D(1, 1, 1) * 3).toVECTOR());

	input = new XInput();
	isCameraBack = true;

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
// @brief オブジェクトの読み込み
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGame::LoadObject(){
	//ボールの生成
	
	return true;
};
//----------------------------------------------------------------------
// @brief UIの読み込み
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGame::LoadUI(){

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
	input->Input();
	// 時間制限の処理
	float x, y, z;
	Quaternion qx, qy, qz;
	x = y = z = 0;
	x = (float)input->GetLy() / (MAXSHORT * 100);
	y = (float)input->GetLx() / (MAXSHORT * 100);
	z -= (float)input->GetLTrg() / (MAXCHAR * 100);
	z += (float)input->GetRTrg() / (MAXCHAR * 100);
	qx.SetToRotateX(x);
	qy.SetToRotateY(y);
	qz.SetToRotateZ(z);
	origin *= qx;
	origin *= qy;
	origin *= qz;
	MV1SetRotationXYZ(jetModel, origin.byEuler().toVECTOR());
	Vector3D forwardVec = RotateVectorByQuaternion(Vector3D(0, 0, 1), origin);
	pos += forwardVec*10;
	MV1SetPosition(jetModel, pos.toVECTOR());

	if (input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) { isCameraBack = !isCameraBack; }

	if (isCameraBack) {
		Vector3D upVec = RotateVectorByQuaternion(Vector3D(0, 1, 0), origin);
		SetCameraPositionAndTargetAndUpVec((pos - forwardVec * 1500).toVECTOR(), pos.toVECTOR(), upVec.toVECTOR());
	}
	else {
		x = (float)input->GetRy() / (MAXSHORT * 100);
		y = (float)input->GetRx() / (MAXSHORT * 100);
		qx.SetToRotateX(x);
		qy.SetToRotateY(y);
		cameraQuaternion *= qx;
		cameraQuaternion *= qy;
		forwardVec = RotateVectorByQuaternion(Vector3D(0, 0, 1), origin * cameraQuaternion);
		Vector3D framePos = MV1GetFramePosition(jetModel, MV1SearchFrame(jetModel,"Bone_end"));
		Vector3D upVec = RotateVectorByQuaternion(Vector3D(0, 1, 0), origin);
		SetCameraPositionAndTargetAndUpVec(framePos.toVECTOR(), (framePos + forwardVec).toVECTOR(), upVec.toVECTOR());
	}
	return true;
}
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool ModeGame::Render() {
	base::Render();
	MV1DrawModel(jetModel);
	MV1DrawModel(skySphere);
	return true;
}
