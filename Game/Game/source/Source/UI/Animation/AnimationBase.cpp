#include "../../../Header/UI/Animation/AnimationBase.h"
#include "dxlib.h"
AnimationBase::AnimationBase(UIRotaBase* ui):
 _ui(ui),
 _currentTime(GetNowCount()),
 _endTime(0),
 _delayTime(0),
 _isEnd(false)
{
};

AnimationBase::~AnimationBase(){
	_ui = nullptr;
};