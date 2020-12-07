#pragma once
class Player;

class Space : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSky* space = nullptr;
	Player* player = nullptr;
};

