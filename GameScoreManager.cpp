#include "stdafx.h"
#include "GameScoreManager.h"

void GameScoreManager::OnDestroy()
{
	DeleteGO(m_scoreFontRender);
}

bool GameScoreManager::Start()
{
	m_scoreFontRender = NewGO<prefab::CFontRender>(0);
	m_scoreFontRender->SetPivot({ 1.0f,1.0f });
	m_scoreFontRender->SetPosition({ 620.0f,355.0f });
	m_scoreFontRender->SetShadowParam(true, 1.0f, { 0.0f,0.0f,0.5f,1.0f });
	return true;
}

void GameScoreManager::Update()
{
	Score = L"Score:" + std::to_wstring(m_gameScore);
	string = Score.c_str();
	m_scoreFontRender->SetText(string);
}