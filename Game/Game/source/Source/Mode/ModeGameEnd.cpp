#define _CRT_SECURE_NO_WARNINGS
#include "../../Header/Mode/ModeGameEnd.h"
#include "../../Header/Other/Score.h"
#include "../../Header/Mode/ModeTitle.h"
#include <algorithm>
bool ModeGameEnd::Initialize(){
	_score = Score::GetInstance();
	_fontHandle = CreateFontToHandle("メイリオ", 128, 3, DX_FONTTYPE_EDGE);
	_input = XInput::GetInstance();
	LoadScore("ranking.csv");
	return true;
};

bool ModeGameEnd::Terminate(){
	SaveScore();
	delete _score;
	return true;
};

bool ModeGameEnd::LoadScore(std::string name) {
	// csvファイルを読み込む
	CFile file(name);
	// ファイルが開けた場合
	if (file.Success()) {
		int loopCount = 1;
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			int score = 0;
			c += GetDecNum(&p[c], &score); //スコアを取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			_scoreData.push_back(score);
		}
	}
	else {
#ifdef _DEBUG
		DebugErrar();
#endif
	}
	return true;
};

bool ModeGameEnd::SaveScore(){
	FILE* file = fopen("ranking.csv","w");
	if (file == nullptr) {
		return false;
	}
	_scoreData.push_back(_score->GetScore());
	std::sort(_scoreData.begin(), _scoreData.end(), std::greater<int>());
	int loopCount = 0;
	for (auto&& list : _scoreData) {
		fprintf(file, "%d\n", list);
		loopCount++;
		if(loopCount >= 3){
			break;
		}
	}
	fclose(file);
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
	int loopCount = 1;
	for(auto&& list : _scoreData){
		DrawFormatStringToHandle(0, 200 + 128*loopCount, GetColor(255, 255, 255), _fontHandle, "%d位：%d", loopCount,list);
		loopCount++;
	}
	return true;
};
