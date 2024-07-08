#pragma once
#include "dxlib.h"
#include "../Function/Vector3D.h"
#include <vector>
#include <string>

#include "../Game/Game/source/Header/Object/Base/ObjectBase.h"

// 円周率
#define	PI				(3.141592653589793)

// RECT用構造体
struct MYRECT {
	float		x, y, w, h;
};

// RECT用構造体+アルファ
struct MYRECT_PLUS : public MYRECT{
	float cx, cy;
	float direction;
};

struct MYCIRCLE {
	float x, y;
	float r;
};

struct HITSLIDE {
	int	hit;				// 当たっていれば1、当たっていないければ0
	int	slide_x, slide_y;	// 当たっている場合、ずらすべき値
	int index;
};

struct POINT_LINE_SHORT {
	Vector3D hit_point;
	float length = 0.0f;
	float coefficient = 0.0f;//ベクトル係数
};

struct TWOLINE_SHORT {
	Vector3D line_1_point;
	Vector3D line_2_point;
	float line_1_coefficient = 0.0f;
	float line_2_coefficient = 0.0f;
	float length = 0.0f;
};

class CollisionBase
{
public:
	CollisionBase() { name = "";isHit = false; isHitOld = false; object = nullptr;};
	CollisionBase(std::string name, ObjectBase* object) { this->name = name; this->object = object; isHit = false; isHitOld = false;};
	virtual ~CollisionBase() {};
	void SetName(std::string name) { this->name = name; }
	virtual void Render(unsigned int color) = 0;
	std::string GetName() { return name; }
	ObjectBase* GetObje() { return object; }
public:
	Vector3D pos;
	bool isHit;
	bool isHitOld;
protected:
	std::string name;
	ObjectBase* object;
};

class Sphere : public CollisionBase
{
public:
	Sphere() : CollisionBase(){
		pos = VGet(0.0f, 0.0f, 0.0f);
		r = 0.0f;
	}
	Sphere(Vector3D pos, float r) : CollisionBase() {
		this->pos = pos;
		this->r = r;
	}
	Sphere(std::string name, Vector3D pos, float r, ObjectBase* object) : CollisionBase(name, object) {
		this->pos = pos;
		this->r = r;
	}

	// 描画処理
	void Render(unsigned int color)override;

	float r;
};

class OBB : public CollisionBase
{
public:
	OBB() : CollisionBase() {
		// 初期状態ではワールドの軸と平行な状態にする（AABB）
		dir_vec[0] = Vector3D(1.0f, 0.0f, 0.0f);
		dir_vec[1] = Vector3D(0.0f, 1.0f, 0.0f);
		dir_vec[2] = Vector3D(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < 3; ++i) {			
			length[i] = 0.0f;
		}
	}
	OBB(std::string name, Vector3D pos, ObjectBase* object) : CollisionBase(name, object) {
		this->pos = pos;
		// 初期状態ではワールドの軸と平行な状態にする（AABB）
		dir_vec[0] = Vector3D(1.0f, 0.0f, 0.0f);
		dir_vec[1] = Vector3D(0.0f, 1.0f, 0.0f);
		dir_vec[2] = Vector3D(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < 3; ++i) {
			length[i] = 0.0f;
		}
	}
	OBB(std::string name,Vector3D pos, Vector3D length, ObjectBase* object) : CollisionBase(name,object) {
		this->pos = pos;
		dir_vec[0] = Vector3D(1.0f, 0.0f, 0.0f);
		dir_vec[1] = Vector3D(0.0f, 1.0f, 0.0f);
		dir_vec[2] = Vector3D(0.0f, 0.0f, 1.0f);
		SetLength(length);
	}

	void SetLength(Vector3D value) {
		length[0] = value.x;
		length[1] = value.y;
		length[2] = value.z;
	}

	// 回転処理
	// x軸->y軸->z軸の順番で, 各軸を回転させる
	void Rotate(Vector3D vRot);

	// 頂点座標の取得
	void GetVertexPos(std::vector<Vector3D>& vertexPosList);

	// 描画処理
	void Render(unsigned int color)override;

	Vector3D dir_vec[3];//xv,yv,zv
	float length[3]; //0:w 1:h 3:d
};

class Capsule : public CollisionBase {
public:
	Capsule() : CollisionBase() {
		up = 0.0f;
		r = 0.0f;
		for (int i = 0; i < 3; i++) {
			direction[i] = 0;
		}
	};
	Capsule(std::string name, Vector3D pos, float up, float r, ObjectBase* object) : CollisionBase(name, object) {
		this->pos = pos;
		this->up = up;
		this->r = r;
		for (int i = 0; i < 3; i++) {
			direction[i] = 0;
		}
	};
	void Update() {
		up_pos = pos + Vector3D(0,up,0);
	};

	// 描画処理
	void Render(unsigned int color)override;

	Vector3D up_pos;
	float up = 0.0f;
	float r = 0.0f;
	float direction[3] = { 0.0f,0.0f, 0.0f };
};