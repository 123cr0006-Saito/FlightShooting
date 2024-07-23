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
	// 4点間を指定して2点間の補間を行う
	static Vector3D CatmullRomSpline(const std::vector<Vector3D>& point, double time);
private:
	static double BSplineBase(int i, int k, double t, const std::vector<double>& knots);
};

