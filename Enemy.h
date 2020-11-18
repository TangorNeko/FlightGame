#pragma once
class Enemy : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* tModel = nullptr;
	CVector3 position;
};

