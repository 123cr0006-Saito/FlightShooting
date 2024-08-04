#include "../../Header/UI/UITimer.h"
#include "../AppFrame/source/System/Header/Resource/ResourceServer.h"
#include "../../Header/Other/TimeLimit.h"
UITimer::UITimer() : UIBase(){
	_colonHandle = LoadGraph("Res/UITime/UI_Time_Colon.png");
	for(int i = 1; i <= 10; i++){
		_timeHandle[i-1] = LoadGraph(("Res/UITime/Ui_Time (" + std::to_string(i) + ").png").c_str());
	}

	_name = "UITimer";
	_pos = Vector3D(1700, 150, 0);
	_numPos = Vector3D(1700 + 60, 130, 0);
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

	int x = (int)_numPos.x;
	for(int i = 0; i < 4; i++){

		int handleX, handleY;
		if (loopCount == 2) {
			// ƒRƒƒ“‚ð•`‰æ
			GetGraphSize(_colonHandle, &handleX, &handleY);
			DrawGraph(x + handleX , _numPos.y , _colonHandle, true);
			x -= handleX + 20;// ”Žš‚ÌŠÔŠu
		}

		// •b”‚ð•`‰æ
		int num = time % 10;
		DrawGraph(x, _numPos.y, _timeHandle[num], true);
		time /= 10;

		GetGraphSize(_timeHandle[num], &handleX, &handleY);

		x -= handleX + 10;// ”Žš‚ÌŠÔŠu
		loopCount++;// ‰½Œ…–Ú‚©”‚¦‚é

		

	}
};
