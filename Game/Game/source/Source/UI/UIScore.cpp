#include "../../Header/UI/UIScore.h"
#include "../../Header/Other/Score.h"
UIScore::UIScore(){
	for (int i = 1; i <= 10; i++) {
		_timeHandle[i-1] = LoadGraph(("Res/UITime/Ui_Time (" + std::to_string(i) + ").png").c_str());
	}
	_numPos = Vector3D(1000, 50, 0);
};

UIScore::~UIScore(){

};

void UIScore::Update(){

};

void UIScore::Draw(){
	int score = Score::GetInstance()->GetScore();
	int x = (int)_numPos.x;
	do
	{
		int handleX, handleY;

		int num = score % 10;
		DrawGraph(x, _numPos.y, _timeHandle[num], true);
		score /= 10;

		GetGraphSize(_timeHandle[num], &handleX, &handleY);

		x -= handleX + 10;// ”Žš‚ÌŠÔŠu

	}
	while (score > 0);
};
