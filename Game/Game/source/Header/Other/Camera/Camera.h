#pragma once
#include "../../../AppFrame/source/System/Header/Function/Vector3D.h"
#include <utility>
class Camera
{
public:
	Camera();
	~Camera();
	bool Update();
	void UpdateSelectAndResult();
	void UpdateGame();
	bool Draw();

	static Camera* GetInstance() { return _instance; }

	void SetIsGame(bool isGame);
	void SetPos(Vector3D pos) { _pos.first = pos; }
	void SetTarget(Vector3D target) { _pos.second = target; }
	bool SpringDamperSystem(Vector3D targetPos);

private:
	static Camera* _instance;
	std::pair<Vector3D, Vector3D> _holdPos;// <カメラ位置,注視点>
	std::pair<Vector3D, Vector3D> _pos; // <カメラ位置,注視点>
	Vector3D _speed;
	bool _isGame;
	int _currentTime;
	class PlayerManager* _player;
};

