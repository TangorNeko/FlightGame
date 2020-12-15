#pragma once

class Player;

class Blackhole: public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	CVector3 m_position;// { 1000.f, 1000.0f, 1000.0f };
	CVector3 m_scaling = { 100.0f,100.0f,100.0f };
	prefab::CEffect* m_bhEffect = nullptr;
	Player* player = nullptr;
	bool m_isPulling = false;

	int count = 0;
	int expire = 100;
	bool isExpanding = true;
	float bhradius = 70;
};

