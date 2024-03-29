#pragma once

class Ring;
class Player;
class GameScene;

class RingGenerator : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	Ring* m_ring = nullptr;
	Player* m_player = nullptr;
	GameScene* m_gameScene = nullptr;
};

