#pragma once
class Player;
class Enemy;

class Laser : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	float degx, degy, odegx = 0, odegy = 0;
	CVector3 m_position;
	CQuaternion m_qRotX, m_qRotY, m_rotation;
	prefab::CEffect* m_effect = nullptr;
	Player* m_targetPlayer = nullptr;
	Enemy* m_owner = nullptr;

	int m_damageCount = 0;
};

