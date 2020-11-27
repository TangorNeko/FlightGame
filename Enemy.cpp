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
	/*
	count++;
	if (count < 100)
	{
		m_position.x += 100;
		m_position.y += 20;
		m_position.z += 50;
	}
	else if (count < 200)
	{
		m_position.x -= 100;
		m_position.y -= 20;
		m_position.z -= 50;
	}
	else
		count = 0;
		*/
	m_skinModelRender->SetPosition(m_position);
}