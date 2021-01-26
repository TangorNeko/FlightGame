#pragma once
#include "Player.h"
#include "tkEngine/camera/tkSpringCamera.h"

class GameScene;

class GameCamera : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	GameScene* m_gameScene = nullptr;
	Player* m_player = nullptr;
	CVector3 m_position;
	CVector3 m_target;
	CSpringCamera m_springCamera;

	int m_blackholeCameraScene = 0;
	bool m_isCameraChangeFrame = false;
};

