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
	bool m_isShooting = false;

	float dbgDegx = 0, dbgDegy = 0;

	int count = 0;
	CShaderResourceView m_specSRV;


	CVector3 laserpos;
	prefab::CEffect* m_effect = nullptr;
	float degx, degy, odegx = 0, odegy = 0;
	CQuaternion m_qRotX, m_qRotY, m_rotation;

	bool m_isDeleted = true;
	int m_damageCount = 0;
};

