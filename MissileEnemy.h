#pragma once
#include "IEnemy.h"
class Player;
class GameScene;

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

	prefab::CSpriteRender* m_enemyPosRender = nullptr;

	CVector3 m_missileDir;
	prefab::CEffect* m_effect = nullptr;
	float degx, degy, odegx = 0, odegy = 0;
	CQuaternion m_qRotX, m_qRotY, m_rotation;

	GameScene* m_gameScene = nullptr;

};

