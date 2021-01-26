#include "stdafx.h"
#include "EnemyGenerator.h"
#include "LaserEnemy.h"
#include "MissileEnemy.h"
#include "Player.h"
#include "GameScene.h"
#include <random>

void EnemyGenerator::OnDestroy()
{
	
}

bool EnemyGenerator::Start()
{
	player = FindGO<Player>("player");
	gameScene = FindGO<GameScene>("gamescene");

	for (int i = 0; i < 5;)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		CVector3 randomPos = {
			player->m_position.x - 50000.0f + rnd() % 100000,
			player->m_position.y - 50000.0f + rnd() % 100000,
			player->m_position.z - 50000.0f + rnd() % 100000 
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

	for (int i = 0; i < 3;)
	{
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		CVector3 randomPos = {
			player->m_position.x - 100000.0f + rnd() % 200000,
			player->m_position.y - 25000.0f + rnd() % 50000,
			player->m_position.z - 100000.0f + rnd() % 200000
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

	if (gameScene->m_isPlayerDead == false)
	{
		if (LaserEnemy::m_laserEnemyNum <= 12)
		{
			std::random_device seed_gen;
			std::mt19937_64 rnd(seed_gen());
			CVector3 randomPos = {
				player->m_position.x - 50000.0f + rnd() % 100000,
				player->m_position.y - 50000.0f + rnd() % 100000,
				player->m_position.z - 50000.0f + rnd() % 100000
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

		if (MissileEnemy::m_missileEnemyNum < 3)
		{
			std::random_device seed_gen;
			std::mt19937_64 rnd(seed_gen());
			CVector3 randomPos = {
				player->m_position.x - 100000.0f + rnd() % 200000,
				player->m_position.y - 10000.0f + rnd() % 20000,
				player->m_position.z - 100000.0f + rnd() % 200000
			};

			CVector3 diff = player->m_position - randomPos;

			if (diff.Length() > 100000.0f)
			{
				mEnemy = NewGO<MissileEnemy>(0, "enemy");
				mEnemy->m_position = randomPos;
			}

		}
	}
}