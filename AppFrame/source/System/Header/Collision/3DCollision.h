#pragma once
#include "MyStructure.h"
#include "../../Header/Collision/2DCollision.h"
#include"dxlib.h"
class Collision3D
{
public:

	//最短距離は大体点と点を垂直に下した線になる

	//平面上での四角同士のあたり判定
	static bool PlaneCollision(Vector3D pos1, float w1, float d1, Vector3D pos2, float w2, float d2);

	//AABBの当たり判定
	static bool AABBCollision(Vector3D pos1, float w1, float h1, float d1, Vector3D pos2, float w2, float h2, float d2);

	//平面上での円のあたり判定
	static bool CircleCollision(Vector3D pos1, float r1, Vector3D pos2, float r2);

	//円柱の当たり判定
	static bool CylindCollision(Vector3D pos1, float r1,float h1, Vector3D pos2, float r2,float h2);

	//AABBの最短距離
	static float AABBShortLength(Vector3D Box, float wide, float height, float depth, Vector3D Point);

	//OBB同士の当たり判定
	static bool OBBCollision(OBB box_1,OBB box_2);

    //点と直線の最短距離
	static POINT_LINE_SHORT PointLineShortLength(Vector3D line_start, Vector3D line_end, Vector3D point);

	//点と線分の最短距離   私的な作りは始点と終点はワールド座標で入れる
	static POINT_LINE_SHORT PointLineSegShortLength(Vector3D line_start, Vector3D line_end, Vector3D point);

	//2直線の垂直になる場所とベクトル係数
	static TWOLINE_SHORT TwoLineShortPoint(Vector3D line_1_start, Vector3D line_1_end, Vector3D line_2_start, Vector3D line_2_end);

	//2線分の垂直になる場所とベクトル係数
	static TWOLINE_SHORT TwoSegmentShortPoint(Vector3D line_1_start, Vector3D line_1_end, Vector3D line_2_start, Vector3D line_2_end);

	//球と球の当たり判定
	static bool SphereCol(Vector3D pos1, float r1, Vector3D pos2, float r2);
	static bool SphereCol(const Sphere& sphere1, const Sphere& sphere2);

	//カプセル同士の当たり判定
	static bool TwoCapsuleCol(Vector3D line_1_start, Vector3D line_1_end, float r_1, Vector3D line_2_start, Vector3D line_2_end, float r_2);
	static bool TwoCapsuleCol(const Capsule& capsule1, const Capsule& capsule2);

	// 球とカプセルの当たり判定
	static bool SphereCapsuleCol(Vector3D spherePos, float sphereR, Vector3D capsuleStartPos, Vector3D capsuleEndPos, float capsuleR, Vector3D* shortestPos = nullptr);
	static bool SphereCapsuleCol(const Sphere& sphere, const Capsule& capsule, Vector3D* shortestPos = nullptr);

	//点とＯＢＢの最接近点　OBBと点が一番近いOBBの位置を返します。
	static  Vector3D PointOBB(Vector3D point, OBB obb);
	
	//OBBと球の当たり判定
	static bool OBBSphereCol(OBB obb, Vector3D point, float r, Vector3D* hitPos = nullptr);
	static bool OBBSphereCol(const OBB& obb, const Sphere& sphere, Vector3D* hitPos = nullptr);

	//OBBとカプセルの当たり判定
	static bool OBBCapsuleCol(OBB obb, Vector3D line_start, Vector3D line_end, float r, Vector3D* hitPos = nullptr);
	//オーバーロード
	static bool OBBCapsuleCol(OBB obb, Capsule capsule, Vector3D* hitPos = nullptr);

};