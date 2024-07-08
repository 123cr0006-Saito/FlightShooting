#pragma once
#include "../Base/UIRotaBase.h"
class AnimationBase
{
public:
	AnimationBase(UIRotaBase*);
	~AnimationBase();
	virtual void Update()= 0;
	virtual void Load(std::string) = 0;
protected:
	UIRotaBase* _ui;
	int _currentTime;
	int _endTime;
	int _delayTime;
	bool _isEnd;
};