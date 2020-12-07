#include "stdafx.h"
#include "Blackhole.h"
#include "Player.h"

void Blackhole::OnDestroy()
{
	DeleteGO(m_bhEffect);
}

bool Blackhole::Start()
{
	m_bhEffect = NewGO<prefab::CEffect>(0);
	m_bhEffect->Play(L"effect/blackhole.efk");

	player = FindGO<Player>("player");

	m_scaling.x = 1000.0f;
	m_scaling.y = 1000.0f;
	m_scaling.z = 1000.0f;

	bhradius = 700.0f;
	return true;
}

void Blackhole::Update()
{

	if (m_bhEffect->IsPlay() == false)
	{
		m_bhEffect->Play(L"effect/blackhole.efk");
	}
	m_bhEffect->SetScale(m_scaling);
	m_bhEffect->SetPosition(m_position);

	CVector3 rad = { 0.0f,0.0f,bhradius };
	dbg::DrawVector(rad,m_position,"bhradius");

	CVector3 pdiff = player->m_position - m_position;

	if (pdiff.Length() < bhradius)
	{
		float a = pdiff.Length();
		pdiff.Normalize();
		player->m_position -= pdiff * 100;
	}
	
	if (isExpanding) {
		m_scaling.x += 150.0f;
		m_scaling.y += 150.0f;
		m_scaling.z += 150.0f;

		bhradius += 105.0f;
	}
	else
	{
		m_scaling.x -= expire;
		m_scaling.y -= expire;
		m_scaling.z -= expire;

		bhradius -= 0.7 * expire;

		expire *= 1.01f;
	}

	if (bhradius < 60.0f)
	{
		DeleteGO(this);
	}

	if (count == 1800)
	{
		isExpanding = false;
	}

	count++;
	
}