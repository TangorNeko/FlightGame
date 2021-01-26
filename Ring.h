#pragma once
class Player;
class GameScoreManager;

class Ring : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();
	static int s_ringNum;
	static int s_redRingStreak;
	static int s_blueRingStreak;
	static int s_alternateRingStreak;
	static int s_lastRingFlag; //0ÅAèâä˙ 1ÅAê‘ 2ÅAê¬

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CSkinModelRender* m_fuelPanelRender = nullptr;
	prefab::CSkinModelRender* m_scorePanelRender = nullptr;
	Player* player = nullptr;
	GameScoreManager* gamescoremanager = nullptr;

	CVector3 m_position;
	CVector3 m_direction;
	float m_x = 0, m_y = 0;
	CQuaternion m_qRotX, m_qRotY;
	CQuaternion m_rotation;

	prefab::CPointLight* plight = nullptr;
	CShaderResourceView m_specSRV;
};

