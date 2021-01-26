#include "stdafx.h"
#include "LaserEnemy.h"
#include "Player.h"
#include "GameScene.h"
#include <random>

int LaserEnemy::m_laserEnemyNum = 0;

void LaserEnemy::OnDestroy()
{
	//削除される時レーザーが発射されていた時だけ削除(他の場合はもう削除されているため)
	if (m_isShooting == true)
	{
		DeleteGO(m_effect);
		DeleteGO(m_beamSound);
	}

	DeleteGO(m_skinModelRender);
	
	m_laserEnemyNum--;

	if (m_laserEnemyNum < 0)//応急策　ブラックホールフェーズで何故か0以下になることがある
	{
		m_laserEnemyNum = 0;
	}
}

bool LaserEnemy::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/UFOMAPPED.cmo");
	m_skinModelRender->SetScale({ 5.0f,5.0f,5.0f });

	m_specSRV.CreateFromDDSTextureFromFile(L"modelData/Vehicle.fbm/vehicle_spec.dds");
	m_skinModelRender->FindMaterial([&](auto material) {
		material->SetSpecularMap(m_specSRV.GetBody());
		});

	m_targetPlayer = FindGO<Player>("player");
	m_gameScene = FindGO<GameScene>("gamescene");

	m_laserEnemyNum++;

	m_isUFO = true;
	return true;
}

