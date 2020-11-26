#include "stdafx.h"
#include "Blackhole.h"

void Blackhole::OnDestroy()
{
	DeleteGO(m_bhEffect);
}

bool Blackhole::Start()
{
	m_bhEffect = NewGO<prefab::CEffect>(0);
	m_bhEffect->Play(L"effect/test.efk");
	return true;
}

void Blackhole::Update()
{
	if (m_bhEffect->IsPlay() == false)
	{
		m_bhEffect->Play(L"effect/test.efk");
	}
	m_bhEffect->SetScale({ 100.0f,100.0f,100.0f });
	m_bhEffect->SetPosition(m_position);
}