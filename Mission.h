#pragma once
class Player;
class GameScoreManager;

class Mission : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	GameScoreManager* m_gamescoremanager = nullptr;
	Player* m_player = nullptr;
	int m_receiveMission = 0;
	int m_missionTimer = 1200;
	float m_missionSprite = 0.1f;
	bool m_isMissionConfirm = false;
	bool m_isSelectReverseMission = false;
	bool m_isMissionEnd = false;
	prefab::CFontRender* m_fontRender = nullptr;
	std::wstring teststring;
	const wchar_t* string;

	prefab::CSpriteRender* m_missionTimeSpriteRender[5] = { nullptr ,nullptr,nullptr,nullptr,nullptr};

	prefab::CSpriteRender* m_missionSpriteRender = nullptr;
};

