#include "stdafx.h"
#include "Blackhole.h"
#include "Player.h"
#include "IEnemy.h"
#include "RingGenerator.h"
#include "EnemyGenerator.h"
#include "GameClear.h"
#include "GameOver.h"
#include "GameScene.h"
#include "GameScoreManager.h"
#include "GameCamera.h"
#include <random>
#include "MissileEnemy.h"

void Blackhole::OnDestroy()
{
	
	if(m_bhEffect != nullptr && m_bhEffect->IsPlay() == true)
		DeleteGO(m_bhEffect);

	QueryGOs<IEnemy>("enemy", [](IEnemy* enemy)->bool {
		DeleteGO(enemy);
		return true;
		});

	m_gameScene->m_isBlackholePhase = false;

}

bool Blackhole::Start()
{
	
	DeleteGO(FindGO<RingGenerator>("ringgenerator"));

	player = FindGO<Player>("player");

	m_scaling.x = 1000.0f;
	m_scaling.y = 1000.0f;
	m_scaling.z = 1000.0f;

	m_gameScene = FindGO<GameScene>("gamescene");
	m_gameScoreManager = FindGO<GameScoreManager>("gamescoremanager");

	bhradius = 700.0f;

	m_blackHoleDistanceBar = NewGO<prefab::CSpriteRender>(0);
	m_blackHoleDistanceBar->Init(L"sprite/BlackholeDistanceBar.dds", 1120.0f, 48.0f);
	m_blackHoleDistanceBar->SetPosition({ 0.0f,-300.0f,0.0f });

	
	GeneratePanel();

	m_blackHolePlayer = NewGO<prefab::CSpriteRender>(0);
	m_blackHolePlayer->Init(L"sprite/BlackHolePhasePlayer.dds", 271.0f, 271.0f);
	m_blackHolePlayer->SetScale({ 0.25f,0.25f,1.0f });
	m_blackHolePlayer->SetPosition({ -320.0f,-245.0f,0.0f });

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	return true;
}

