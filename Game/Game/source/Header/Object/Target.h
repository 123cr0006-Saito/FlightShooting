#pragma once
#include "Base/ObjectBase.h"
#include "appframe.h"
enum class SplineMoveType : int {
	BSpline,
	Catmull
};
class Target : public ObjectBase
{
public:
	Target(float Time, float speed, int handle ,std::string type , std::vector<Vector3D>* point);
	virtual ~Target();
	virtual bool Update();
	virtual bool Draw();
protected:
	float _time;
	float _speed;
	int _model;
	Vector3D _nextPos;
	std::vector<Vector3D>* _point; 
	SplineMoveType _moveType;
	Sphere* _sphere;
};

