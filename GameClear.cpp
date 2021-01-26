#include "stdafx.h"
#include "GameClear.h"
#include "Player.h"
#include "GameScoreManager.h"
#include "GameScene.h"
#include "SceneChange.h"

void GameClear::OnDestroy()
{
	DeleteGO(m_fontRender);
	m_gameScene->m_isGameEnd = true;
}

bool GameClear::Start()
{
	m_player = FindGO<Player>("player");
	m_gameScoreManager = FindGO<GameScoreManager>("gamescoremanager");
	m_gameScene = FindGO<GameScene>("gamescene");

	m_hpBonus = m_player->m_hp * 2;
	m_fuelBonus = (1000 - m_player->m_fuel) * 2;
	m_totalScore = m_gameScoreManager->m_gameScore;

	m_fontRender = NewGO<prefab::CFontRender>(3);
	m_fontRender->SetPivot({ 0.0f,1.0f });
	m_fontRender->SetPosition({ -200.0f,100.0f });
	m_fontRender->SetShadowParam(true, 1.0f, { 0.0f,0.0f,0.5f,1.0f });

	return true;
}

void GameClear::PostUpdate()
{

	if (m_timeCount < 120)
	{

	}
	else if (m_timeCount < 240)
	{
		m_string = L"Mission Complete!";
		m_fontRender->SetText(m_string.c_str());
	}
	else if (m_timeCount < 360)
	{
		m_string = L"Mission Complete!\nScore = " + std::to_wstring(m_gameScoreManager->m_gameScore);
		m_fontRender->SetText(m_string.c_str());
	}
	else if (m_timeCount < 480)
	{
		m_string = L"Mission Complete!\nScore = " + std::to_wstring(m_gameScoreManager->m_gameScore)
					+ L"\nHP Bonus = " + std::to_wstring(m_hpBonus);
		m_fontRender->SetText(m_string.c_str());
	}
	else if (m_timeCount < 600)
	{
		m_string = L"Mission Complete!\nScore = " + std::to_wstring(m_gameScoreManager->m_gameScore)
			+ L"\nHP Bonus = " + std::to_wstring(m_hpBonus)
			+ L"\nHunger Point = " + std::to_wstring(m_fuelBonus);
		m_fontRender->SetText(m_string.c_str());
	}
	else
	{
		m_string = L"Mission Complete!\nScore = " + std::to_wstring(m_gameScoreManager->m_gameScore)
			+ L"\nHP Bonus = " + std::to_wstring(m_hpBonus)
			+ L"\nHunger Point = " + std::to_wstring(m_fuelBonus)
			+ L"\nTotalScore = " + std::to_wstring(m_totalScore);

		if (m_hpBonus > 0)
		{
			if (m_hpBonus > m_fuelBonus && m_hpBonus != 1)
			{
				m_hpBonus -= 2;
				m_totalScore += 2;
			}
			else
			{
				m_hpBonus--;
				m_totalScore++;
			}
		}

		if (m_fuelBonus > 0)
		{
			if (m_fuelBonus > m_hpBonus && m_fuelBonus != 1)
			{
				m_fuelBonus -= 2;
				m_totalScore += 2;
			}
			else
			{
				m_fuelBonus--;
				m_totalScore++;
			}
		}
		

		if (m_scoreCountSound == nullptr)
		{
			m_scoreCountSound = NewGO<prefab::CSoundSource>(0);
			m_scoreCountSound->Init(L"sound/ScoreCountLoop.wav");
			m_scoreCountSound->SetVolume(0.25f);
			m_scoreCountSound->Play(true);
		}

		m_fontRender->SetText(m_string.c_str());
	}
	
	if (m_hpBonus == 0 && m_fuelBonus == 0 && m_timeCount < 5000)
	{
		DeleteGO(m_scoreCountSound);
		m_scoreCountSound = NewGO<prefab::CSoundSource>(0);
		m_scoreCountSound->Init(L"sound/ScoreCountLoopEnd.wav");
		m_scoreCountSound->SetVolume(0.25f);
		m_scoreCountSound->Play(false);

		m_timeCount = 5000;
	}

	if (m_timeCount == 5290)
	{
		NewGO<SceneChange>(0);
	}

	if (m_timeCount == 5300)
		DeleteGO(this);

	m_timeCount++;
}