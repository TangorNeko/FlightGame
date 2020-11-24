#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "Player.h"
#include "BackGround.h"
#include "Enemy.h"
#include "Ring.h"

void GameScene::OnDestroy()
{
	QueryGOs<Enemy>("enemy", [](Enemy* enemy)->bool {
		DeleteGO(enemy);
		return true;
		});

	BackGround* background = FindGO<BackGround>("background");
	DeleteGO(background);

	Player* player = FindGO<Player>("player");
	DeleteGO(player);

	GameCamera* gamecamera = FindGO<GameCamera>("gamecamera");
	DeleteGO(gamecamera);
}

bool GameScene::Start()
{
	//ベクトルを可視化する
	dbg::SetDrawVectorEnable();

	NewGO<GameCamera>(0, "gamecamera");

	NewGO<Player>(0, "player");

	NewGO<BackGround>(0, "background");


	//敵をいっぱい作る(後から自動化したい)
	Enemy* enemy = nullptr;
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 5000,1000,2000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 2000,1000,5000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 10000,1000,1000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 1000,1000,10000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 3000,1000,7500 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 7500,1000,3000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 4000,1000,3000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 3000,1000,4000 };

	Ring* ring = nullptr;
	ring = NewGO<Ring>(0, "ring");

	return true;
}

void GameScene::Update()
{

	//Aボタンでリセット
	if (Pad(0).IsTrigger(enButtonA))
	{
		NewGO<GameScene>(0, "gamescene");
		DeleteGO(this);
	}

	//ポーズ機能とかつけたい
}
