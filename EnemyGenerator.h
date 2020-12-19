#pragma once

class LaserEnemy;
class MissileEnemy;
class Player;

class EnemyGenerator : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	LaserEnemy* lEnemy = nullptr;
	MissileEnemy* mEnemy = nullptr;
	Player* player = nullptr;
	int timer = 0;
};

