#pragma once
#include "appframe.h"
class SplineBox : public ObjectBase
{
public:
	SplineBox();
	SplineBox(Vector3D pos);
	~SplineBox();
	bool Update()override;
	OBB* GetOBB() { return obb; }
protected:
	OBB* obb;
};