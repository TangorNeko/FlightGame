#pragma once

class Ring;
class Player;

class RingGenerator : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	Ring* ring = nullptr;
	Player* player = nullptr;
	int timer = 0;
};

