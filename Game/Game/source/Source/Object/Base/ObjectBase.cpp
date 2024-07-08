#include "../../../Header/Object/Base/ObjectBase.h"
#include "../AppFrame/source/System/Header/Function/mymath.h"
ObjectBase::ObjectBase(std::string name){
	_name = name;
};

ObjectBase::~ObjectBase(){

};

bool ObjectBase::Init(){
	return true;
};

bool ObjectBase::UpdateInit(){
	return true;
};

bool ObjectBase::Update(){
	return true;
};

bool ObjectBase::UpdateEnd(){
	return true;
};

bool ObjectBase::DebugDraw() {
	return true;
};