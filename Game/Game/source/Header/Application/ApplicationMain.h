#pragma once
#include "appframe.h"
#include "../Manager/SuperManager.h"
#include "../Manager/RenderManager.h"
#include "../../../AppFrame/source/System/Header/Function/Timer.h"
class ApplicationMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }

protected:
	Timer* _timer;
	Fps* _fpsController;
	class SuperManager* _superManager;
}; 
