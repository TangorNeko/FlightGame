#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"

void Enemy::OnDestroy()
{
	//DeleteGO(m_laser);

	//�폜����鎞���[�U�[�����˂���Ă����������폜(���̏ꍇ�͂����폜����Ă��邽��)
	if (m_isShooting == true)
	 DeleteGO(m_effect);

	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/UFOMAPPED.cmo");
	m_skinModelRender->SetScale({ 5.0f,5.0f,5.0f });
	
	m_specSRV.CreateFromDDSTextureFromFile(L"modelData/Vehicle.fbm/vehicle_spec.dds");
	m_skinModelRender->FindMaterial([&](auto material) {
		material->SetSpecularMap(m_specSRV.GetBody());
		});

	m_targetPlayer = FindGO<Player>("player");

	return true;
}

void Enemy::Update()
{
	
	CVector3 diff;
	//�v���C���[�̂Ƃ̋���
	diff = m_targetPlayer->m_position - m_position;


	//�v���C���[�Ƃ̋���20000�ȓ����܂����[�U�[�𔭎˂��Ă��Ȃ��Ȃ�
	if (diff.Length() <= 20000 && m_isShooting == false)
	{
		/*
		m_laser = NewGO<Laser>(0, "laser");
		m_laser->m_owner = this;
		*/
		m_effect = NewGO<prefab::CEffect>(0);
		m_effect->Play(L"effect/Laser.efk");
		m_effect->SetScale({ 25.0f,25.0f,100.0f });
		
		m_isShooting = true;
		m_isDeleted = false;
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
			degx = 0.0f;
		}
		else
		{
			degx = acos(laserDir.Dot(zx));
		}

		//���[�U�[�̊p�x����������Ȃ��̂Ń��[�U�[�̌�����������Ȃ�p�x���}�C�i�X�ɂ���
		if (laserDir.y > 0)
			degx = -degx;

		//�O�t���[������̍����̉�]���Z�b�g
		m_qRotX.SetRotation(CVector3::AxisX, degx - odegx);


		//Y���̉�]
		degy = acos(CVector3::Front.Dot(zx));

		//���[�U�[�̊p�x����������Ȃ��̂Ń��[�U�[�̌������������Ȃ�p�x���}�C�i�X�ɂ���
		if (laserDir.x < 0)
			degy = -degy;

		//�O�t���[������̍����̉�]���Z�b�g
		m_qRotY.SetRotation(CVector3::AxisY, degy - odegy);


		m_rotation.Multiply(m_qRotX, m_rotation);
		m_rotation.Multiply(m_qRotY);

		m_effect->SetRotation(m_rotation);

		odegx = degx;
		odegy = degy;
	}

	//�v���C���[�Ƃ̋�����20000�ȏ㗣��Ă��āA�����[�U�[�𔭎˂��Ă�����
	if (diff.Length() > 20000 && m_isShooting == true && m_effect != nullptr)
	{
		DeleteGO(m_effect);
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
		}
	}

	count++;
	if (count < 100)
	{
		m_position.x += 100;
		m_position.y += 20;
		m_position.z += 50;
	}
	else if (count < 200)
	{
		m_position.x -= 100;
		m_position.y -= 20;
		m_position.z -= 50;
	}
	else
		count = 0;
		
	m_skinModelRender->SetPosition(m_position);
}