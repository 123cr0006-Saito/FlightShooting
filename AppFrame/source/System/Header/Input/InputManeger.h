#pragma once
#include <map>
#include "XInput.h"
class InputManeger
{
	public:
	InputManeger();
	~InputManeger();
	virtual bool UpdateJoyPad();
	void MainUpdate();

protected:
	std::map<int,XInput> _inputList;
	static unsigned char _connectNum;
};

