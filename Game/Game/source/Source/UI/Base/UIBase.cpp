#include "../../../Header/UI/Base/UIBase.h"
#include "../AppFrame/source/Application/Global.h"

#if !defined(_DEBUG) || defined(FULLSCREEN)
Vector3D UIBase::_screenSize = Vector3D(1920, 1080, 0);
#else
Vector3D UIBase::_screenSize = Vector3D(1280, 720, 0);
#endif

UIBase::UIBase() :
	_name(""),
	_expansionRate(1.0f),
	_alpha(255),
	_layer(0),
	_handle(-1)
{
};

UIBase::UIBase(std::string name,Vector3D pos, float alpha, int handle, int layer) :
	_name(name),
	_pos(pos),
	_alpha(alpha),
	_handle(handle),
	_layer(layer)
{

};

UIBase::UIBase(std::string name,Vector3D pos, float expansion, float alpha, int handle, int layer) :
	_name(name),
	_pos(pos),
	_expansionRate(expansion),
	_alpha(alpha),
	_handle(handle),
	_layer(layer)
{

};

UIBase::~UIBase(){

};

void UIBase::Update(){

};

void UIBase::Draw(){
	float rate = _screenSize.x / 1920;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	DrawRotaGraph(_pos.x * rate,_pos.y * rate, _expansionRate * rate,0.0f,_handle,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
};

