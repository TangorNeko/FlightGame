#pragma once
class SceneChange : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void PostUpdate();

	prefab::CSpriteRender* m_sceneChangeSprite = nullptr;
	float m_timer = 0;
	bool m_increasing = true;
	CVector3 SceneChangeColor = { 0.0f,0.0f,0.0f };
};

