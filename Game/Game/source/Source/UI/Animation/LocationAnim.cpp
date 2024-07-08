#include "../../../Header/UI/Animation/LocationAnim.h"
#include "../AppFrame/source/CFile/CFile.h"
#include "../AppFrame/source/Application/UtilMacro.h"
#include "../AppFrame/source/System/Header/Function/Easing.h"
LocationAnim::LocationAnim(UIRotaBase* ui,std::string filePath) :
	AnimationBase(ui)
{
	Load(filePath);
};

LocationAnim::~LocationAnim(){
	_changeValueList.clear();
};

void LocationAnim::Load(std::string filePath){
	CFile file(filePath);
	// ファイルが開けた場合
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		int loopCount = 0;
		while (c < size) {
			std::string tag;
			float start,end;
			int delayTime,endTime;
			c += GetString(&p[c], &tag);
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &start); //始まりの値を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetFloatNum(&p[c], &end); //最終的な値の取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &endTime); //終了時間を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &delayTime); //遅延時間を取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする

			void (LocationAnim::* func)(float, float, int, int) ;
			if (tag == "x") {
				func = &LocationAnim::ChangeX;
			}
			else if (tag == "y") {
				func = &LocationAnim::ChangeY;
			}
		    else if (tag == "angle") {
				func = &LocationAnim::ChangeAngle;
			}
			else if (tag == "alpha") {
				func = &LocationAnim::ChangeAlpha;
			}
			else if (tag == "extX") {
				func = &LocationAnim::ChangeExtrateX;
			}
			else if (tag == "extY") {
				func = &LocationAnim::ChangeExtrateY;
			}
			else {
				DebugErrar();
			}

			_changeValueList.push_back(std::make_pair(func, std::make_tuple(start, end, endTime, delayTime)));
			loopCount++;
		}
	}
	else {
		DebugErrar();
	}
};

void LocationAnim::Update(){
	for(auto&& list : _changeValueList){
		(this->*list.first)(std::get<0>(list.second), std::get<1>(list.second), std::get<2>(list.second), std::get<3>(list.second));
	}
};

void LocationAnim::ChangeX(float start, float end, int endTime, int delay){
	float x = 0;
	int nowTime = GetNowCount() - _currentTime - delay;
	Easing::CallingFunction(&x, nowTime, start, end, endTime, Easing::Linear);
	_ui->SetPosX(x);
};

void LocationAnim::ChangeY(float start, float end, int endTime, int delay){
	float y = 0;
	int nowTime = GetNowCount() - _currentTime - delay;
	Easing::CallingFunction(&y, nowTime, start, end, endTime, Easing::Linear);
	_ui->SetPosY(y);
};

void LocationAnim::ChangeAngle(float start, float end, int endTime, int delay){
	float angle = 0;
	int nowTime = GetNowCount() - _currentTime - delay;
	Easing::CallingFunction(&angle, nowTime, start, end, endTime, Easing::Linear);
	_ui->SetAngle(angle);
};

void LocationAnim::ChangeAlpha(float start, float end, int endTime, int delay){
	float alpha = 0;
	int nowTime = GetNowCount() - _currentTime - delay;
	Easing::CallingFunction(&alpha, nowTime, start, end, endTime, Easing::Linear);
	_ui->SetAlpha(alpha);
};

void LocationAnim::ChangeExtrateX(float start, float end, int endTime, int delay){
	float extX = 0;
	int nowTime = GetNowCount() - _currentTime - delay;
	Easing::CallingFunction(&extX, nowTime, start, end, endTime, Easing::Linear);
	_ui->SetExtrateX(extX);
};

void LocationAnim::ChangeExtrateY(float start, float end, int endTime, int delay){
	float extY = 0;
	int nowTime = GetNowCount() - _currentTime - delay;
	Easing::CallingFunction(&extY, nowTime, start, end, endTime, Easing::Linear);
	_ui->SetExtrateY(extY);
};