void Blackhole::Update()
{

	//�u���b�N�z�[���G�t�F�N�g���Đ�����Ă��Ȃ��Ȃ�Đ�(�����I�ȃ��[�v)
	if (m_isPulling == false)
	{
		m_bhEffect = NewGO<prefab::CEffect>(0);
		m_bhEffect->Play(L"effect/Blackhole.efk");

		m_isPulling = true;
	}

	if (m_bhEffect != nullptr && m_isPulling == true)
	{
		m_bhEffect->SetScale(m_scaling);
		m_bhEffect->SetPosition(m_position);


		/*
		CVector3 rad = { 0.0f,0.0f,bhradius };
		dbg::DrawVector(rad, m_position, "bhradius");
		*/

		//�z�����ޗp�Ƀv���C���[����u���b�N�z�[���̒��S�܂ł̋������擾
		CVector3 pdiff = player->m_position - m_position;


		//�{�[�i�X�_���p
		if (m_blackHolePlayer) {
			m_blackHolePlayer->SetPosition({ -400.0f + (pdiff.Length() - bhradius) / 25,-245.0f,0.0f });
		}
		if (count > 180 && count % 120 == 0 && isExpanding == true && m_gameScene->m_isPlayerDead == false)
		{
			if ((pdiff.Length() - bhradius) / 25 < 0)
			{

			}
			else if ((pdiff.Length() - bhradius) / 25 < 160)
			{
				GetBonus(m_blackHoleBonusPanelNum[0]);
			}
			else if ((pdiff.Length() - bhradius) / 25 < 320)
			{
				GetBonus(m_blackHoleBonusPanelNum[1]);
			}
			else if ((pdiff.Length() - bhradius) / 25 < 480)
			{
				GetBonus(m_blackHoleBonusPanelNum[2]);
			}
			else if ((pdiff.Length() - bhradius) / 25 < 640)
			{
				GetBonus(m_blackHoleBonusPanelNum[3]);
			}
			else if ((pdiff.Length() - bhradius) / 25 < 800)
			{
				GetBonus(m_blackHoleBonusPanelNum[4]);
			}

			
			DeleteGOs("panel");
			GeneratePanel();

			if (count % 240 == 0)
			{
				std::random_device seed_gen;
				std::mt19937_64 rnd(seed_gen());

				m_gameCamera->m_blackholeCameraScene = rnd() % 4;
				m_gameCamera->m_isCameraChangeFrame = true;
			}
			
		}


		//�u���b�N�z�[���̔��a��7���Ƀv���C���[���߂Â��ƃQ�[���I�[�o�[
		if (pdiff.Length() < bhradius * 0.7f && m_gameScene->m_isPlayerDead == false)
		{
			prefab::CEffect* deadEffect = NewGO<prefab::CEffect>(0);
			deadEffect->SetPosition(player->m_position + MainCamera().GetForward() * 100);
			deadEffect->SetScale({ 100.0f,100.0f,100.0f });
			deadEffect->Play(L"effect/Explosion.efk");

			NewGO<GameOver>(0, "gameover");

			DeleteGO(player);

			DeleteGO(m_blackHoleDistanceBar);
			DeleteGO(m_blackHolePlayer);
			DeleteGOs("panel");
		}


		//�u���b�N�z�[���̔��a���v���C���[�Ƃ̋����̕����߂���΁A���t���[��200�̑����Ńu���b�N�z�[���̒��S�ɋz���񂹂�
		if (pdiff.Length() < bhradius)
		{
			float a = pdiff.Length();
			pdiff.Normalize();
			player->m_position -= pdiff * 200;
		}

		
		QueryGOs<IEnemy>("enemy", [=](IEnemy* enemy)->bool
			{
				CVector3 ediff = enemy->m_position - m_position;

				if (ediff.Length() < bhradius)
				{
					/*
					std::random_device seed_gen;
					std::mt19937_64 rnd(seed_gen());
					if (rnd() % 5 == 0)
					{
						prefab::CEffect* deadEffect = NewGO<prefab::CEffect>(0);
						deadEffect->SetPosition(enemy->m_position + MainCamera().GetForward() * 100);
						deadEffect->SetScale({ 100.0f,100.0f,100.0f });
						deadEffect->Play(L"effect/Explosion.efk");
					}
					*/
					
					DeleteGO(enemy);

				}

				ediff.Normalize();
				enemy->m_position -= ediff * 100;
				return true;
			});
		

		//�t���O���g�����u���b�N�z�[���̊g�又��
		//isExpanding(�g�咆)��true�Ȃ�A���t���[���G�t�F�N�g�̊g�嗦��150,���a��105����(�g�嗦:���a = 10 : 7)
		if (isExpanding) {
			m_scaling.x += 150.0f;
			m_scaling.y += 150.0f;
			m_scaling.z += 150.0f;

			bhradius += 105.0f;
		}
		else //�g�咆�łȂ�(�k����)�Ȃ珙�X�ɏk�����x�𑝂��Ȃ���k������
		{
			m_scaling.x -= expire;
			m_scaling.y -= expire;
			m_scaling.z -= expire;

			bhradius -= 0.7 * expire;

			expire *= 1.01f;
		}

		if (count == 1300)
		{
			EnemyGenerator* enemygenerator = FindGO<EnemyGenerator>("enemygenerator");
			DeleteGO(enemygenerator);
		}

		//1800�t���[���܂Ŋg�債���̌�k������
		if (count == 1800 && isExpanding == true)
		{
			isExpanding = false;
		}

		if (isExpanding == false)
		{
			m_gameCamera->m_blackholeCameraScene = 1;

			m_panelAlpha -= 0.01f;

			if (m_panelAlpha <= 0)
			{
				DeleteGO(m_blackHoleDistanceBar);
				m_blackHoleDistanceBar = nullptr;
				DeleteGO(m_blackHolePlayer);
				m_blackHolePlayer = nullptr;
				DeleteGOs("panel");
			}
			else
			{
				m_blackHoleDistanceBar->SetMulColor({ 1.0f,1.0f,1.0f,m_panelAlpha });
				m_blackHoleBonus[0]->SetMulColor({ 1.0f,1.0f,1.0f,m_panelAlpha });
				m_blackHoleBonus[1]->SetMulColor({ 1.0f,1.0f,1.0f,m_panelAlpha });
				m_blackHoleBonus[2]->SetMulColor({ 1.0f,1.0f,1.0f,m_panelAlpha });
				m_blackHoleBonus[3]->SetMulColor({ 1.0f,1.0f,1.0f,m_panelAlpha });
				m_blackHoleBonus[4]->SetMulColor({ 1.0f,1.0f,1.0f,m_panelAlpha });
				m_blackHolePlayer->SetMulColor({ 1.0f,1.0f,1.0f,m_panelAlpha });
			}
		}

		count++;

	}
	

	//�k���̌��ʃu���b�N�z�[���̔��a��60�ȉ��ɂȂ�΍폜
	if (bhradius < 60.0f)
	{
		DeleteGO(m_bhEffect);
		m_bhEffect = nullptr;

		if (m_gameScene->m_isPlayerDead == false)
		{
			NewGO<GameClear>(2, "gameclear");
		}

		DeleteGO(this);
	}
	
}

