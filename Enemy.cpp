#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Laser.h"

void Enemy::OnDestroy()
{
	DeleteGO(m_laser);
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
		m_laser = NewGO<Laser>(0, "laser");
		m_laser->m_owner = this;
		m_isShooting = true;
	}

	//�v���C���[�Ƃ̋�����20000�ȏ㗣��Ă��āA�����[�U�[�𔭎˂��Ă�����
	if (diff.Length() > 20000 && m_isShooting == true)
	{
		DeleteGO(m_laser);
		m_isShooting = false;
	}

	//�G�̓���
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