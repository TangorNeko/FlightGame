#pragma once
class Player;
class GameScoreManager;
class GameScene;

class GameClear : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void PostUpdate();

	Player* m_player = nullptr;
	GameScoreManager* m_gameScoreManager = nullptr;
	GameScene* m_gameScene = nullptr;
	prefab::CFontRender* m_fontRender = nullptr;
	std::wstring m_string;
	prefab::CSoundSource* m_scoreCountSound = nullptr;

	int m_hpBonus;
	int m_fuelBonus;
	int m_totalScore;
	int m_timeCount = 0;
};

