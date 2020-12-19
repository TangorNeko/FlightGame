#pragma once

class TitlePlayer : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	CVector3 m_moveDir, m_rightDir, m_upDir;
	float m_fSpeed = 0;
	float m_movex = 0, m_movez = 0;
	float m_x = 0, m_y = 0, m_z = 0;
	CQuaternion m_qRotX, m_qRotY, m_qRotZ;
	CQuaternion m_rotation;

	prefab::CEffect* m_frictionEffect = nullptr;
	prefab::CEffect* m_sbEffectRight = nullptr, * m_sbEffectLeft = nullptr;
	prefab::CEffect* m_jetEffectRight = nullptr, * m_jetEffectLeft = nullptr;
	bool m_isBoosting = false;
	bool m_isTurning = false;
	float m_fSpeedBeforeTurn = 0;
	bool m_isMissileRight = false;
	int m_turnCount = 0;

	CShaderResourceView m_specSRV;
};

