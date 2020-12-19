#pragma once
class Blackhole;
class Player;
class EnemyGenerator;
class Mission;

class GameScene : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	Blackhole* blackhole = nullptr;
	Player* player = nullptr;
	EnemyGenerator* enemygenerator = nullptr;
	Mission* m_mission = nullptr;

	bool m_isOnMission;
	int m_gameTimer = 0;
};

