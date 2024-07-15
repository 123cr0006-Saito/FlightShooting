#pragma once
#include "Vector3D.h"
class Quaternion
{

public:
	Quaternion();
	Quaternion(float w, float x, float y, float z);
	void identity(){ w = 1.0f; x = y = z = 0.0f;};
	void	SetToRotateX(float theta);
	void	SetToRotateY(float theta);
	void	SetToRotateZ(float theta);
	void	SetToRotateAxis(const Vector3D& axis, float theta);

	void	Normalize();

	float	GetToRotationAngle() const;
	Vector3D	GetToRotationAxis() const;

	float Dot(const Quaternion& q);

	void	SetRotateObjectToInertial(const Vector3D& orientation);
	void	SetRotateInertialToObject(const Vector3D& orientation);

	Quaternion operator *(const Quaternion& q) const;

	Quaternion& operator *=(const Quaternion& q);

	Vector3D byEuler();

public:
	float w, x, y, z;
};

extern inline float Dot(const Quaternion& left, const Quaternion& right);

extern inline Quaternion Conjugate(const Quaternion& q);

extern inline Quaternion Pow(const Quaternion& q, float exponent);

extern inline Vector3D RotateVectorByQuaternion(const Vector3D& v, const Quaternion& q);

extern inline Quaternion Slerp(const Quaternion& p, const Quaternion& q, float t);