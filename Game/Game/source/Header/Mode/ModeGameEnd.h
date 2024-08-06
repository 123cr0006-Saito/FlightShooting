#pragma once
#include "appframe.h"
class ModeGameEnd : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize()override;
	virtual bool Terminate()override;
	virtual bool Process()override;
	virtual bool Render()override;

	bool LoadScore(std::string name);
	bool SaveScore();
private:
	XInput* _input;
	class Score* _score;
	int _fontHandle;
	std::vector<int> _scoreData;
};

