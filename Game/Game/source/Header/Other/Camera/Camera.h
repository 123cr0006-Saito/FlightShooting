#pragma once
#include "../../../AppFrame/source/System/Header/Function/Vector3D.h"
#include <utility>
class Camera
{
public:
	Camera();
	~Camera();
	bool Update(Vector3D pos,Vector3D target, Vector3D up);
	void UpdateSelectAndResult();
	void UpdateGame();
	bool Draw();

	static Camera* GetInstance() { return _instance; }

	void SetIsGame(bool isGame);
	void SetPos(Vector3D pos) { _pos.first = pos; }
	void SetTarget(Vector3D target) { _pos.second = target; }
	bool SpringDamperSystem(double time, Vector3D& nowPos,Vector3D targetPos, Vector3D& speed ,float spling, float danpa);
	
private:
	static Camera* _instance;
	std::pair<Vector3D, Vector3D> _holdPos;// <カメラ位置,注視点>
	std::pair<Vector3D, Vector3D> _pos; // <カメラ位置,注視点>
	Vector3D _targetSpeed;
	Vector3D _posSpeed;
	bool _isGame;
	int _currentTime;
};

