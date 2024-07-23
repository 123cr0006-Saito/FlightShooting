#pragma once
#include "../Function/Vector3D.h"
#include "dxlib.h"

#define MOUTH_LEFT  0
#define MOUTH_RIGHT 1
#define MOUTH_CENTER 2
#define MOUTH_MAX 3

class Mouse
{
	public:
	Mouse();
	~Mouse();
	void Update();

	int GetX() { return x; }
	int GetY() { return y; }
	int GetOldX() { return oldX; }
	int GetOldY() { return oldY; }

	bool GetKey(int index) { return _key[index]; }
	bool GetTrg(int index) { return _trg[index]; }
	bool GetRel(int index) { return _rel[index]; }
protected:
	int x,y,oldX,oldY;
	bool _key[MOUTH_MAX];
	bool _trg[MOUTH_MAX];
	bool _rel[MOUTH_MAX];
};

