#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "Player.h"
#include "BackGround.h"
#include "LaserEnemy.h"
#include "MissileEnemy.h"
#include "Blackhole.h"
#include "Missile.h"
#include "Space.h"
#include "RingGenerator.h"
#include "TitleScene.h"
#include "EnemyMissile.h"
#include "EnemyGenerator.h"
#include "Mission.h"
#include "tkEngine/light/tkDirectionLight.h"
#include <random>


void GameScene::OnDestroy()
{
	Space* space = FindGO<Space>("space");
	DeleteGO(space);

	QueryGOs<Missile>("missile", [](Missile* missile)->bool {
		DeleteGO(missile);
		return true;
		});

	QueryGOs<EnemyMissile>("enemymissile", [](EnemyMissile* enemymissile)->bool {
		DeleteGO(enemymissile);
		return true;
		});

	
	//DeleteGO(blackhole);

	//ブラックホール生成時に消してる
	RingGenerator* ringgenerator = FindGO<RingGenerator>("ringgenerator",false);
	if (ringgenerator != nullptr)
	DeleteGO(ringgenerator);
	

	DeleteGO(enemygenerator);
	//敵の削除のタイミングは要検討
	/*
	QueryGOs<IEnemy>("enemy", [](IEnemy* enemy)->bool {
		DeleteGO(enemy);
		return true;
		});
		*/


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


	//敵をいっぱい作る
	enemygenerator = NewGO<EnemyGenerator>(0, "enemygenerator");
	/*
	LaserEnemy* lenemy = nullptr;
	lenemy = NewGO<LaserEnemy>(0, "enemy");
	lenemy->m_position = { 0,0000,50000 };
	lenemy = NewGO<LaserEnemy>(0, "enemy");
	lenemy->m_position = { 2000,1000,65000 };
	lenemy = NewGO<LaserEnemy>(0, "enemy");
	lenemy->m_position = { 10000,1000,61000 };
	lenemy = NewGO<LaserEnemy>(0, "enemy");
	lenemy->m_position = { -10000,1000,64000 };
	lenemy = NewGO<LaserEnemy>(0, "enemy");
	lenemy->m_position = { 11000,1000,64000 };

	
	MissileEnemy* menemy = nullptr;
	menemy = NewGO<MissileEnemy>(0, "enemy");
	menemy->m_position = { 3000,2000,36000 };
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
		NewGO<TitleScene>(0, "titlescene");
		DeleteGO(this);
	}

	
	if (Pad(0).IsTrigger(enButtonRB2))
	{
		blackhole = NewGO<Blackhole>(0, "blackhole");
		blackhole->m_position = { player->m_position - player->m_moveDir * 10000 };
	}
	

	if (m_gameTimer > 300)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		if (rnd() % 1000 == 777 && m_isOnMission == false)
		{
			m_mission = NewGO<Mission>(0, "mission");
			m_isOnMission = true;
		}
	
		if (m_isOnMission == true && m_mission->m_isMissionEnd)
		{
			DeleteGO(m_mission);
			m_mission = nullptr;
			m_isOnMission = false;
		}
	}


	m_gameTimer++;
	//ポーズ機能とかつけたい
}
