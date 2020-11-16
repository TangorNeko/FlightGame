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

}
