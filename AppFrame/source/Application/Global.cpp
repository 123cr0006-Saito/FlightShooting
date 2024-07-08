#include "Global.h"


Global global;

Global::Global() {
	_soundServer = new SoundServer();
	exit_count = false;
	_stageNum = 1;
	_allExp = 0;
	_oldExp = 0;
	IsVibration = 1; 
};

Global::~Global() {
	delete _soundServer;
};

bool Global::Init() {
	_stageNum = 1;
	_allExp = 0;
	_oldExp = 0;
	return true;
};

bool Global::ResourceLoad() {
	SetUseASyncLoadFlag(true);
	SetUseASyncLoadFlag(false);
	return true;
};

bool Global::SoundLoad() {
	// 非同期読み込み設定
	SetUseASyncLoadFlag(true);
	//BGM
	_soundServer->Add("BGM_Title", new SoundItemBGM("Sound/BGM/TitleBGM.mp3"));
	_soundServer->Add("BGM_Game", new SoundItemBGM("Sound/BGM/GameMainBGM.mp3"));
	_soundServer->Add("BGM_Cheers", new SoundItemBGM("Sound/BGM/Cheers.mp3"));
	_soundServer->Add("BGM_Applause", new SoundItemBGM("Sound/BGM/Applause.mp3"));
	//SE	
	_soundServer->Add("SE_Bound", new SoundItemSE("Sound/SE/Bound.mp3"));
	_soundServer->Add("SE_CountDown", new SoundItemSE("Sound/SE/CountDown.mp3"));
	_soundServer->Add("SE_KnockBack", new SoundItemSE("Sound/SE/KnockBack.mp3"));
	_soundServer->Add("SE_Shoot", new SoundItemSE("Sound/SE/Shoot.mp3"));
	_soundServer->Add("SE_Whistle", new SoundItemSE("Sound/SE/Whistle.mp3"));
	_soundServer->Add("SE_GameEnd", new SoundItemSE("Sound/SE/GameEnd.mp3"));
	_soundServer->Add("SE_Cheers", new SoundItemSE("Sound/SE/Cheers.mp3"));
	_soundServer->Add("SE_Dribble", new SoundItemSE("Sound/SE/Dribble.mp3"));
	SetUseASyncLoadFlag(FALSE);
	return true;
}

void DebugErrar(const std::source_location location) {
	std::string errar = "ファイル名:" + std::string(location.file_name()) + "\n" + "行:" + std::to_string(location.line()) + "でエラー発生";
	MessageBox(NULL, errar.c_str(), "", MB_OK);
}