#pragma once
class Player;

class IEnemy : public IGameObject
{
public:
	virtual void OnDestroy() override = 0;
	virtual bool Start() override = 0;
	virtual void Update() override = 0;

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position,m_moveDir, m_rightDir, m_upDir, m_oldMoveDir;
	bool m_isMortal = false;
	Player* m_targetPlayer = nullptr;
	CShaderResourceView m_specSRV;

	//ミッション用変数
	static int s_killEnemyNum;
	static int s_surpriseUFONum;
	static int s_surpriseFighterNum;

	bool m_isUFO = false;
	bool m_isAttackSuccess = false;
};

