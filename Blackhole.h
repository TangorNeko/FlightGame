#pragma once

class Player;
class GameScene;
class GameScoreManager;
class GameCamera;

class Blackhole: public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();
	void GeneratePanel();
	void GetBonus(int panelNum);

	CVector3 m_position;
	CVector3 m_scaling = { 100.0f,100.0f,100.0f };
	prefab::CEffect* m_bhEffect = nullptr;
	Player* player = nullptr;
	GameScene* m_gameScene = nullptr;
	bool m_isPulling = false;
	GameScoreManager* m_gameScoreManager = nullptr;
	GameCamera* m_gameCamera = nullptr;

	//スプライト
	prefab::CSpriteRender* m_blackHoleDistanceBar = nullptr;
	prefab::CSpriteRender* m_blackHoleBonus[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };
	int m_blackHoleBonusPanelNum[5];
	prefab::CSpriteRender* m_blackHolePlayer = nullptr;
	float m_panelAlpha = 1.0f;

	int count = 0;
	int expire = 100;
	bool isExpanding = true;
	float bhradius = 70;


};

