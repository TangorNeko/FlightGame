#pragma once
class Player;

class EnemyMissile : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CQuaternion m_qRot;
	CVector3 m_position;
	CVector3 m_moveDir;
	CVector3 m_oldMoveDir;
	Player* m_trackingPlayer = nullptr;
	prefab::CEffect* m_effect = nullptr;
	prefab::CEffect* m_effect2 = nullptr;
	float m_fSpeed = 10;
	bool m_isRocking = true;

	int m_lifeSpan = 900;

	float odegx = 0, odegy = 0;
};

