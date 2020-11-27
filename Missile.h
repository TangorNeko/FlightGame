#pragma once
class Enemy;

class Missile : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	CVector3 m_moveDir;
	Enemy* m_trackingEnemy = nullptr;
	prefab::CEffect* m_effect = nullptr;
	float m_fSpeed = 1;
};

