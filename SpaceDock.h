#pragma once
class TitlePlayer;
class SpaceDock : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_dockModel = nullptr;
	prefab::CSkinModelRender* m_teleporterModel = nullptr;
	prefab::CEffect* m_portalEffect = nullptr;

	bool m_isPortalOpen = false;

	TitlePlayer* m_player = nullptr;
};

