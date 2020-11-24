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

	CVector3 m_position;
};

