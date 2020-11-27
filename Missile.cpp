#include "stdafx.h"
#include "Missile.h"
#include "Enemy.h"

void Missile::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

bool Missile::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modeldata/testMissile.cmo");
	//m_skinModelRender->SetScale({ 10.0f,10.0f,10.0f });
	return true;
}

void Missile::Update()
{	

	CVector3 diff = m_moveDir = m_trackingEnemy->m_position - m_position;
	m_moveDir.Normalize();
	m_fSpeed += m_fSpeed / 10;
	if (m_fSpeed > 300)
	{
		m_fSpeed = 300;
	}
	m_position += m_moveDir * m_fSpeed;
	m_skinModelRender->SetPosition(m_position);

	if (diff.Length() < 200)
	{
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->SetPosition(m_trackingEnemy->m_position);
		effect->SetScale({ 0.5f,0.5f,0.5f });
		effect->Play(L"effect/fire.efk");
		DeleteGO(m_trackingEnemy);
		DeleteGO(this);
	}
}