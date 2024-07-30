#pragma once
#include "Base/UIBase.h"
#include <array>
class UITimer : public UIBase
{
public:
	UITimer();
	~UITimer();
	void Update() override;
	void Draw() override;
protected:
	std::array<int,10> _timeHandle;
	int _colonHandle;
	Vector3D _numPos;
	class TimeLimit* _timeLimit;
};

