#pragma once
#include "Base/UIBase.h"
#include <array>
class UIScore : public UIBase
{
public:
	UIScore();
	~UIScore();
	void Update() override;
	void Draw() override;
protected:
	std::array<int, 10> _timeHandle; 
	Vector3D _numPos;
};

