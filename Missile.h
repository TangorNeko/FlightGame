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
	Enemy* m_trackingEnemy = nullptr;
};

