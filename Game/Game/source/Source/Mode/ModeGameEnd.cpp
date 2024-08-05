#include "../../Header/Mode/ModeGameEnd.h"
#include "../../Header/Other/Score.h"
#include "../../Header/Mode/ModeTitle.h"
bool ModeGameEnd::Initialize(){
	_score = Score::GetInstance();
	_fontHandle = CreateFontToHandle("メイリオ", 128, 3, DX_FONTTYPE_EDGE);
	_input = XInput::GetInstance();
	return true;
};

bool ModeGameEnd::Terminate(){
	return true;
};

bool ModeGameEnd::Process(){
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "ModeTitle");
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};

bool ModeGameEnd::Render(){
	DrawFormatStringToHandle(1920 / 2, 1080 / 2 - 200, GetColor(255, 255, 255), _fontHandle,"スコア：%d", _score->GetScore());
	DrawFormatStringToHandle(1920/2, 1080/2, GetColor(255, 255, 255), _fontHandle, "タイトルに戻る");
	return true;
};
