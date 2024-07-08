#pragma once
#include "ManagerBase.h"
// @brief プレイヤーの管理クラス
// セレクト画面で決定したキャラクターを管理するクラス　パッドの数でプレイヤーの数を設定
class PlayerManager : public ManagerBase
{
	public:
	PlayerManager();
	~PlayerManager();
	virtual bool Terminate()override;

	void Add(std::vector<std::tuple<std::string,class XInput*, int>> param);
	void DelAll();

	void InitParam();
	
	std::vector<class Player*> GetList() { return _player; }

	bool Update()override;
	bool UpdateEnd()override;

	bool Draw()override;
	
protected:
	std::vector<class Player*> _player;
	std::vector<class Vector3D> _originPos;
};

