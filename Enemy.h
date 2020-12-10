#pragma once
class Player;

class Enemy : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	bool m_isMortal = false;
	Player* m_targetPlayer = nullptr;
	prefab::CEffect* m_laserEffect = nullptr;
	bool m_isShooting = false;
	CQuaternion m_laserQRot;
	float odegx = 0, odegy = 0;

	int count = 0;
};

