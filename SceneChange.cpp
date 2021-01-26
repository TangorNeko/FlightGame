#include "stdafx.h"
#include "SceneChange.h"

void SceneChange::OnDestroy()
{
	DeleteGO(m_sceneChangeSprite);
}

bool SceneChange::Start()
{
	m_sceneChangeSprite = NewGO<prefab::CSpriteRender>(5);
	m_sceneChangeSprite->Init(L"sprite/SceneChange.dds", 1920, 1080);
	m_sceneChangeSprite->SetMulColor({ SceneChangeColor.x,SceneChangeColor.y,SceneChangeColor.z,0.0f });

	return true;
}

void SceneChange::PostUpdate()
{
	if (m_increasing == true)
		m_timer++;
	else
		m_timer--;

	m_sceneChangeSprite->SetMulColor({ SceneChangeColor.x,SceneChangeColor.y,SceneChangeColor.z,min(m_timer/10,1.0f) });

	if (m_timer > 20)
		m_increasing = false;

	if (m_timer < 0)
		DeleteGO(this);
}
