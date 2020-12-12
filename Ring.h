#pragma once
class Player;

class Ring : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();
	static int s_ringNum;

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	Player* player = nullptr;

	CVector3 m_position;
	CVector3 m_direction;
	float m_x = 0, m_y = 0;
	CQuaternion m_qRotX, m_qRotY;
	CQuaternion m_rotation;

	prefab::CPointLight* plight = nullptr;
	CShaderResourceView m_specSRV;
};

