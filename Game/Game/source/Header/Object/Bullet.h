#pragma once
#include "Base/ObjectBase.h"
#include "appframe.h"
class Bullet : public ObjectBase {
public :
	Bullet(const Vector3D& pos,const Vector3D& forward);
	~Bullet();
	bool Update()override;
	bool Draw()override;
protected:
	bool _isAlive;
	int _currentTime;
	Sphere* _sphere;
};

