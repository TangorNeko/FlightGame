#include "stdafx.h"
#include "LaserEnemy.h"
#include "Player.h"
#include "GameScene.h"
#include <random>

int LaserEnemy::m_laserEnemyNum = 0;

void LaserEnemy::OnDestroy()
{
	//�폜����鎞���[�U�[�����˂���Ă����������폜(���̏ꍇ�͂����폜����Ă��邽��)
	if (m_isShooting == true)
	{
		DeleteGO(m_effect);
		DeleteGO(m_beamSound);
	}

	DeleteGO(m_skinModelRender);
	
	m_laserEnemyNum--;

	if (m_laserEnemyNum < 0)//���}��@�u���b�N�z�[���t�F�[�Y�ŉ��̂�0�ȉ��ɂȂ邱�Ƃ�����
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
		//�v���C���[�̂Ƃ̋���
		diff = m_targetPlayer->m_position - m_position;


		//�v���C���[�Ƃ̋���15000�ȓ����܂����[�U�[�𔭎˂��Ă��Ȃ��Ȃ�
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

			//�ŏ��ɑ����������͉F���l�̉����o��
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

			//�~�b�V�����p
			m_isAttackSuccess = true;
		}

		if (m_effect != nullptr)
		{
			//���[�U�[���o���ʒu�̓A���e�i����ɂ���
			laserpos.x = m_position.x;
			laserpos.y = m_position.y + 950.0f;
			laserpos.z = m_position.z;

			m_effect->SetPosition(laserpos);

			//���[�U�[�̌������擾
			CVector3 laserDir = m_targetPlayer->m_position - laserpos;
			laserDir.Normalize();


			CVector3 zx = { laserDir.x,0.0f,laserDir.z };
			zx.Normalize();


			//X���̉�]
			//�h�b�g�ς̌��ʂ�1�𒴂��鎖���������̂�1���傫�����x�̊p�x��acos(1) = 0�x�ɂ���
			if (laserDir.Dot(zx) > 1)
			{
				laserdegx = 0.0f;
			}
			else
			{
				laserdegx = acos(laserDir.Dot(zx));
			}

			//���[�U�[�̊p�x����������Ȃ��̂Ń��[�U�[�̌�����������Ȃ�p�x���}�C�i�X�ɂ���
			if (laserDir.y > 0)
				laserdegx = -laserdegx;

			//�O�t���[������̍����̉�]���Z�b�g
			m_laserqRotX.SetRotation(CVector3::AxisX, laserdegx - laserodegx);


			//Y���̉�]
			laserdegy = acos(CVector3::Front.Dot(zx));

			//���[�U�[�̊p�x����������Ȃ��̂Ń��[�U�[�̌������������Ȃ�p�x���}�C�i�X�ɂ���
			if (laserDir.x < 0)
				laserdegy = -laserdegy;

			//�O�t���[������̍����̉�]���Z�b�g
			m_laserqRotY.SetRotation(CVector3::AxisY, laserdegy - laserodegy);


			m_laserRotation.Multiply(m_laserqRotX, m_laserRotation);
			m_laserRotation.Multiply(m_laserqRotY);

			m_effect->SetRotation(m_laserRotation);

			laserodegx = laserdegx;
			laserodegy = laserdegy;
		}

		//�v���C���[�Ƃ̋�����15000�ȏ㗣��Ă��āA�����[�U�[�𔭎˂��Ă�����
		if (diff.Length() > 15000 && m_isShooting == true && m_effect != nullptr)
		{
			DeleteGO(m_effect);
			DeleteGO(m_beamSound);
			m_effect = nullptr;

			m_isShooting = false;
			m_isDeleted = true;
		}

		//���[�U�[�̃_���[�W��t�^
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

		//�G�̓���

		m_moveDir = m_targetPlayer->m_position - m_position;
		m_moveDir.Normalize();

		if (diff.Length() < 10000.0f)
		{
			m_moveDir = m_oldMoveDir;
		}

		m_oldMoveDir = m_moveDir;


		CVector3 movezx = { m_moveDir.x,0.0f,m_moveDir.z };
		movezx.Normalize();

		//x���̉�]


		if (m_moveDir.Dot(movezx) > 1)
		{
			degx = 0.0f;
		}
		else
		{
			degx = acos(m_moveDir.Dot(movezx));
		}


		//�p�x������ɓ���Ȃ��̂Ō����Ă���������Ȃ畉�̐��ɂ���
		if (m_moveDir.y > 0)
			degx = -degx;

		m_qRotX.SetRotation(CVector3::AxisX, degx - odegx);

		//y���̉�]
		degy = acos(CVector3::Front.Dot(movezx));
		//�p�x������ɓ���Ȃ��̂Ō����Ă���������Ȃ畉�̐��ɂ���
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

		//�s�K���ɓ���
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

		//����20000�ȓ��ɋ߂Â��ƃ��[�U�[�������Ȃ���߂Â��Ă���
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

		//���@����100000��藣���ƍ폜����
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