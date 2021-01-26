#include "stdafx.h"
#include "Mission.h"
#include "Ring.h"
#include "IEnemy.h"
#include "Player.h"
#include "EnemyMissile.h"
#include "GameScoreManager.h"
#include <random>

void Mission::OnDestroy()
{
	DeleteGO(m_fontRender);
	DeleteGO(m_missionSpriteRender);
	DeleteGOs("time");
}

bool Mission::Start()
{
	std::random_device seed_gen;
	std::mt19937_64 rnd(seed_gen());
	m_receiveMission = (rnd() % 5) * 2;

	//ミッション達成状況用のテキスト
	m_fontRender = NewGO<prefab::CFontRender>(1);
	m_fontRender->SetScale(1.0f);
	m_fontRender->SetPosition({ -500.0f,-285.0f });
	m_fontRender->SetPivot({ 0.5f,0.5f });
	m_fontRender->SetShadowParam(true, 1.0f, { 0.0f,0.0f,0.5f,1.0f });

	m_missionTimeSpriteRender[0] = NewGO<prefab::CSpriteRender>(1,"time");
	m_missionTimeSpriteRender[0]->Init(L"sprite/Missions/Time1.dds", 300.0f, 250.0f);
	m_missionTimeSpriteRender[0]->SetPosition({ -628.0f,-235.0f,0.0f });
	m_missionTimeSpriteRender[0]->SetPivot({ 0.04f,0.5f });
	m_missionTimeSpriteRender[0]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });
	
	m_missionTimeSpriteRender[1] = NewGO<prefab::CSpriteRender>(1,"time");
	m_missionTimeSpriteRender[1]->Init(L"sprite/Missions/Time2.dds",300.0f, 250.0f);
	m_missionTimeSpriteRender[1]->SetPosition({ -490.0f,-125.0f,0.0f });
	m_missionTimeSpriteRender[1]->SetPivot({ 0.5f,0.94f });
	m_missionTimeSpriteRender[1]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });

	m_missionTimeSpriteRender[2] = NewGO<prefab::CSpriteRender>(1,"time");
	m_missionTimeSpriteRender[2]->Init(L"sprite/Missions/Time3.dds", 300.0f, 250.0f);
	m_missionTimeSpriteRender[2]->SetPosition({ -351.0f,-235.0f,0.0f });
	m_missionTimeSpriteRender[2]->SetPivot({ 0.963f,0.5f });
	m_missionTimeSpriteRender[2]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });

	m_missionTimeSpriteRender[3] = NewGO<prefab::CSpriteRender>(1,"time");
	m_missionTimeSpriteRender[3]->Init(L"sprite/Missions/Time4.dds", 300.0f, 250.0f);
	m_missionTimeSpriteRender[3]->SetPosition({ -490.0f,-328.0f,0.0f });
	m_missionTimeSpriteRender[3]->SetPivot({ 0.5f,0.130f });
	m_missionTimeSpriteRender[3]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });

	m_missionTimeSpriteRender[4] = NewGO<prefab::CSpriteRender>(1,"time");
	m_missionTimeSpriteRender[4]->Init(L"sprite/Missions/Time5.dds", 300.0f, 250.0f);
	m_missionTimeSpriteRender[4]->SetPosition({ -490.0f,-235.0f,0.0f });
	m_missionTimeSpriteRender[4]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });

	m_player = FindGO<Player>("player");
	m_gamescoremanager = FindGO<GameScoreManager>("gamescoremanager");

	prefab::CSoundSource* noticeSound = NewGO<prefab::CSoundSource>(0);
	noticeSound->Init(L"sound/Notice.wav");
	noticeSound->SetVolume(0.5f);
	noticeSound->Play(false);
	return true;
}

