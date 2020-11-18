#pragma once
class BackGround : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* BGmodel = nullptr;
};

