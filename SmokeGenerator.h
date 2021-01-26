#pragma once
class Smoke;
class Player;

class SmokeGenerator : public IGameObject
{
	void OnDestroy();
	bool Start();
	void Update();

	Smoke* m_smoke;
	Player* m_player;
};

