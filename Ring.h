#pragma once
class Player;

class Ring : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	Player* player = nullptr;

	CVector3 m_position = { 0.0f,0.0f,5000.0f };
	CVector3 m_direction = CVector3::Front;
	CQuaternion m_qRot;
};

