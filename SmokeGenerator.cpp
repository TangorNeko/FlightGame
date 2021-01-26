#include "stdafx.h"
#include "SmokeGenerator.h"
#include "Player.h"
#include "Smoke.h"

const int NUM_SMOKE = 10;
void SmokeGenerator::OnDestroy()
{
	DeleteGOs("smoke");
}

bool SmokeGenerator::Start()
{
	m_player = FindGO<Player>("player");

	for (int i = 0;i < NUM_SMOKE;i++)
	{
		m_smoke = NewGO<Smoke>(0, "smoke");
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		int x = rnd() % 1000, y = rnd() % 1000, z = rnd() % 5000;
		/*
		m_smoke->m_position = { m_player->m_position.x - 500 + static_cast<float>(x),
							m_player->m_position.y - 500 + static_cast<float>(y),
							m_player->m_position.z - 500 + static_cast<float>(z) };
		*/
		
		m_smoke->m_position = { m_player->m_position +
			m_player->m_rightDir * (-500 + x) +
			m_player->m_upDir * (-500 + y) +
			m_player->m_moveDir * (-2500 + z) };
			
	}

	return true;
}

void SmokeGenerator::Update()
{
	if (Smoke::s_smokeNum < NUM_SMOKE)
	{
		m_smoke = NewGO<Smoke>(0, "smoke");
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		int x = rnd() % 1000, y = rnd() % 1000, z = rnd() % 5000;
		/*
		m_smoke->m_position = { m_player->m_position.x - 500 + static_cast<float>(x),
							m_player->m_position.y - 500 + static_cast<float>(y),
							m_player->m_position.z - 500 + static_cast<float>(z) };
							*/

		m_smoke->m_position = { m_player->m_position +
			m_player->m_rightDir * (-500 + x) +
			m_player->m_upDir * (-500 + y) +
			m_player->m_moveDir * (-2500 + z) };
	}
}
