//----------------------------------------------------------------------
// @filename CollisionManager.cpp
// @author: saito ko
// @explanation
// 当たり判定とそのリアクションを行うクラス
//----------------------------------------------------------------------
#include "../../Header/Manager/CollisionManager.h"
#include "../../AppFrame/source/Application/UtilMacro.h"
#include "../../AppFrame/source/System/Header/Collision/3DCollision.h"
#include "../../Header/Object/Stage/Ball.h"
#include "../../Header/Object/Stage/Wall.h"
#include "../../Header/Object/Player/Player.h"
#include "../../Header/Object/Stage/Goal.h"
#include "../AppFrame/source/Mode/ModeServer.h"
#include "../../Header/Mode/ModeGame.h"
#include "../../Header/Mode/ModeGoal.h"
#include "../AppFrame/MemoryLeak.h"
#include "../AppFrame/source/Application/Global.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
CollisionManager::CollisionManager(){

};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
CollisionManager::~CollisionManager(){
	_addCollisionList.clear();
	_delCollisionNameList.clear();
	_delCollisionList.clear();
	_collisionList.clear();
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool CollisionManager::Terminate() {
	return true;
}
//----------------------------------------------------------------------
// @brief インスタンスの追加
// @param 追加するインスタンス
// @return 無し
//----------------------------------------------------------------------
void CollisionManager::AddInput(void* value){
	CollisionBase* collision = static_cast<CollisionBase*>(value);
	_addCollisionList.emplace_back(collision);
};
//----------------------------------------------------------------------
// @brief 削除するインスタンスの名前を挿入
// @param 削除したいインスタンスの名前
// @return 無し
//----------------------------------------------------------------------
void  CollisionManager::DeleteName(std::string name){
	_delCollisionNameList.emplace_back(name);
};
//----------------------------------------------------------------------
// @brief インスタンスの削除
// @param 削除するインスタンス
// @return 無し
//----------------------------------------------------------------------
void CollisionManager::DeleteInput(void* value){
	CollisionBase* collision = static_cast<CollisionBase*>(value);
	_delCollisionList.emplace_back(collision);
};
//----------------------------------------------------------------------
// @brief 格納しているインスタンスをすべて削除
// @return 無し
//----------------------------------------------------------------------
void CollisionManager::DelAll(){
	for(auto&& list : _addCollisionList){
		delete list;
	}
	for (auto&& list : _collisionList) {
		delete list;
	}
	_collisionList.clear();
	_addCollisionList.clear();
};
//----------------------------------------------------------------------
// @brief 追加していたインスタンスの削除と追加
// @return 成功したか
//----------------------------------------------------------------------
bool CollisionManager::UpdateInit(){
	// deleteListの中に値があるとき削除
	for (auto list : _delCollisionList) {
		for (auto itr = _collisionList.begin(); itr != _collisionList.end();) {
			if ((*itr) == list) {
				delete (*itr);
				itr = _collisionList.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

	for (auto list : _delCollisionNameList) {
		for (auto itr = _collisionList.begin(); itr != _collisionList.end();) {
			if ((*itr)->GetName() == list) {
				delete (*itr);
				itr = _collisionList.erase(itr);
			}
			else{
				++itr;
			}
		}
	}

	// addListの中に値があるとき追加
	for (auto list : _addCollisionList) {
		_collisionList.emplace_back(list);
	}

	// addListとdeleteListをクリア
	_addCollisionList.clear();
	_delCollisionNameList.clear();
	_delCollisionList.clear();
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CollisionManager::Update(){
	
	UpdateInit();

	// コリジョン判定を取る前の初期化
	for (auto&& first : _collisionList) {
		// 前回の判定結果を保存
		first->isHitOld = first->isHit;
		// 判定結果を初期化
		first->isHit = false;
	}

	// 衝突判定
	for(auto&& first :_collisionList){
		std::vector<std::string> name = { "shoot", "goal","wall","GoalNet"};
		auto itr = std::find_if(name.begin(), name.end(),[=](std::string temp){
			return temp == first->GetName();
		});

		if (itr != name.end()) {
			// nameに書いてあるものはballのみと判定を行うためballのほうで処理をする
			continue;
		}
		
		if(first->GetName() == "player"){
			//プレイヤーの処理
			CollisionCheckForPlayer(first);
		}
		else if(first->GetName() == "ball"){
			//ボールの処理
			CollisionCheckForBall(first);
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CollisionManager::Draw(){
#ifdef _DEBUG
	for (auto&& list : _collisionList) {
		list->Render(0xff0000);
	}
#endif
	return true;
};
//----------------------------------------------------------------------
// @brief プレイヤーとの当たり判定
// @param 当たり判定をとるプレイヤーのインスタンス
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CollisionManager::CollisionCheckForPlayer(CollisionBase* first){

	Capsule* capsule1 = dynamic_cast<Capsule*>(first);

	// キャスト失敗
	if(!capsule1){
		DebugErrar();
		return false;
	}

	for (auto&& second : _collisionList) {

		if (first == second)continue;
		if (second->GetName() == "shoot" || second->GetName() == "goal")continue;

		if(second->GetName() == "player"){
			// 同じ名前のオブジェクト同士は判定を行わない

			Capsule* capsule2 = dynamic_cast<Capsule*>(second);
			// キャスト失敗
			if (!capsule2) {
				DebugErrar();
				return false;
			}

			if (Collision3D::TwoCapsuleCol((*capsule1), (*capsule2))) {
				//衝突処理
				Player* player1 = dynamic_cast<Player*>(first->GetObje());
				if (!player1) {
					DebugErrar();
				}
				Player* player2 = dynamic_cast<Player*>(second->GetObje());
				if (!player2) {
					DebugErrar();
				}
				first->isHit = second->isHit = true;
				if (player1->GetDash() > player2->GetDash()) {
					// player2を吹き飛ばす
					Vector3D dirVec = capsule2->up_pos - capsule1->pos;
					if (!player2->GetIsKnockBack()) {
						global._soundServer->DirectPlay("SE_KnockBack");
						player2->SetKnockBack(150, dirVec);
					}
				}
			}
		}
		else if (second->GetName() == "ball") {
			Sphere* sphere = dynamic_cast<Sphere*>(second);
			// キャスト失敗
			if (!sphere) {
				DebugErrar();
				return false;
			}

			if (Collision3D::SphereCapsuleCol((*sphere),(*capsule1))) {
				//衝突処理		
				Ball* ball = dynamic_cast<Ball*>(second->GetObje());
				if (!ball) {
					DebugErrar();
				}
				bool IsShoot = ball->GetIsShoot();
				Vector3D pos1 = capsule1->pos;
				Vector3D pos2 = sphere->pos;
				Player* player = dynamic_cast<Player*>(first->GetObje());
				if (!player) {
					DebugErrar();
				}
				first->isHit = second->isHit = true;
				if (IsShoot) {
					// player1を吹き飛ばす
					Vector3D dirVec = pos1 - pos2;
					if (!player->GetIsKnockBack()) {
						global._soundServer->DirectPlay("SE_KnockBack");
						player->SetKnockBack(250, dirVec);
					}
				}
				else {
					// 球を飛ばす
					Vector3D dirVec = pos2 - pos1;
					ball->SetForwardVec(dirVec);

					Vector3D forwardVec = player->GetForwardVec();
					Vector3D ballVec = ball->GetPos() - player->GetPos();
					float angle = Math::CalcVectorAngle(forwardVec, ballVec.Normalize());
					// 正面からぶつかったときだけスピードを上げる
					if (angle < Math::DegToRad(90)) {
						global._soundServer->DirectPlay("SE_Dribble");
						ball->SetSpeed(130);
					}
				}
			}
		}
	}

	// 壁の判定 壁は平面の当たり判定で考える
	Player* player = dynamic_cast<Player*>(first->GetObje());
	if (!player) {
		DebugErrar();
	}
	auto PositionLimit = [](float& pos, int max, int min) {
		if (pos < min) pos = min;
		else if (pos > max) pos = max;
	};
	Vector3D pos = capsule1->pos;
	PositionLimit(pos.x, 6335, -6335);
	PositionLimit(pos.z, 4876, -3180);
	player->SetPos(pos);

	return true;
};
//----------------------------------------------------------------------
// @brief ボールの当たり判定
// @param 当たり判定をとるボールのインスタンス
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CollisionManager::CollisionCheckForBall(CollisionBase* first){

	Sphere* sphere1 = dynamic_cast<Sphere*>(first);

	// キャスト失敗
	if (!sphere1) {
		DebugErrar();
		return false;
	}
	bool isHitGoalNet = false;
	for (auto&& second : _collisionList) {
		// 同じオブジェクト同士は判定を行わない
		if (first == second)continue;

		if (second->GetName() == "player") {
			first->isHit = second->isHit = true;
		}
		else if (second->GetName() == "shoot") {
			Sphere* sphere2 = dynamic_cast<Sphere*>(second);
			if (!sphere2) {
				DebugErrar();
				return false;
			}
			if (Collision3D::SphereCol((*sphere1), (*sphere2))) {
				Vector3D pos1 = sphere1->pos;
				Vector3D pos2 = sphere2->pos;
				Vector3D dirVec =  pos1 - pos2;
				Ball* ball = dynamic_cast<Ball*>(first->GetObje());
				if (!ball) {
					DebugErrar();
					return false;
				}
				Player* player = dynamic_cast<Player*>(second->GetObje());
				if (!player) {
					DebugErrar();
					return false;
				}
				dirVec = player->GetForwardVec();
				first->isHit = second->isHit = true;
				global._soundServer->DirectPlay("SE_Shoot");
				//球の進む方向をプレイヤーが向いているほうに設定
				ball->SetForwardVec(dirVec);
				int power = player->GetPower();
				if (!ball->GetIsShoot()) {
					//シュート処理
					ball->SetSpeed(10 * power);
				}
				else {
					//シュートを打ち返した場合、速度の半分を追加
					ball->AddSpeed(10 * (power/2));
				}
			}
		}
		else if (second->GetName() == "goal") {
			OBB* obb = dynamic_cast<OBB*>(second);
			if (!obb) {
				DebugErrar();
				return false;
			}
			if (Collision3D::OBBSphereCol((*obb), (*sphere1) )) {
				first->isHit = second->isHit = true;
				if(second->isHitOld)continue;
				Ball* ball = dynamic_cast<Ball*>(first->GetObje());
				if (!ball) {
					DebugErrar();
					return false;
				}
				Goal* goal = dynamic_cast<Goal*>(second->GetObje());
				if (!goal) {
					DebugErrar();
					return false;
				}
				//ゴールに入った場合ModeGoal追加　あった場合は作成しない
				ModeServer* modeServer = ModeServer::GetInstance();
				if (!modeServer->Search("ModeGoal")){
					global._soundServer->DirectPlay("SE_Whistle");
					global._soundServer->DirectPlay("SE_Cheers");
					modeServer->Add(NEW ModeGoal(goal->GetName()),1000,"ModeGoal");
				}
			}		
		}
		else if(second->GetName() == "ball"){
			Sphere* sphere2 = dynamic_cast<Sphere*>(second);
			if (!sphere2) {
				DebugErrar();
				return false;
			}
			if (Collision3D::SphereCol((*sphere1), (*sphere2))) {
				Ball* ball1 = dynamic_cast<Ball*>(first->GetObje());
				if (!ball1) {
					DebugErrar();
					return false;
				}
				Ball* ball2 = dynamic_cast<Ball*>(second->GetObje());
				if (!ball2) {
					DebugErrar();
					return false;
				}
				// ボール同士の衝突時の処理
				first->isHit = second->isHit = true;
				Vector3D pos1 = ball1->GetPos();
				Vector3D pos2 = ball2->GetPos();
				Vector3D dirVec = pos2 - pos1;
				Vector3D forwardVec1 = ball1->GetForwardVec();
				float len = dirVec.Len();
				float speed1 = ball1->GetSpeed();
				// 当たりに行った方を反射させ速度を半分にする
				ball1->SetForwardVec(Reflect(forwardVec1, dirVec.Normalize()));
				ball1->SetSpeed(speed1 / 2.0f);
				//　ぶつけられたほうは直進し速度を半分もらう
				ball2->SetForwardVec(dirVec);
				ball2->AddSpeed(speed1 / 2.0f);
				ball2->SetPos(ball2->GetPos() + dirVec.Normalize() * ((sphere1->r + sphere2->r) - len));
			}
		}
		else if(second->GetName() == "wall"){
			//ゴールネットに触れていた場合は処理しない
			if(isHitGoalNet)continue;

			OBB* obb = dynamic_cast<OBB*>(second);
			if (!obb) {
				DebugErrar();
				return false;
			}
			Vector3D hitPos;
			if (Collision3D::OBBSphereCol((*obb), (*sphere1),&hitPos)) {
				Ball* ball = dynamic_cast<Ball*>(first->GetObje());
				if (!ball) {
					DebugErrar();
					return false;
				}
				Wall* wall = dynamic_cast<Wall*>(second->GetObje());
				first->isHit = second->isHit = true;
				if (!wall) {
					DebugErrar();
					return false;
				}
				global._soundServer->DirectPlay("SE_Bound");
				//押し出し処理
				Vector3D correctionVector = ball->GetPos() - hitPos;
				ball->SetPos(ball->GetPos() + correctionVector.Normalize() * (sphere1->r - correctionVector.Len()));
				// 反射処理
				ball->SetForwardVec(Reflect(ball->GetForwardVec(), correctionVector.Normalize()));
				// 反射時スピード加算
				ball->AddSpeed(10);
			}
		}
		else if (second->GetName() == "GoalNet") {
			OBB* obb = dynamic_cast<OBB*>(second);
			Vector3D hitPos;
			if (Collision3D::OBBSphereCol((*obb), (*sphere1), &hitPos)) {
				Ball* ball = dynamic_cast<Ball*>(first->GetObje());
				if (!ball) {
					DebugErrar();
					return false;
				}
				Wall* wall = dynamic_cast<Wall*>(second->GetObje());
				first->isHit = second->isHit = true;
				if (!wall) {
					DebugErrar();
					return false;
				}
				//前フレームの位置に設定
				ball->SetPosToOldPos();
				ball->AddSpeed(2); // ゴールネットに当たったとき回転しながら止まってほしいのでスピード少し加算する
				isHitGoalNet = true;
			}
		}
	}
	// 超えてはいけない領域を超えたときの処理
	Vector3D pos1 = sphere1->pos;
	Vector3D normal = Vector3D(0, 0, 0);

	if (pos1.z < -3180) {
		normal = Vector3D(0, 0, 1);
	}
	else if (pos1.z > 4876) {
		normal = Vector3D(0, 0, -1);
	}
	else if (pos1.x < -7500) {
		normal = Vector3D(1, 0, 0);
	}
	else if (pos1.x > 7500) {
		normal = Vector3D(-1, 0, 0);
	}

	// 壁に当たったときの処理
	if(normal.Sqlen() > 0){
		Ball* ball = dynamic_cast<Ball*>(first->GetObje());
		ball->SetForwardVec(Reflect(ball->GetForwardVec(), normal));
		ball->SetPos(pos1);
		ball->AddSpeed(10);
	}

	// 超えてはいけない座標を超えたときは押し戻す
	auto PositionLimit = [](float& pos, int max, int min) {
		if (pos < min) pos = min;
		else if (pos > max) pos = max;
	};
	PositionLimit(pos1.x, 7500, -7500);
	PositionLimit(pos1.z, 4876, -3180);


	return true;
};