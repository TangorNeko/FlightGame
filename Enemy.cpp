#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"

void Enemy::OnDestroy()
{
	DeleteGO(m_laserEffect);
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/UFOMAPPED.cmo");
	m_skinModelRender->SetScale({ 5.0f,5.0f,5.0f });
	
	m_targetPlayer = FindGO<Player>("player");

	return true;
}

void Enemy::Update()
{
	//���[�U�[���ˊ֘A
	CVector3 diff, laserDir;

	diff = m_targetPlayer->m_position - m_position;

	CVector3 laserPosition = { m_position.x,m_position.y + 950,m_position.z };

	laserDir = m_targetPlayer->m_position - laserPosition;

	//���[�U�[�̊p�x
	laserDir.Normalize();

	dbg::DrawVector(laserDir * 1000,laserPosition, "laser");

	//�G�̕��Ɍ���
	CVector3 zx = { laserDir.x,0.0f,laserDir.z };
	zx.Normalize();

	//x���̉�]
	float degx = acos(laserDir.Dot(zx));
	dbgDegx = degx * (180/CMath::PI);
	//�p�x������ɓ���Ȃ��̂Ō����Ă�����������Ȃ畉�̐��ɂ���
	if (laserDir.y < 0)
		degx = -degx;

	if (degx < 0)
	{
		float a = degx;
	}

	CQuaternion qRotx;
	qRotx.SetRotation(CVector3::AxisX, degx - odegx);

	//y���̉�]
	float degy = acos(CVector3::Front.Dot(zx));
	//�p�x������ɓ���Ȃ��̂Ō����Ă�����������Ȃ畉�̐��ɂ���
	
	dbgDegy = degy * (180 / CMath::PI);

	if (laserDir.x < 0)
		degy = -degy;
		
	CQuaternion qRoty;
	qRoty.SetRotation(CVector3::AxisY, degy - odegy);

	m_laserQRot.Multiply(qRotx,m_laserQRot);
	m_laserQRot.Multiply(qRoty, m_laserQRot);

	odegx = degx;
	odegy = degy;
	

	if (m_isShooting == false)
	{
		m_laserEffect = NewGO<prefab::CEffect>(0);
		m_laserEffect->Play(L"effect/Laser5.efk");
		m_isShooting = true;
	}

	if (m_laserEffect != nullptr) {
		m_laserEffect->SetPosition(laserPosition);
		m_laserEffect->SetRotation(m_laserQRot);
		m_laserEffect->SetScale({ 75.0f,75.0f,100.0f });
	}

	/*
	if (diff.Length() > 10000 && m_isShooting == true)
	{
		DeleteGO(m_laserEffect);
		m_isShooting = false;
	}
	*/




	//�G�̓���
	/*
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
		*/
		
	m_skinModelRender->SetPosition(m_position);
}