#pragma once
class Blackhole: public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	CVector3 m_position;
	CVector3 m_scaling;
	prefab::CEffect* m_bhEffect = nullptr;
};

