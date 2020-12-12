#pragma once
class Player;
class Laser;

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

	Laser* m_laser = nullptr;
	float dbgDegx = 0, dbgDegy = 0;

	int count = 0;
	CShaderResourceView m_specSRV;
};

