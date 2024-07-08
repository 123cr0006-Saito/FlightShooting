#pragma once
#include "AnimationBase.h"
#include "../AppFrame/source/System/Header/Function/Vector3D.h"
class IterationAnim : public AnimationBase
{
public:
	IterationAnim(UIRotaBase*,std::string,bool isRandom = false);
	virtual ~IterationAnim();
	void Update()override;
	void Load(std::string)override;
protected:
	Vector3D _originPos;
	Vector3D _movePos;
	int _cycleTime;
};