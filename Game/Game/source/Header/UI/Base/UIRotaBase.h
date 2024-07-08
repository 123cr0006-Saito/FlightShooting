#pragma once
#include "UIBase.h"
#include <vector>

class UIRotaBase : public UIBase
{
public:
	UIRotaBase();
	UIRotaBase(std::string name, Vector3D pos, Vector3D center, Vector3D extrate, float angle, int alpha, int handle,int layer);
	virtual ~UIRotaBase();
	void Update()override;
	void Draw()override;

	void SetAnimation(class AnimationBase* anim) { _animation.emplace_back(anim); }

	
	Vector3D GetCenter() { return _center; }
	Vector3D GetExtrate() { return _extrate; }
	float GetAngle() { return _angle; }

	void SetParam(std::tuple<Vector3D, Vector3D, float,float>);
	void SetExtrateX(float x) { _extrate.x = x; }
	void SetExtrateY(float y) { _extrate.y = y; }
	void SetAngle(float angle) { _angle = angle; }
	//--------------------------------------
	//ïœêî
protected:
	std::vector<class AnimationBase*> _animation;
	Vector3D _center;
	Vector3D _extrate;
	float _angle;
};