void Blackhole::GeneratePanel()
{
	std::random_device seed_gen;
	std::mt19937_64 rnd(seed_gen());

	
	m_blackHoleBonus[0] = NewGO<prefab::CSpriteRender>(0, "panel");

	m_blackHoleBonusPanelNum[0] = rnd() % 4;

	switch (m_blackHoleBonusPanelNum[0])
	{
	case 0:
		m_blackHoleBonus[0]->Init(L"sprite/BlackholePhaseFuel.dds", 160.0f, 45.0f);
			break;
	case 1:
		m_blackHoleBonus[0]->Init(L"sprite/BlackholePhase100.dds", 160.0f, 45.0f);
		break;
	case 2:
		m_blackHoleBonus[0]->Init(L"sprite/BlackholePhase200.dds", 160.0f, 45.0f);
		break;
	case 3:
		m_blackHoleBonus[0]->Init(L"sprite/BlackholePhase300.dds", 160.0f, 45.0f);
		break;
	}
	
	m_blackHoleBonus[0]->SetPosition({ -320.0f,-277.0f,0.0f });


	m_blackHoleBonus[1] = NewGO<prefab::CSpriteRender>(0, "panel");

	m_blackHoleBonusPanelNum[1] = rnd() % 4;

	switch (m_blackHoleBonusPanelNum[1])
	{
	case 0:
		m_blackHoleBonus[1]->Init(L"sprite/BlackholePhaseFuel.dds", 160.0f, 45.0f);
		break;
	case 1:
		m_blackHoleBonus[1]->Init(L"sprite/BlackholePhase100.dds", 160.0f, 45.0f);
		break;
	case 2:
		m_blackHoleBonus[1]->Init(L"sprite/BlackholePhase200.dds", 160.0f, 45.0f);
		break;
	case 3:
		m_blackHoleBonus[1]->Init(L"sprite/BlackholePhase300.dds", 160.0f, 45.0f);
		break;
	}

	m_blackHoleBonus[1]->SetPosition({ -160.0f,-277.0f,0.0f });


	m_blackHoleBonus[2] = NewGO<prefab::CSpriteRender>(0, "panel");

	m_blackHoleBonusPanelNum[2] = rnd() % 4;

	switch (m_blackHoleBonusPanelNum[2])
	{
	case 0:
		m_blackHoleBonus[2]->Init(L"sprite/BlackholePhaseFuel.dds", 160.0f, 45.0f);
		break;
	case 1:
		m_blackHoleBonus[2]->Init(L"sprite/BlackholePhase100.dds", 160.0f, 45.0f);
		break;
	case 2:
		m_blackHoleBonus[2]->Init(L"sprite/BlackholePhase200.dds", 160.0f, 45.0f);
		break;
	case 3:
		m_blackHoleBonus[2]->Init(L"sprite/BlackholePhase300.dds", 160.0f, 45.0f);
		break;
	}

	m_blackHoleBonus[2]->SetPosition({ 0.0f,-277.0f,0.0f });


	m_blackHoleBonus[3] = NewGO<prefab::CSpriteRender>(0, "panel");

	m_blackHoleBonusPanelNum[3] = rnd() % 4;

	switch (m_blackHoleBonusPanelNum[3])
	{
	case 0:
		m_blackHoleBonus[3]->Init(L"sprite/BlackholePhaseFuel.dds", 160.0f, 45.0f);
		break;
	case 1:
		m_blackHoleBonus[3]->Init(L"sprite/BlackholePhase100.dds", 160.0f, 45.0f);
		break;
	case 2:
		m_blackHoleBonus[3]->Init(L"sprite/BlackholePhase200.dds", 160.0f, 45.0f);
		break;
	case 3:
		m_blackHoleBonus[3]->Init(L"sprite/BlackholePhase300.dds", 160.0f, 45.0f);
		break;
	}

	m_blackHoleBonus[3]->SetPosition({ 160.0f,-277.0f,0.0f });

	m_blackHoleBonus[4] = NewGO<prefab::CSpriteRender>(0, "panel");

	m_blackHoleBonusPanelNum[4] = rnd() % 4;

	switch (m_blackHoleBonusPanelNum[4])
	{
	case 0:
		m_blackHoleBonus[4]->Init(L"sprite/BlackholePhaseFuel.dds", 160.0f, 45.0f);
		break;
	case 1:
		m_blackHoleBonus[4]->Init(L"sprite/BlackholePhase100.dds", 160.0f, 45.0f);
		break;
	case 2:
		m_blackHoleBonus[4]->Init(L"sprite/BlackholePhase200.dds", 160.0f, 45.0f);
		break;
	case 3:
		m_blackHoleBonus[4]->Init(L"sprite/BlackholePhase300.dds", 160.0f, 45.0f);
		break;
	}

	m_blackHoleBonus[4]->SetPosition({ 320.0f,-277.0f,0.0f });
}

void Blackhole::GetBonus(int panelNum)
{

	prefab::CSoundSource* m_panelGetSound = NewGO<prefab::CSoundSource>(0);

	switch (panelNum)
	{
	case 0:
		player->m_fuel += 100.0f;
		if (player->m_fuel > 1000)
		{
			player->m_fuel = 1000;
		}

		m_panelGetSound->Init(L"sound/GetFuel.wav");
		m_panelGetSound->SetVolume(0.25f);
		m_panelGetSound->Play(false);
		break;
	case 1:
		m_gameScoreManager->m_gameScore += 100.0f;

		m_panelGetSound->Init(L"sound/GetScore.wav");
		m_panelGetSound->SetVolume(0.25f);
		m_panelGetSound->Play(false);
		break;
	case 2:
		m_gameScoreManager->m_gameScore += 200.0f;

		m_panelGetSound->Init(L"sound/GetScore.wav");
		m_panelGetSound->SetVolume(0.25f);
		m_panelGetSound->Play(false);
		break;
	case 3:
		m_gameScoreManager->m_gameScore += 300.0f;

		m_panelGetSound->Init(L"sound/GetScore.wav");
		m_panelGetSound->SetVolume(0.25f);
		m_panelGetSound->Play(false);
		break;
	}
}