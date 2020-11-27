#pragma once
class Enemy : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	bool m_isMortal = false;

	int count = 0;
};

