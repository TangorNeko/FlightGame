#include "stdafx.h"
#include "GameOver.h"
#include "GameScene.h"
#include "SceneChange.h"

void GameOver::OnDestroy()
{
	DeleteGO(m_spriteRender);
}

bool GameOver::Start()
{
	m_spriteRender = NewGO<prefab::CSpriteRender>(0);
	m_spriteRender->Init(L"sprite/GameOver.dds",300,350);

	m_gameScene = FindGO<GameScene>("gamescene");
	return true;
}

void GameOver::Update()
{
	m_timeCount++;

	if (m_timeCount == 290)
		NewGO<SceneChange>(0);

	if (m_timeCount >= 300)
	{
		m_gameScene->m_isGameEnd = true;
		DeleteGO(this);
	}
}
