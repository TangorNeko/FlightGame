#pragma once
class GameScene : public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();
	void Update();
};

