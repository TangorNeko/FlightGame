#pragma once
#include "IEnemy.h"
class Player;

class LaserEnemy : public IEnemy
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	static int m_laserEnemyNum;

	bool m_isShooting = false;
	int m_movecount = 0;

	CVector3 laserpos;
	CVector3 m_moveDir,m_rightDir,m_upDir,m_oldMoveDir;
	prefab::CEffect* m_effect = nullptr;
	float degx, degy, odegx = 0, odegy = 0;
	float laserdegx, laserdegy, laserodegx = 0, laserodegy = 0;
	CQuaternion m_qRotX, m_qRotY, m_rotation;
	CQuaternion m_laserqRotX, m_laserqRotY, m_laserRotation;
	bool m_movingRight = true;
	bool m_movingUp;

	bool m_isDeleted = true;
	int m_damageCount = 0;
};



