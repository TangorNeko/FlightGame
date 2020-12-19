#pragma once
#include "IEnemy.h"
class Player;

class MissileEnemy : public IEnemy
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	static int m_missileEnemyNum;

	bool m_isShooting = false;
	int m_movecount = 0;
	int m_shotCooldown = 0;

	CVector3 m_moveDir, m_rightDir, m_upDir, m_oldMoveDir,m_missileDir;
	prefab::CEffect* m_effect = nullptr;
	float degx, degy, odegx = 0, odegy = 0;
	CQuaternion m_qRotX, m_qRotY, m_rotation;

};