void Mission::Update()
{

	//ミッション受領時の処理
	if (Pad(0).IsTrigger(enButtonLB1) && m_isMissionConfirm == false)
	{
		m_isMissionConfirm = true;

		DeleteGO(m_missionSpriteRender);
		m_missionSpriteRender = nullptr;

		m_missionTimer = 3600;

		m_missionTimeSpriteRender[0]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });
		m_missionTimeSpriteRender[1]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });
		m_missionTimeSpriteRender[2]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });
		m_missionTimeSpriteRender[3]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });
		m_missionTimeSpriteRender[4]->SetMulColor({ 0.0f,1.0f,1.0f,0.7f });

		switch (m_receiveMission)
		{
		case 0://ミッション1 同じ色のリングを連続でくぐる
			Ring::s_redRingStreak = 0;
			Ring::s_blueRingStreak = 0;
			break;
		case 1://ミッション1 裏 リングを交互にくぐる
			Ring::s_alternateRingStreak = 0;
			Ring::s_lastRingFlag = 0;
			break;
		case 2://ミッション2 ミサイルをX発発射する
			m_player->m_shootMissileNum = 0;
			break;
		case 3://ミッション2 裏 ミサイルをX発回避する
			EnemyMissile::s_missileAvoidNum = 0;
			break;
		case 4://ミッション3 敵をX体倒す
			IEnemy::s_killEnemyNum = 0;
			break;
		case 5://ミッション3 裏 一定時間敵からダメージを受けない
			m_player->m_damageCount = 0;
			break;
		case 6://ミッション4 レーザーを食らわずUFOをX体倒す
			IEnemy::s_surpriseUFONum = 0;
			break;
		case 7://ミッション4 裏ミサイルを食らわず敵機をX体倒す
			IEnemy::s_surpriseFighterNum = 0;
			break;
		case 8://ミッション5 最高速度をX秒維持する
			m_player->m_highSpeedCount = 0;
			break;
		case 9://ミッション5 裏最低速度をX秒維持する
			m_player->m_lowSpeedCount = 0;
			break;
		}

		prefab::CSoundSource* missionSelecteSound = NewGO<prefab::CSoundSource>(0);
		missionSelecteSound->Init(L"sound/MissionSelect.wav");
		missionSelecteSound->SetVolume(0.5f);
		missionSelecteSound->Play(false);
	}

	//ミッション選択時の処理
	if (m_isMissionConfirm == false)
	{
		if (Pad(0).IsTrigger(enButtonA))
		{
			if (m_isSelectReverseMission)
			{
				m_receiveMission--;
				m_isSelectReverseMission = false;

				DeleteGO(m_missionSpriteRender);
				m_missionSpriteRender = nullptr;
				m_missionSprite = 0.0f;
			}
			else
			{
				m_receiveMission++;
				m_isSelectReverseMission = true;

				DeleteGO(m_missionSpriteRender);
				m_missionSpriteRender = nullptr;
				m_missionSprite = 0.0f;
			}

			prefab::CSoundSource* missionChangeSound = NewGO<prefab::CSoundSource>(0);
			missionChangeSound->Init(L"sound/MissionChange.wav");
			missionChangeSound->SetVolume(1.0f);
			missionChangeSound->Play(false);
			
		}
		switch (m_receiveMission)
		{
		case 0://ミッション1
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission1N.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション1\n同じ色のリングを連続でくぐる";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			*/
			break;
		case 1://ミッション1 裏
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission1H.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション1 裏\nリングを交互にくぐる";
			m_fontRender->SetText(teststring.c_str());
			*/
			break;
		case 2://ミッション2
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission2N.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション2\nミサイルをX発発射する";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			*/
			break;
		case 3://ミッション2 裏
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission2H.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション2 裏\nミサイルをX発回避する";
			m_fontRender->SetText(teststring.c_str());
			*/
			break;
		case 4://ミッション3
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission3N.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション3\n敵をX体倒す";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			*/
			break;
		case 5://ミッション3 裏
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission3H.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション3 裏\n 一定時間敵からダメージを受けない";
			m_fontRender->SetText(teststring.c_str());
			*/
			break;
		case 6://ミッション4
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission4N.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション4\nレーザーを食らわずUFOをX体倒す";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			*/
			break;
		case 7://ミッション4 裏
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission4H.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション4 裏\nミサイルを食らわず敵機をX体倒す";
			m_fontRender->SetText(teststring.c_str());
			*/
			break;
		case 8://ミッション5
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission5N.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション5\n最高速度をX秒維持する";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			*/
			break;
		case 9://ミッション5 裏
			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission5H.dds", 300, 250);
			}
			else
			{
				m_missionSpriteRender->SetScale({ 1.0f,min(1.0f,m_missionSprite),1.0f });
			}
			m_missionSprite += 0.15f;
			/*
			teststring = L"ミッション5 裏\n最低速度をX秒維持する";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			*/
			break;
		}

		m_missionTimeSpriteRender[0]->SetScale({max(m_missionTimer-1050.0f,0.0f) / 150.0f ,1.0f, 1.0f });
		m_missionTimeSpriteRender[1]->SetScale({ 1.0f,min(max(m_missionTimer - 750.0f,0.0f) / 300.0f,1.0f),1.0f });
		m_missionTimeSpriteRender[2]->SetScale({ min(max(m_missionTimer - 450.0f,0.0f) / 300.0f,1.0f),1.0f,1.0f });
		m_missionTimeSpriteRender[3]->SetScale({ 1.0f,min(max(m_missionTimer - 150.0f,0.0f) / 300.0f,1.0f),1.0f });
		m_missionTimeSpriteRender[4]->SetScale({ min(max(m_missionTimer,0.0f) / 150.0f,1.0f),1.0f,1.0f });

		if (m_missionTimer / 60 == 10)
		{
			m_missionTimeSpriteRender[0]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[1]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[2]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[3]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[4]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
		}

		m_missionTimer--;
	}
	else
	{
		//ミッション遂行中の処理
		switch (m_receiveMission)
		{
		case 0://ミッション1

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission1NR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(max(Ring::s_redRingStreak, Ring::s_blueRingStreak)) + L"/5";
			m_fontRender->SetText(teststring.c_str());
			


			if (Ring::s_redRingStreak == 5 || Ring::s_blueRingStreak == 5)
			{
				m_gamescoremanager->m_gameScore += 1000;
				m_isMissionEnd = true;
			}
			break;
		case 1://ミッション1 裏

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission1HR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(Ring::s_alternateRingStreak) + L"/6";
			m_fontRender->SetText(teststring.c_str());
			

			if (Ring::s_alternateRingStreak == 6)
			{
				m_gamescoremanager->m_gameScore += 1250;
				m_isMissionEnd = true;
			}
			break;
		case 2://ミッション2

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission2NR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(m_player->m_shootMissileNum) + L"/5";
			m_fontRender->SetText(teststring.c_str());
			

			if (m_player->m_shootMissileNum == 5)
			{
				m_gamescoremanager->m_gameScore += 1000;
				m_isMissionEnd = true;
			}
			break;
		case 3://ミッション2 裏

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission2HR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(EnemyMissile::s_missileAvoidNum) + L"/3";
			m_fontRender->SetText(teststring.c_str());
			

			if (EnemyMissile::s_missileAvoidNum >= 3)
			{
				m_gamescoremanager->m_gameScore += 2000;
				m_isMissionEnd = true;
			}
			break;
		case 4://ミッション3

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission3NR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(IEnemy::s_killEnemyNum) + L"/5";
			m_fontRender->SetText(teststring.c_str());
			

			if (IEnemy::s_killEnemyNum == 5)
			{
				m_gamescoremanager->m_gameScore += 1000;
				m_isMissionEnd = true;
			}
			break;
		case 5://ミッション3 裏

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission3HR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(m_player->m_damageCount /60) + L"/10";
			m_fontRender->SetText(teststring.c_str());
			

			if (m_player->m_damageCount >= 600)
			{
				m_gamescoremanager->m_gameScore += 3000;
				m_isMissionEnd = true;
			}
			break;
		case 6://ミッション4

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission4NR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(IEnemy::s_surpriseUFONum) + L"/5";
			m_fontRender->SetText(teststring.c_str());
			

			if (IEnemy::s_surpriseUFONum == 5)
			{
				m_gamescoremanager->m_gameScore += 1500;
				m_isMissionEnd = true;
			}
			break;
		case 7://ミッション4 裏

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission4HR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(IEnemy::s_surpriseFighterNum) + L"/3";
			m_fontRender->SetText(teststring.c_str());
			

			if (IEnemy::s_surpriseFighterNum == 3)
			{
				m_gamescoremanager->m_gameScore += 4000;
				m_isMissionEnd = true;
			}
			break;
		case 8://ミッション5

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission5NR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(m_player->m_highSpeedCount / 60) + L"/10";
			m_fontRender->SetText(teststring.c_str());


			if (m_player->m_highSpeedCount >= 600)
			{
				m_gamescoremanager->m_gameScore += 3000;
				m_isMissionEnd = true;
			}
			break;
		case 9://ミッション5 裏

			if (m_missionSpriteRender == nullptr)
			{
				m_missionSpriteRender = NewGO<prefab::CSpriteRender>(0);
				m_missionSpriteRender->Init(L"sprite/Missions/Mission5HR.dds", 300, 250);
			}

			
			teststring = std::to_wstring(m_player->m_lowSpeedCount / 60) + L"/10";
			m_fontRender->SetText(teststring.c_str());


			if (m_player->m_lowSpeedCount >= 600)
			{
				m_gamescoremanager->m_gameScore += 4000;
				m_isMissionEnd = true;
			}
			break;
		}

		m_missionTimeSpriteRender[0]->SetScale({ max(m_missionTimer - 3150.0f,0.0f) / 450.0f ,1.0f, 1.0f });
		m_missionTimeSpriteRender[1]->SetScale({ 1.0f,min(max(m_missionTimer - 2250.0f,0.0f) / 900.0f,1.0f),1.0f });
		m_missionTimeSpriteRender[2]->SetScale({ min(max(m_missionTimer - 1350.0f,0.0f) / 900.0f,1.0f),1.0f,1.0f });
		m_missionTimeSpriteRender[3]->SetScale({ 1.0f,min(max(m_missionTimer - 450.0f,0.0f) / 900.0f,1.0f),1.0f });
		m_missionTimeSpriteRender[4]->SetScale({ min(max(m_missionTimer,0.0f) / 450.0f,1.0f),1.0f,1.0f });

		if (m_missionTimer / 60 == 30)
		{
			m_missionTimeSpriteRender[0]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[1]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[2]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[3]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[4]->SetMulColor({ 1.0f,1.0f,0.0f,0.7f });
		}
		else if (m_missionTimer / 60 == 10)
		{
			m_missionTimeSpriteRender[0]->SetMulColor({ 1.0f,0.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[1]->SetMulColor({ 1.0f,0.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[2]->SetMulColor({ 1.0f,0.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[3]->SetMulColor({ 1.0f,0.0f,0.0f,0.7f });
			m_missionTimeSpriteRender[4]->SetMulColor({ 1.0f,0.0f,0.0f,0.7f });
		}

		m_missionTimer--;
	}

	m_missionSpriteRender->SetPosition({ -490.0f,-235.0f,0.0f });

	if (m_missionTimer <= 0)
		m_isMissionEnd = true;
}