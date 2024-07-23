#pragma once
#include <vector>
#include "Vector3D.h"
class Spline
{
public:
	static Vector3D LinearSpline(const std::vector<Vector3D>& point , double time);
	static Vector3D QuadraticSpline(const std::vector<Vector3D>& point, double time);
	static Vector3D CubicSpline(const std::vector<Vector3D>& point, double time);
	static Vector3D BSpline(const std::vector<Vector3D>& point, int k, double time);
	// 4�_�Ԃ��w�肵��2�_�Ԃ̕�Ԃ��s��
	static Vector3D CatmullRomSpline(const std::vector<Vector3D>& point, double time);
private:
	static double BSplineBase(int i, int k, double t, const std::vector<double>& knots);
};

