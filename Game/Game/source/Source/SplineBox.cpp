#include "../Header/SplineBox.h"
SplineBox::SplineBox() : ObjectBase("Spline") {
	_pos = Vector3D(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500);
	obb = new OBB("Spline",_pos,this);
	obb->SetLength(Vector3D(1, 1, 1) * 50);
};

SplineBox::SplineBox(Vector3D pos) : ObjectBase("Spline") {
	_pos = pos;
	obb = new OBB("Spline",_pos,this);
	obb->SetLength(Vector3D(1,1,1)*50);
};

SplineBox::~SplineBox(){
	delete obb;
};

bool SplineBox::Update(){
	obb->Update(_pos);
	return true;
}