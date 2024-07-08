#pragma once
#include "dxlib.h"
#include <math.h>
#include "Vector3D.h"
#include "Easing.h"
class Math
{
public:
	// degree¨radian 
	static float DegToRad(float deg);

	// radian¨degree
	static float RadToDeg(float rad);

	static float Max(float left, float right);

	static float Min(float left, float right);

	static float Clamp(float min_n, float max_n, float n);

	//0:x 1:y 2:z
	static Vector3D NormRotMatrix(Vector3D norm, float rotation, int type);

	static Vector3D MatrixToVector(MATRIX matrix, int index);

	static float LenSegOnSeparateAxis(Vector3D Sep, Vector3D e1, Vector3D e2, Vector3D e3 = Vector3D());

	static MATRIX MMultXYZ(float dir_x, float dir_y, float dir_z);

	static MATRIX MMultXYZToMatrix(float dir_x, float dir_y, float dir_z);


	static bool  SetMatrixToVector(Vector3D& vec_x, Vector3D& vec_y, Vector3D& vec_z, const MATRIX matrix);

    //‰sŠp‚©‚Ç‚¤‚©
	static bool CheckAcuteAngle(Vector3D p1, Vector3D p2, Vector3D p3);

	//•½s‚©‚Ç‚¤‚©
	static bool CheckParallelRelation(Vector3D line_1_start, Vector3D line_1_end, Vector3D line_2_start, Vector3D line_2_end);

	//‚’¼‚©‚Ç‚¤‚©
	static bool CheckVerticalRelation(Vector3D line_1_start, Vector3D line_1_end, Vector3D line_2_start, Vector3D line_2_end);

	// 2‚Â‚ÌƒxƒNƒgƒ‹‚Ì‚È‚·Šp‚ğŒvZ‚·‚é
	// –ß‚è’l: Šp“x(radian) 
	static float CalcVectorAngle(Vector3D v1, Vector3D v2);

	// 3Dƒ‚ƒfƒ‹‚ğˆø”vForward‚Ì•ûŒü‚ª³–Ê‚É‚È‚é‚æ‚¤‚É‰ñ“]‚³‚¹‚é
	static void SetModelForward_RotationY(int modelHandle, Vector3D vForward);
};