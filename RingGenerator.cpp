#include "stdafx.h"
#include "RingGenerator.h"
#include "Ring.h"
#include "Player.h"
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
	player = FindGO<Player>("player");

	for (int i = 0;i < 100;i++)
	{
		ring = NewGO<Ring>(0, "ring");
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		ring->m_x = rnd() % 360;
		ring->m_y = rnd() % 360;
		int x = rnd() % 50000, y = rnd() % 50000, z = rnd() % 50000;
		ring->m_position = {player->m_position.x -25000 + static_cast<float>(x),
							player->m_position.y -25000 + static_cast<float>(y),
							player->m_position.z -25000 + static_cast<float>(z) };
	}

	return true;
}

void RingGenerator::Update()
{
	if (Ring::s_ringNum < 100)
	{
		ring = NewGO<Ring>(0, "ring");
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		ring->m_x = rnd() % 360;
		ring->m_y = rnd() % 360;
		int x = rnd() % 50000, y = rnd() % 50000, z = rnd() % 50000;
		ring->m_position = {player->m_position.x -25000 + static_cast<float>(x),
							player->m_position.y -25000 + static_cast<float>(y),
							player->m_position.z -25000 + static_cast<float>(z) };
	}
}