#include "stdafx.h"
#include "Space.h"
#include "Player.h"

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
	space->SetPosition(player->m_position);
	return true;
}

void Space::Update()
{
	space->SetPosition(player->m_position);
}