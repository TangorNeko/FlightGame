#include "stdafx.h"
#include "EnemyGenerator.h"
#include "LaserEnemy.h"
#include "MissileEnemy.h"
#include "Player.h"
#include <random>

void EnemyGenerator::OnDestroy()
{
	
	QueryGOs<IEnemy>("enemy", [](IEnemy* enemy)->bool
	{
		DeleteGO(enemy);
		return true;
	});
	
}

bool EnemyGenerator::Start()
{
	player = FindGO<Player>("player");

	for (int i = 0; i < 5;)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		CVector3 randomPos = {
			player->m_position.x - 5 + rnd() % 100000,
			player->m_position.y - 5 + rnd() % 100000,
			player->m_position.z - 5 + rnd() % 100000 
		};

		CVector3 diff = player->m_position - randomPos;

		if (diff.Length() > 50000.0f)
		{
			lEnemy = NewGO<LaserEnemy>(0, "enemy");
			lEnemy->m_position = randomPos;

			lEnemy = NewGO<LaserEnemy>(0, "enemy");
			CVector3 groupDistance = { static_cast<float>(rnd() % 3000),static_cast<float>(rnd() % 3000),static_cast<float>(rnd() % 3000) };
			lEnemy->m_position = randomPos + groupDistance;

			lEnemy = NewGO<LaserEnemy>(0, "enemy");
			lEnemy->m_position = randomPos - groupDistance;
			i++;
		}

	}

	for (int i = 0; i < 5;)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		CVector3 randomPos = {
			player->m_position.x - 5 + rnd() % 200000,
			player->m_position.y - 5 + rnd() % 200000,
			player->m_position.z - 5 + rnd() % 200000
		};

		CVector3 diff = player->m_position - randomPos;

		if (diff.Length() > 100000.0f)
		{
			mEnemy = NewGO<MissileEnemy>(0, "enemy");
			mEnemy->m_position = randomPos;
			i++;
		}

	}

	return true;
}

void EnemyGenerator::Update()
{
	if (LaserEnemy::m_laserEnemyNum <= 27)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		CVector3 randomPos = {
			player->m_position.x - 5 + rnd() % 100000,
			player->m_position.y - 5 + rnd() % 100000,
			player->m_position.z - 5 + rnd() % 100000
		};

		CVector3 diff = player->m_position - randomPos;

		if (diff.Length() > 50000.0f)
		{
			lEnemy = NewGO<LaserEnemy>(0, "enemy");
			lEnemy->m_position = randomPos;

			lEnemy = NewGO<LaserEnemy>(0, "enemy");
			CVector3 groupDistance = { static_cast<float>(rnd() % 3000),static_cast<float>(rnd() % 3000),static_cast<float>(rnd() % 3000) };
			lEnemy->m_position = randomPos + groupDistance;

			lEnemy = NewGO<LaserEnemy>(0, "enemy");
			lEnemy->m_position = randomPos - groupDistance;
		}

	}

	if(MissileEnemy::m_missileEnemyNum < 5)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		CVector3 randomPos = {
			player->m_position.x - 5 + rnd() % 200000,
			player->m_position.y - 5 + rnd() % 200000,
			player->m_position.z - 5 + rnd() % 200000
		};

		CVector3 diff = player->m_position - randomPos;

		if (diff.Length() > 100000.0f)
		{
			mEnemy = NewGO<MissileEnemy>(0, "enemy");
			mEnemy->m_position = randomPos;
		}

	}
}