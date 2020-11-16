#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "Player.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

bool GameScene::Start()
{
	NewGO<GameCamera>(0, "gamecamera");

	NewGO<Player>(0, "player");
}

void GameScene::Update()
{

}
