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

	void SetIsAlive(bool isAlive) { _isAlive = isAlive; }
	bool GetIsAlive() { return _isAlive; }

	void MoveProcess();
	void ExplosionProcess();
protected:
	bool _isAlive;
	float _time;
	float _speed;
	int _model;
	Vector3D _nextPos;
	std::vector<Vector3D>* _point; 
	SplineMoveType _moveType;
	Sphere* _sphere;

	int _frameSize;
	std::vector<Vector3D> _rotVec;
	std::vector<Vector3D> _moveVec;
	float _alpha;
};

