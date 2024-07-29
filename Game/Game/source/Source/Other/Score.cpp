#include "../../Header/Other/Score.h"

Score* Score::_instance = nullptr;
Score::Score() {
	if (_instance != nullptr) {
		DebugErrar();
	}
	_score = 0;
};

Score::~Score() {
	_instance = nullptr;
};

void Score::AddScore(int score) {
	_score += score;
};

int Score::GetScore() {
	return _score;
};
