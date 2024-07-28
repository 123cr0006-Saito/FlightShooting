//----------------------------------------------------------------------
// @filename ModeGame.h
// @date: 2023/12/14
// @author: saito ko
// @explanation
// ボスバトル以外のステージを管理するクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "dxlib.h"
#include "../../../AppFrame/source/Application/UtilMacro.h"
#include "../../AppFrame/source/System/Header/Function/Quaternion.h"

class ModeGame : public ModeBase
{
	typedef ModeBase base;

public:

	ModeGame();
	virtual bool Initialize()override;
	virtual bool Terminate()override;
	virtual bool Process()override;
	virtual bool Render()override;

	bool LoadObject(std::string name);
	std::vector<std::tuple<std::string, Vector3D, Vector3D>> LoadObjectParam(std::string fileName);
protected:
	class SuperManager* _superManager;
	int skySphere;
	std::vector<std::pair<float, std::vector<Vector3D>>> a;
	std::vector<class Target*> _target;
	class Jet* _jet;
};