#pragma once
#include "appframe.h"
class Score
{
public:
	Score();
	~Score();
	void AddScore(int score);
	int GetScore();
	static Score* GetInstance() {return _instance;}
protected:
	int _score;
	static Score* _instance;
};

