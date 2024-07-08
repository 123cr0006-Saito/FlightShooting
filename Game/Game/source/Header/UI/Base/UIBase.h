#pragma once
#include "../AppFrame/source/System/Header/Function/Vector3D.h"
#include <string>
class UIBase
{
public:
	UIBase();
	UIBase(std::string name ,Vector3D, float, int,int);
	UIBase(std::string name ,Vector3D,float,float,int,int);
	virtual ~UIBase();
	virtual void Update();
	virtual void Draw();

	Vector3D GetPos() { return _pos; }
	std::string GetName() { return _name; }
	float GetAlpha() { return _alpha; }
	int GetLayer() { return _layer; }
	void SetHandle(int handle) { _handle = handle; }
	void SetPos(Vector3D pos) { _pos = pos; }
	void SetAlpha(float alpha) { _alpha = alpha; }
	void SetPosX(float x) { _pos.x = x; }
	void SetPosY(float y) { _pos.y = y; }
protected:
	static Vector3D _screenSize;
	std::string _name;
	int _handle;
	int _layer;
	Vector3D _pos;
	float _alpha;
private:
	float _expansionRate;
};