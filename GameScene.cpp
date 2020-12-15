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
#include "Title.h"
#include "tkEngine/light/tkDirectionLight.h"

void GameScene::OnDestroy()
{
	Space* space = FindGO<Space>("space");
	DeleteGO(space);

	QueryGOs<Missile>("missile", [](Missile* missile)->bool {
		DeleteGO(missile);
		return true;
		});

	
	//DeleteGO(blackhole);

	//ブラックホール生成時に消してる
	RingGenerator* ringgenerator = FindGO<RingGenerator>("ringgenerator",false);
	if (ringgenerator != nullptr)
	DeleteGO(ringgenerator);
	
	
	QueryGOs<Enemy>("enemy", [](Enemy* enemy)->bool {
		DeleteGO(enemy);
		return true;
		});


	//BackGround* background = FindGO<BackGround>("background");
	//DeleteGO(background);

	DeleteGO(player);

	GameCamera* gamecamera = FindGO<GameCamera>("gamecamera");
	DeleteGO(gamecamera);

	DeleteGOs("DirectionLight");
}

bool GameScene::Start()
{
	//ベクトルを可視化する
	dbg::SetDrawVectorEnable();

	auto light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	CVector3 dir = { 1.0f, 0.0f, 0.0f };

	light->SetDirection(dir);
	light->SetColor({2.0f, 1.0f, 1.0f, 1.0f});

	//
	light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	dir = { 1.0f, 1.0f, 0.0f };
	dir.Normalize();
	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

	light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	dir = { -1.0f, 0.0f, 1.0f };
	dir.Normalize();
	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

	light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	dir = { -1.0f, -1.0f, 0.0f };
	dir.Normalize();
	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

	LightManager().SetAmbientLight({ 0.5f, 0.5f, 0.5f });

	NewGO<GameCamera>(1, "gamecamera");

	shadow::DirectionShadowMap().SetLightDirection({1.0f, 0.0f, 0.0f});
	player = NewGO<Player>(0, "player");

	//NewGO<BackGround>(0, "background");


	//敵をいっぱい作る(後から自動化したい)
	Enemy* enemy = nullptr;
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 0,0000,50000 };

	
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 2000,1000,35000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 10000,1000,31000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { -10000,1000,34000 };
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->m_position = { 11000,1000,34000 };
	

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
		blackhole->m_position = { player->m_position - player->m_moveDir * 10000 };
	}
	

	//ポーズ機能とかつけたい
}
