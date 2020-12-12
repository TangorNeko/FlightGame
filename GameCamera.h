#pragma once
#include "Player.h"
#include "tkEngine/camera/tkSpringCamera.h"

class GameCamera : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	Player* player = nullptr;
	CVector3 m_position;
	CVector3 m_target;
	CSpringCamera m_springCamera;
};

