#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "Player.h"
#include "BackGround.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
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
	NewGO<GameCamera>(0, "gamecamera");

	NewGO<Player>(0, "player");

	NewGO<BackGround>(0, "background");

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
