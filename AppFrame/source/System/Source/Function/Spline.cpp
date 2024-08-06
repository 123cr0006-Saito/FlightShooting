#include "../../Header/Function/Spline.h"
#include "../AppFrame/source/Application/UtilMacro.h"
#include "../../Header/Function/mymath.h"
Vector3D Spline::LinearSpline(const std::vector<Vector3D>& point, double time){
#ifdef _DEBUG
	if (point.size() < 2) {
		// �����ɕK�v�ȓ_������Ȃ�
		DebugErrar();
		return Vector3D();
	}
#endif

	// �ǂ̃Z�O�����g�ɑ����邩������
	double t = Math::Clamp(0.0f, 1.0f, time) * (point.size() - 1);
	int segment = static_cast<int>(t);
	t -= segment;

	Vector3D p0 = point[segment];
	Vector3D p1 = point[segment + 1];

	// ���� : p(t) = (1 - t) * p0 + t * p1
	Vector3D result = (1 - t) * p0 + t * p1;

	return result;
};

Vector3D Spline::QuadraticSpline(const std::vector<Vector3D>& point, double time){
#ifdef _DEBUG
	if (point.size() < 3) {
		// �����ɕK�v�ȓ_������Ȃ�
		DebugErrar();
		return Vector3D();
	}
#endif

	// �ǂ̃Z�O�����g�ɑ����邩������
	double t = Math::Clamp(0.0f, 1.0f, time) * (point.size() - 2);
	int segment = static_cast<int>(t);
	t -= segment;
	

	Vector3D p0 = point[segment];
	Vector3D p1 = point[segment + 1];
	Vector3D p2 = point[segment + 2];

	// ���� : p(t) = (1 - t)^2 * p0 + 2(1 - t)t * p1 + t^2 * p2

	Vector3D result = (1 - t) * (1 - t) * p0 + 2 * (1 - t) * t * p1 + t * t * p2;

	return result;
};

Vector3D Spline::CubicSpline(const std::vector<Vector3D>& point, double time){
#ifdef _DEBUG
	if (point.size() < 4) {
		// �����ɕK�v�ȓ_������Ȃ�
		DebugErrar();
		return Vector3D();
	}
#endif

	// �ǂ̃Z�O�����g�ɑ����邩������
	double t = Math::Clamp(0.0f, 1.0f, time) * (point.size() - 3);
	int segment = static_cast<int>(t);
	t -= segment;

	Vector3D p0 = point[segment];
	Vector3D p1 = point[segment + 1];
	Vector3D p2 = point[segment + 2];
	Vector3D p3 = point[segment + 3];

	// ���� : p(t) = (1 - t)^3 * p0 + 3 * (1 - t)^2 * t * p1 + 3 * (1 - t)t^2 * p2 + t^3 * p3
	double t2 = t * t;
	double t3 = t * t * t;

	Vector3D result = (1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * p1 + 3 * (1 - t) * t2 * p2 + t3 * p3;

	return result;
};

double Spline::BSplineBase(int i, int k, double t, const std::vector<double>& knots){
	if (k == 1) {
		return (knots[i] <= t && t < knots[i + 1]) ? 1.0 : 0.0;
	}
	else {
		double coef1 = (knots[i + k - 1] == knots[i]) ? 0.0 : (t - knots[i]) / (knots[i + k - 1] - knots[i]);
		double coef2 = (knots[i + k] == knots[i + 1]) ? 0.0 : (knots[i + k] - t) / (knots[i + k] - knots[i + 1]);

		return coef1 * BSplineBase(i, k - 1, t, knots) + coef2 * BSplineBase(i + 1, k - 1, t, knots);
	}
};

Vector3D Spline::BSpline(const std::vector<Vector3D>& point, int k, double time){
#ifdef _DEBUG
	if (point.size() < k) {
		// �����ɕK�v�ȓ_������Ȃ�
		DebugErrar();
		return Vector3D();
	}
#endif

	std::vector<double> knots(point.size() + k);
	int n = point.size() - 1;

	for (int i = 0; i <= n + k; ++i) {
		if (i < k) {
			knots[i] = 0.0;
		}
		else if (i <= n) {
			knots[i] = static_cast<double>(i - k + 1) / (n - k + 2);
		}
		else {
			knots[i] = 1.0;
		}
	}

	double t = Math::Max(0.0, Math::Min(1.0, time)) * (n - k + 2);

	Vector3D result;
	for (int i = 0; i <= n; ++i) {
		double b = BSplineBase(i, k, t, knots);
		result += b * point[i];
	}

	return result;
};

Vector3D Spline::CatmullRomSpline(const std::vector<Vector3D>& point, double time){
#ifdef _DEBUG
	if (point.size() < 4) {
		// �����ɕK�v�ȓ_������Ȃ�
		DebugErrar();
		return Vector3D();
	}
#endif

	// �ǂ̃Z�O�����g�ɑ����邩������
	double t = Math::Clamp(0.0f, 1.0f, time) * (point.size() - 3);
	int segment = static_cast<int>(t);
	t -= segment;

	Vector3D p0 = point[segment];
	Vector3D p1 = point[segment + 1];
	Vector3D p2 = point[segment + 2];
	Vector3D p3 = point[segment + 3];

	//f(1) = -0.5t^3 + t^2 - 0.5t        �_P0�̉e������B�n�_�̐ڐ��ɉe����^����
	//f(2) = 1.5t^3 - 2.5t^2 + 1.0     �_P1�̉e������B�n�_�ƏI�_�̐ڐ��ɉe����^���� t=0�̎�P1��ʂ�
	//f(3) = -1.5t^3 + 2.0t^2 + 0.5t  �_P2�̉e������B�I�_�Ǝn�_�̐ڐ��ɉe����^���� t=1�̎�P2��ʂ�
	//f(4) = 0.5t^3 - 0.5t^2              �_P3�̉e������B�I�_�̐ڐ��ɉe����^����

	// ���� : p(t) = 0.5 * ((2 * p1) + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * t^2 + (-p0 + 3 * p1 - 3 * p2 + p3) * t^3)
	double t2 = t * t;
	double t3 = t * t * t;

	Vector3D result = 0.5f * ((2 * p1) + (p0 * -1 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * t2 + (p0 * -1 + 3 * p1 - 3 * p2 + p3) * t3);

	return result;
};