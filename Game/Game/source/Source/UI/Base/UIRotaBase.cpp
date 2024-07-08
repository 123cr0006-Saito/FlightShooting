#include "../../../Header/UI/Base/UIRotaBase.h"
#include "../../../Header/UI/Animation/AnimationBase.h"

UIRotaBase::UIRotaBase() :
	UIBase(),
	_extrate(Vector3D(1, 1, 1)),
	_angle(0)
{
};

UIRotaBase::UIRotaBase(std::string name,Vector3D pos,Vector3D center, Vector3D extrate,float angle, int alpha, int handle,int layer):
UIBase(name,pos,alpha,handle,layer),
_center(center),
_extrate(extrate),
_angle(angle)
{
};

UIRotaBase::~UIRotaBase() {
	if (!_animation.empty()) {
		for (auto&& list : _animation) {
			delete list;
		}
	}
	_animation.clear();
};

void UIRotaBase::SetParam(std::tuple<Vector3D, Vector3D, float, float> param){
	_pos = std::get<0>(param);
	_extrate = std::get<1>(param);
	_angle = std::get<2>(param);
	_alpha = std::get<3>(param);
};

void UIRotaBase::Update() {
	if(!_animation.empty()){
		for(auto&& list : _animation){
			list->Update();
		}
	}
};

void UIRotaBase::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	float rate = _screenSize.x / 1920;
	DrawRotaGraph3(_pos.x * rate, _pos.y * rate, _center.x * rate, _center.y * rate, _extrate.x * rate, _extrate.y * rate, _angle, _handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}