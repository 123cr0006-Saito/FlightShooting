#include "../../Header/Input/Mouse.h"
Mouse::Mouse(){

};

Mouse::~Mouse(){

};

void Mouse::Update(){
	oldX = x; oldY = y;
	GetMousePoint(&x,&y);

	int input[MOUTH_MAX] = { MOUSE_INPUT_LEFT,MOUSE_INPUT_RIGHT,MOUSE_INPUT_MIDDLE };
	int now = GetMouseInput();

	bool oldkey[MOUTH_MAX] = { _key[0],_key[1],_key[2] };

	for (int i = 0; i < MOUTH_MAX; i++) {
		_key[i] = now & input[i];
		_trg[i] = (_key[i] && !oldkey[i]);
		_rel[i] = (!_key[i] && oldkey[i]);
	}
};
