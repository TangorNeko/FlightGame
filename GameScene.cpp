#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "Player.h"
#include "BackGround.h"
#include "Enemy.h"

void GameScene::OnDestroy()
{
	GameCamera* gamecamera = FindGO<GameCamera>("gamecamera");
	DeleteGO(gamecamera);

	Player* player = FindGO<Player>("player");
	DeleteGO(player);

	BackGround* background = FindGO<BackGround>("background");
	DeleteGO(background);
}

bool GameScene::Start()
{
	dbg::SetDrawVectorEnable();
	NewGO<GameCamera>(0, "gamecamera");

	NewGO<Player>(0, "player");

	NewGO<BackGround>(0, "background");


	Enemy* enemy = nullptr;
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 5000,1000,2000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 2000,1000,5000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 10000,1000,1000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 1000,1000,10000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 3000,1000,7500 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 7500,1000,3000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 4000,1000,3000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->position = { 3000,1000,4000 };

	return true;
}

void GameScene::Update()
{
	if (Pad(0).IsTrigger(enButtonA))
	{
		NewGO<GameScene>(0, "gamescene");
		DeleteGO(this);
	}
}
