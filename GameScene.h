#pragma once
class GameScene : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();
};

