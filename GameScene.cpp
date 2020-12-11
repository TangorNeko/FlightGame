#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "Player.h"
#include "BackGround.h"
#include "Enemy.h"
#include "Blackhole.h"
#include "Missile.h"
#include "Space.h"
#include "RingGenerator.h"

void GameScene::OnDestroy()
{
	Space* space = FindGO<Space>("space");
	DeleteGO(space);

	QueryGOs<Missile>("missile", [](Missile* missile)->bool {
		DeleteGO(missile);
		return true;
		});

	
	DeleteGO(blackhole);

	/*ブラックホール生成時に消してる
	RingGenerator* ringgenerator = FindGO<RingGenerator>("ringgenerator");
	DeleteGO(ringgenerator);
	*/

	QueryGOs<Enemy>("enemy", [](Enemy* enemy)->bool {
		DeleteGO(enemy);
		return true;
		});

	//BackGround* background = FindGO<BackGround>("background");
	//DeleteGO(background);

	DeleteGO(player);

	GameCamera* gamecamera = FindGO<GameCamera>("gamecamera");
	DeleteGO(gamecamera);
}

bool GameScene::Start()
{
	//ベクトルを可視化する
	dbg::SetDrawVectorEnable();

	NewGO<GameCamera>(0, "gamecamera");

	player = NewGO<Player>(0, "player");

	//NewGO<BackGround>(0, "background");


	//敵をいっぱい作る(後から自動化したい)
	Enemy* enemy = nullptr;
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 0,0000,20000 };
	/*
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 2000,1000,5000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 10000,1000,1000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 1000,1000,10000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 3000,1000,7500 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { -7500,1000,3000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 4000,1000,3000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 3000,1000,4000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 50000,1000,4000 };
	enemy = NewGO<Enemy>(0, "eneemy");
	enemy->m_position = { 51000,1000,4000 };
	*/

	NewGO<RingGenerator>(0, "ringgenerator");

	
	//blackhole = NewGO<Blackhole>(0, "blackhole");
	//blackhole->m_position = { 5000.0f,200.0f,500.0f };

	NewGO<Space>(0, "space");

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

	
	if (Pad(0).IsTrigger(enButtonRB2))
	{
		blackhole = NewGO<Blackhole>(0, "blackhole");
		blackhole->m_position = { 5000.0f,200.0f,500.0f };
	}
	

	//ポーズ機能とかつけたい
}
