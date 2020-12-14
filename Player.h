#pragma once
class Enemy;

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
	CVector3 m_moveDir = CVector3::AxisZ;
	float m_fSpeed = 0;
	float m_movex = 0, m_movez = 0;
	float m_x=0, m_y=0, m_z=0;
	CQuaternion m_qRotX, m_qRotY, m_qRotZ,m_qRotCam;
	CQuaternion m_rotation;

	CVector3 m_cameraup = CVector3::Up;
	CQuaternion m_camerarot;
	prefab::CSpriteRender* m_sightSpriteRender = nullptr;
	prefab::CSpriteRender* m_lockonSpriteRender = nullptr;
	Enemy* m_lockingEnemy = nullptr;
	float m_maxLockonDistance = 25000.0f;
	int m_shotcooldown = 0;
	prefab::CEffect* m_frictionEffect = nullptr;
	bool m_isBoosting = false;
	bool m_isTurning = false;
	int m_turnCount = 0;

	CShaderResourceView m_specSRV;
	int m_hp = 10000;
	int m_fuel = 5000;
	int m_score = 0;
	prefab::CFontRender* m_fontRender = nullptr;
};

