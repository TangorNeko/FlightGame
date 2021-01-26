#pragma once
class Player;
class GameScene;

class Space : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSky* space = nullptr;
	Player* player = nullptr;
	GameScene* gameScene = nullptr;
	CVector3 m_position;
};

