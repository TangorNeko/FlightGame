#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	player = FindGO<Player>("player");

	return true;
}

void GameCamera::Update()
{
	position.x = player->position.x;
	position.y = player->position.y + 200;
	position.z = player->position.z - 300;
	MainCamera().SetPosition(position);

	target = player->position;

	MainCamera().SetTarget(target);

	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);

	MainCamera().Update();
}