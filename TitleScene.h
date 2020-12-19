#pragma once
class TitlePlayer;
class SpaceDock;

class TitleScene : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	TitlePlayer* player = nullptr;
	SpaceDock* spacedock = nullptr;
	prefab::CSky* space = nullptr;

};

