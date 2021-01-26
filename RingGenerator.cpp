#include "stdafx.h"
#include "RingGenerator.h"
#include "Ring.h"
#include "Player.h"
#include "GameScene.h"
#include <random>

void RingGenerator::OnDestroy()
{
	QueryGOs<Ring>("ring", [](Ring* ring)->bool
		{
			DeleteGO(ring);
			return true;
		});
}

bool RingGenerator::Start()
{
	m_player = FindGO<Player>("player");
	m_gameScene = FindGO<GameScene>("gamescene");

	for (int i = 0;i < 100;i++)
	{
		m_ring = NewGO<Ring>(0, "ring");
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		m_ring->m_x = rnd() % 360;
		m_ring->m_y = rnd() % 360;
		int x = rnd() % 100000, y = rnd() % 100000, z = rnd() % 100000;
		m_ring->m_position = { m_ring->m_position.x -50000 + static_cast<float>(x),
							m_ring->m_position.y -50000 + static_cast<float>(y),
							m_ring->m_position.z -50000 + static_cast<float>(z) };
	}

	return true;
}

void RingGenerator::Update()
{
	if (m_gameScene->m_isPlayerDead == false)
	{
		if (Ring::s_ringNum < 100)
		{
			m_ring = NewGO<Ring>(0, "ring");
			std::random_device seed_gen;
			std::mt19937_64 rnd(seed_gen());
			m_ring->m_x = rnd() % 360;
			m_ring->m_y = rnd() % 360;
			int x = rnd() % 100000, y = rnd() % 100000, z = rnd() % 100000;
			m_ring->m_position = { m_player->m_position.x - 50000 + static_cast<float>(x),
								m_player->m_position.y - 50000 + static_cast<float>(y),
								m_player->m_position.z - 50000 + static_cast<float>(z) };
		}
	}
}