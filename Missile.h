#pragma once
class IEnemy;
class GameScoreManager;

class Missile : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CQuaternion m_qRot;
	CVector3 m_position;
	CVector3 m_moveDir;
	IEnemy* m_trackingEnemy = nullptr;
	prefab::CEffect* m_effect = nullptr;
	prefab::CEffect* m_effect2 = nullptr;
	float m_fSpeed = 10;

	float odegx = 0, odegy = 0;

	GameScoreManager* m_gamescoremanager = nullptr;
};

