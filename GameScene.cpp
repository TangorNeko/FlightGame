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
#include "SmokeGenerator.h"
#include "GameScoreManager.h"
#include "tkEngine/light/tkDirectionLight.h"
#include <random>


void GameScene::OnDestroy()
{
	DeleteGO(m_fontRender);

	GameScoreManager* gameScoreManager = FindGO<GameScoreManager>("gamescoremanager");
	DeleteGO(gameScoreManager);

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

	if(m_isBlackholePhase == true)
		DeleteGO(blackhole);

	//ブラックホール生成時に消してる
	RingGenerator* ringgenerator = FindGO<RingGenerator>("ringgenerator",false);
	if (ringgenerator != nullptr)
		DeleteGO(ringgenerator);
	
	//敵の削除のタイミングは要検討
	
	EnemyGenerator* enemygenerator = FindGO<EnemyGenerator>("enemygenarator", false);
	if (enemygenerator != nullptr)
	{
		DeleteGO(enemygenerator);
	}
	
	
	QueryGOs<IEnemy>("enemy", [](IEnemy* enemy)->bool {
		DeleteGO(enemy);
		return true;
		});

	
	if(m_isPlayerDead == false)
		DeleteGO(player);

	GameCamera* gamecamera = FindGO<GameCamera>("gamecamera");
	DeleteGO(gamecamera);

	DeleteGOs("DirectionLight");

	DeleteGO(m_gameBGM);
}

bool GameScene::Start()
{
	//ベクトルを可視化する
	dbg::SetDrawVectorEnable();

	auto light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	CVector3 dir = { 1.0f, 0.0f, 0.0f };

	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

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

	LightManager().SetAmbientLight({ 0.2f, 0.2f, 0.2f });

	NewGO<GameCamera>(1, "gamecamera");

	shadow::DirectionShadowMap().SetLightDirection({ 1.0f, 0.0f, 0.0f });

	player = NewGO<Player>(0, "player");

	//敵をいっぱい作る
	NewGO<EnemyGenerator>(0, "enemygenerator");

	NewGO<RingGenerator>(0, "ringgenerator");

	NewGO<Space>(0, "space");

	NewGO<GameScoreManager>(0, "gamescoremanager");

	m_fontRender = NewGO<prefab::CFontRender>(0);

	//BGM再生
	m_gameBGM = NewGO<prefab::CSoundSource>(0, "bgm");
	m_gameBGM->Init(L"sound/Space_Travel.wav");
	m_gameBGM->SetVolume(0.25f);
	//m_gameBGM->Play(true);

	m_warningSpriteUp = NewGO<prefab::CSpriteRender>(3);
	m_warningSpriteUp->Init(L"sprite/WarningUp.dds", 2560, 720);
	m_warningSpriteUp->SetPosition({ 2560.0f,0.0f,0.0f });

	m_warningSpriteDown = NewGO<prefab::CSpriteRender>(3);
	m_warningSpriteDown->Init(L"sprite/WarningDown.dds", 2560, 720);
	m_warningSpriteDown->SetPosition({-2560.0f, 0.0f, 0.0f});



	return true;
}

void GameScene::Update()
{
	//Selectボタン,Spaceキーでリセット
	if (Pad(0).IsTrigger(enButtonSelect) || m_isGameEnd == true)
	{
		NewGO<TitleScene>(0, "titlescene");
		DeleteGO(this);
	}

	if (m_gameTimer > 10500)
	{
		m_warningCount += 15;

		m_warningSpriteUp->SetPosition({ 2560.0f - m_warningCount,0.0f,0.0f });
		m_warningSpriteDown->SetPosition({ -2560.0f + m_warningCount,0.0f,0.0f });
	}
	
	if (m_gameTimer == 10500 && m_isPlayerDead == false)
	{
		prefab::CSoundSource* warningSound = NewGO<prefab::CSoundSource>(0);
		warningSound->Init(L"sound/Warning.wav");
		warningSound->SetVolume(5.0f);
		warningSound->Play(false);
	}

	if (Pad(0).IsTrigger(enButtonStart) || m_gameTimer == 10800 && m_isPlayerDead == false)
	{
		blackhole = NewGO<Blackhole>(0, "blackhole");
		blackhole->m_position = { player->m_position - player->m_moveDir * 10000 };
		m_isBlackholePhase = true;

		DeleteGO(m_gameBGM);
		m_gameBGM = NewGO<prefab::CSoundSource>(0, "bgm");
		m_gameBGM->Init(L"sound/Blackhole.wav");
		m_gameBGM->SetVolume(0.25f);
		//m_gameBGM->Play(true);

		m_fontRender->SetScale(0.0f);
	}
	

	
	//ゲーム時間300から5400の間だけミッション発生
	if (300 < m_gameTimer && m_gameTimer < 10200)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		if (rnd() % 100 == 77 && m_isOnMission == false)
		{
			m_mission = NewGO<Mission>(0, "mission");
			m_isOnMission = true;
		}
	}
	
	//ミッション終了を受け取りミッションを削除する
	if (m_isOnMission == true && (m_mission->m_isMissionEnd || m_isPlayerDead == true || m_gameTimer  == 10200))
	{
		DeleteGO(m_mission);
		m_mission = nullptr;
		m_isOnMission = false;
	}



	//ブラックホール発生までの残り時間の表示
	int remainTime = (10800 - m_gameTimer) / 60;
	if (remainTime < 0)
		remainTime = 0;

	std::wstring remainTimeText = std::to_wstring(remainTime) ;
	m_fontRender->SetText(remainTimeText.c_str());
	m_fontRender->SetPivot({0.5f,0.5f});
	m_fontRender->SetPosition({ 0.0f,300.0f });
	m_fontRender->SetShadowParam(true, 1.0f, { 0.0f,0.0f,0.5f,1.0f });

	m_gameTimer++;
}
