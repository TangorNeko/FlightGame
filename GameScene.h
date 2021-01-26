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
	Mission* m_mission = nullptr;

	bool m_isPlayerDead = false;
	bool m_isGameEnd = false;
	bool m_isOnMission = false;
	bool m_isBlackholePhase = false;
	int m_gameTimer = 0;

	prefab::CFontRender* m_fontRender = nullptr;

	prefab::CSoundSource* m_gameBGM = nullptr;

	prefab::CSpriteRender* m_warningSpriteUp = nullptr;
	prefab::CSpriteRender* m_warningSpriteDown = nullptr;
	int m_warningCount = 0;
};

