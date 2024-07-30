#include "../../Header/UI/UITimer.h"
#include "../AppFrame/source/System/Header/Resource/ResourceServer.h"
#include "../../Header/Other/TimeLimit.h"
UITimer::UITimer() : UIBase(){
	_colonHandle = LoadGraph("Res/UITime/Ui_Time_colon.png");
	for(int i = 0; i < 10; i++){
		_timeHandle[i] = LoadGraph(("Res/UITime/Ui_Time (" + std::to_string(i) + ").png").c_str());
	}

	_name = "UITimer";
	_pos = Vector3D(0, 0, 0);
	_handle = LoadGraph("Res/UITime/UI_TIME_Gauge.png");
	_timeLimit = TimeLimit::GetInstance();
};

UITimer::~UITimer(){

};

void UITimer::Update() {

};

void UITimer::Draw() {
	UIBase::Draw();
	int time = _timeLimit->GetTimeLimit();
	int loopCount = 0;

	int x = (int)_pos.x;
	while (1) {

		int handleX, handleY;
		if (loopCount == 2) {
			// コロンを描画
			GetGraphSize(_colonHandle, &handleX, &handleY);
			DrawGraph(x + handleX, _numPos.y + 10, _colonHandle, true);
			x -= handleX + 20;// 数字の間隔
		}

		// 秒数を描画
		int num = time % 10;
		DrawGraph(x, _numPos.y, _timeHandle[num], true);
		time /= 10;

		GetGraphSize(_timeHandle[num], &handleX, &handleY);

		x -= handleX + 10;// 数字の間隔
		loopCount++;// 何桁目か数える

		if (time == 0) {
			break;
		}

	}
};
