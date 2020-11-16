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

	position.x = player->position.x - player->MoveDir.x * 250;
	position.y = player->position.y - player->MoveDir.y * 250;
	position.z = player->position.z - player->MoveDir.z * 250;
	MainCamera().SetPosition(position);

	target = player->position;

	MainCamera().SetTarget(target);
	//MainCamera().SetUp({ 1.0f,0.0f,0.0f });

	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(100000.0f);

	MainCamera().Update();
}