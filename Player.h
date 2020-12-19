#pragma once
class IEnemy;

class Player : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();
	void PostUpdate();
	void Lockon();
	void ShootMissile();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	CVector3 m_moveDir,m_rightDir,m_upDir;
	float m_fSpeed = 50;
	float m_movex = 0, m_movez = 0;
	float m_x=0, m_y=0, m_z=0;
	CQuaternion m_qRotX, m_qRotY, m_qRotZ;
	CQuaternion m_rotation;

	prefab::CSpriteRender* m_sightSpriteRender = nullptr;
	prefab::CSpriteRender* m_lockonSpriteRender = nullptr;
	IEnemy* m_lockingEnemy = nullptr;
	float m_maxLockonDistance = 25000.0f;
	int m_shotcooldown = 0;
	prefab::CEffect* m_frictionEffect = nullptr;
	prefab::CEffect* m_sbEffectRight = nullptr,*m_sbEffectLeft = nullptr;
	prefab::CEffect* m_jetEffectRight = nullptr, * m_jetEffectLeft = nullptr;
	bool m_isBoosting = false;
	bool m_isTurning = false;
	float m_fSpeedBeforeTurn = 0;
	bool m_isMissileRight = false;
	int m_turnCount = 0;
	int m_trackingMissileNum = 0;

	CShaderResourceView m_specSRV;
	int m_hp = 10000;
	float m_fuel = 5000;
	int m_score = 0;
	prefab::CFontRender* m_fontRender = nullptr;
};

