#pragma once
class Blackhole;
class Player;

class GameScene : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	Blackhole* blackhole = nullptr;
	Player* player = nullptr;
};

