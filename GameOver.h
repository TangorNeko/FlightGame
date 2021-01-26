#pragma once
class GameScene;

class GameOver : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSpriteRender* m_spriteRender = nullptr;
	GameScene* m_gameScene = nullptr;

	int m_timeCount = 0;
};

