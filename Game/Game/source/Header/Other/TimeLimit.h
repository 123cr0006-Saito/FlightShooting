//----------------------------------------------------------------------
// @filename TimeLimit.h
// @author: saito ko
// @explanation
// 制限時間の管理を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include <math.h>
#include <string>
class TimeLimit
{
public:
	TimeLimit();
	~TimeLimit();
	void SetTimeLimit(const int minutes,const int second);
	
	float Update();
	int SecondsToTime();
	int GetElapsedSecond();
	void Stop();
	void Restart();
	
	static TimeLimit* GetInstance() { return _instance; }

	float GetRemainingTime() const { return _remainingTime; }
	int GetTimeLimit() const { return _timeLimitSecond > 0 ? _timeLimitSecond : 0; }
	int GetElapsedTime() const { return _elapsedTime; }

	int GetDigits();

	int GetStartTime() const { return _setTime / 1000; }

protected:
	static TimeLimit* _instance;// このクラスのインスタンス
	float _startTime,_timeLimit;// 開始時間と制限時間
	float _setTime;// 設定時間
	float _remainingTime;// 残り時間
	int _stopTime;// ストップした時間
	bool _IsStop;// 時間を止めるかどうか

	int _timeLimitSecond; // 制限時間
	int _elapsedTime; // 経過時間
};

