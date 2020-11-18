#pragma once
#include "Player.h"

class GameCamera : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	Player* player = nullptr;
	CVector3 position;
	CVector3 target;
};