void LaserEnemy::Update()
{
	if (m_gameScene->m_isPlayerDead == false && m_gameScene->m_isBlackholePhase == false)
	{
		CVector3 diff;
		//プレイヤーのとの距離
		diff = m_targetPlayer->m_position - m_position;


		//プレイヤーとの距離15000以内かつまだレーザーを発射していないなら
		if (diff.Length() <= 15000 && m_isShooting == false)
		{
			/*
			m_laser = NewGO<Laser>(0, "laser");
			m_laser->m_owner = this;
			*/
			m_effect = NewGO<prefab::CEffect>(0);
			m_effect->Play(L"effect/Laser.efk");
			m_effect->SetScale({ 25.0f,25.0f,100.0f });

			m_beamSound = NewGO<prefab::CSoundSource>(0);
			m_beamSound->Init(L"sound/Beam.wav");
			m_beamSound->SetVolume(0.5f);
			m_beamSound->Play(true);

			m_isShooting = true;
			m_isDeleted = false;

			//最初に遭遇した時は宇宙人の音を出す
			if (m_isFirstContact == true)
			{
				std::random_device seed_gen;
				std::mt19937_64 rnd(seed_gen());

				prefab::CSoundSource* alienSound = NewGO<prefab::CSoundSource>(0);

				switch (rnd() % 3)
				{
				case 0:
					alienSound->Init(L"sound/UFO1.wav", false);
					break;
				case 1:
					alienSound->Init(L"sound/UFO2.wav",false);
					break;
				case 2:
					alienSound->Init(L"sound/UFO3.wav", false);
					break;
				}
				alienSound->SetPosition(m_position);
				alienSound->SetVolume(0.5f);
				alienSound->Play(false);

				m_isFirstContact = false;
			}

			//ミッション用
			m_isAttackSuccess = true;
		}

		if (m_effect != nullptr)
		{
			//レーザーを出す位置はアンテナからにする
			laserpos.x = m_position.x;
			laserpos.y = m_position.y + 950.0f;
			laserpos.z = m_position.z;

			m_effect->SetPosition(laserpos);

			//レーザーの向きを取得
			CVector3 laserDir = m_targetPlayer->m_position - laserpos;
			laserDir.Normalize();


			CVector3 zx = { laserDir.x,0.0f,laserDir.z };
			zx.Normalize();


			//X軸の回転
			//ドット積の結果が1を超える事があったので1より大きければxの角度をacos(1) = 0度にする
			if (laserDir.Dot(zx) > 1)
			{
				laserdegx = 0.0f;
			}
			else
			{
				laserdegx = acos(laserDir.Dot(zx));
			}

			//レーザーの角度しか分からないのでレーザーの向きが上向きなら角度をマイナスにする
			if (laserDir.y > 0)
				laserdegx = -laserdegx;

			//前フレームからの差分の回転をセット
			m_laserqRotX.SetRotation(CVector3::AxisX, laserdegx - laserodegx);


			//Y軸の回転
			laserdegy = acos(CVector3::Front.Dot(zx));

			//レーザーの角度しか分からないのでレーザーの向きが左向きなら角度をマイナスにする
			if (laserDir.x < 0)
				laserdegy = -laserdegy;

			//前フレームからの差分の回転をセット
			m_laserqRotY.SetRotation(CVector3::AxisY, laserdegy - laserodegy);


			m_laserRotation.Multiply(m_laserqRotX, m_laserRotation);
			m_laserRotation.Multiply(m_laserqRotY);

			m_effect->SetRotation(m_laserRotation);

			laserodegx = laserdegx;
			laserodegy = laserdegy;
		}

		//プレイヤーとの距離が15000以上離れていて、かつレーザーを発射していたら
		if (diff.Length() > 15000 && m_isShooting == true && m_effect != nullptr)
		{
			DeleteGO(m_effect);
			DeleteGO(m_beamSound);
			m_effect = nullptr;

			m_isShooting = false;
			m_isDeleted = true;
		}

		//レーザーのダメージを付与
		if (m_isShooting == true)
		{
			m_damageCount++;
			if (m_damageCount == 10)
			{
				m_targetPlayer->m_hp--;
				m_damageCount = 0;
				m_isAttackSuccess = true;
			}
		}

		//敵の動き

		m_moveDir = m_targetPlayer->m_position - m_position;
		m_moveDir.Normalize();

		if (diff.Length() < 10000.0f)
		{
			m_moveDir = m_oldMoveDir;
		}

		m_oldMoveDir = m_moveDir;


		CVector3 movezx = { m_moveDir.x,0.0f,m_moveDir.z };
		movezx.Normalize();

		//x軸の回転


		if (m_moveDir.Dot(movezx) > 1)
		{
			degx = 0.0f;
		}
		else
		{
			degx = acos(m_moveDir.Dot(movezx));
		}


		//角度しか手に入らないので向いてる方向が下なら負の数にする
		if (m_moveDir.y > 0)
			degx = -degx;

		m_qRotX.SetRotation(CVector3::AxisX, degx - odegx);

		//y軸の回転
		degy = acos(CVector3::Front.Dot(movezx));
		//角度しか手に入らないので向いてる方向が左なら負の数にする
		if (m_moveDir.x < 0)
			degy = -degy;

		m_qRotY.SetRotation(CVector3::AxisY, degy - odegy);

		m_rotation.Multiply(m_qRotX, m_rotation);
		m_rotation.Multiply(m_qRotY, m_rotation);

		auto mRot = CMatrix::Identity;
		mRot.MakeRotationFromQuaternion(m_rotation);

		odegx = degx;
		odegy = degy;


		m_rightDir.x = mRot.m[0][0];
		m_rightDir.y = mRot.m[0][1];
		m_rightDir.z = mRot.m[0][2];

		m_upDir.x = mRot.m[1][0];
		m_upDir.y = mRot.m[1][1];
		m_upDir.z = mRot.m[1][2];

		//不規則に動く
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		if (rnd() % 50 == 0)
		{
			m_movingRight = !m_movingRight;
		}

		if (rnd() % 50 == 0)
		{
			m_movingUp = !m_movingUp;
		}

		//距離20000以内に近づくとレーザーを撃ちながら近づいてくる
		if (diff.Length() < 20000)
			m_position += m_moveDir * 50;

		if (m_movingRight)
		{
			m_position += m_rightDir * 25;
		}
		else
		{
			m_position += m_rightDir * -25;
		}

		if (m_movingUp)
		{
			m_position += m_upDir * 25;
		}
		else
		{
			m_position += m_upDir * -25;
		}

		//自機から100000より離れると削除する
		if (diff.Length() > 100000)
		{
			DeleteGO(this);
		}
	}
	else
	{
		if (m_isShooting == true)
		{
			DeleteGO(m_effect);
			DeleteGO(m_beamSound);
			m_effect = nullptr;
			m_isShooting = false;
		}

	std::random_device seed_gen;
	std::mt19937_64 rnd(seed_gen());
	if (rnd() % 50 == 0)
		{
			m_movingRight = !m_movingRight;
		}
	
	if (rnd() % 50 == 0)
		{
			m_movingUp = !m_movingUp;
		}

	if (m_movingRight)
	{
		m_position.x += 25;
	}
	else
	{
		m_position.x += -25;
	}

	if (m_movingUp)
	{
		m_position.y += 25;
	}
	else
	{
		m_position.y += -25;
	}
	}

	m_skinModelRender->SetPosition(m_position);
}