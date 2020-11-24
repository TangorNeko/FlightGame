#include "stdafx.h"
#include "Enemy.h"

void Enemy::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/target.cmo");
	m_skinModelRender->SetScale({ 100.0f,100.0f,100.0f });
	return true;
}

void Enemy::Update()
{
	m_skinModelRender->SetPosition(m_position);
}