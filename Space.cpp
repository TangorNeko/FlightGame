#include "stdafx.h"
#include "Space.h"
#include "Player.h"
#include "GameScene.h"

void Space::OnDestroy()
{
	DeleteGO(space);
}

bool Space::Start()
{
	
	space = NewGO<prefab::CSky>(0,"space");
	space->SetSkyCubeMapFilePath(L"sprite/GreenCubeMap.dds");
	space->SetScale(1000000.0f);

	player = FindGO<Player>("player");
	m_position = player->m_position;
	space->SetPosition(m_position);

	gameScene = FindGO<GameScene>("gamescene");
	return true;
}

void Space::Update()
{
	if (gameScene->m_isPlayerDead == false)
		m_position = player->m_position;

	space->SetPosition(m_position);
}