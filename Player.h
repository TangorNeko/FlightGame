#pragma once
class Enemy;

class Player : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();
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
	prefab::CSpriteRender* m_spriteRender = nullptr;
	Enemy* m_lockingEnemy = nullptr;
};

