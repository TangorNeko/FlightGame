#include "stdafx.h"
#include "EnemyMissile.h"
#include "Player.h"

void EnemyMissile::OnDestroy()
{

	DeleteGO(m_effect2);

	DeleteGO(m_skinModelRender);
}

bool EnemyMissile::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/MissileTest.cmo");
	m_effect2 = NewGO<prefab::CEffect>(0);
	m_skinModelRender->SetScale({1.5f,1.5f,1.5f });

	m_trackingPlayer = FindGO<Player>("player");

	m_trackingPlayer->m_trackingMissileNum++;

	m_oldMoveDir = m_trackingPlayer->m_position - m_position;
	m_oldMoveDir.Normalize();
	return true;
}

void EnemyMissile::Update()
{

	if (m_effect2->IsPlay() == false)
	{
		m_effect2->Play(L"effect/Missile3.efk");
	}

	//�G��ǔ�����
	CVector3 diff = m_moveDir = m_trackingPlayer->m_position - m_position;
	m_moveDir.Normalize();

	//�^�[�������ꍇ�ǔ����Ȃ��Ȃ�
	if (m_trackingPlayer->m_isTurning == true && m_isRocking == true)
	{
		m_isRocking = false;
		m_trackingPlayer->m_trackingMissileNum--;
	}

	if (m_isRocking == false)
		m_moveDir = m_oldMoveDir;

	m_oldMoveDir = m_moveDir;
	m_fSpeed += m_fSpeed / 20;
	if (m_fSpeed > 300)
	{
		m_fSpeed = 300;
	}
	m_position += m_moveDir * m_fSpeed;
	m_skinModelRender->SetPosition(m_position);

	m_effect2->SetPosition(m_position + m_moveDir * -25.0f);
	m_effect2->SetScale({ 15.0f,15.0f,15.0f });


	//�G�̕��Ɍ���
	CVector3 zx = { m_moveDir.x,0.0f,m_moveDir.z };
	zx.Normalize();

	//x���̉�]
	float degx = acos(m_moveDir.Dot(zx));
	//�p�x������ɓ���Ȃ��̂Ō����Ă���������Ȃ畉�̐��ɂ���
	if (m_moveDir.y > 0)
		degx = -degx;
	CQuaternion qRotx;
	qRotx.SetRotation(CVector3::AxisX, degx - odegx);

	//y���̉�]
	float degy = acos(CVector3::Front.Dot(zx));
	//�p�x������ɓ���Ȃ��̂Ō����Ă���������Ȃ畉�̐��ɂ���
	if (m_moveDir.x < 0)
		degy = -degy;
	CQuaternion qRoty;
	qRoty.SetRotation(CVector3::AxisY, degy - odegy);

	m_qRot.Multiply(qRotx, m_qRot);
	m_qRot.Multiply(qRoty, m_qRot);

	m_skinModelRender->SetRotation(m_qRot);
	m_effect2->SetRotation(m_qRot);

	odegx = degx;
	odegy = degy;


	//�G�̋߂��܂ŗ�����
	if (diff.Length() < 200)
	{
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/fire3.efk");
		effect->SetPosition(m_trackingPlayer->m_position + m_trackingPlayer->m_moveDir * m_trackingPlayer->m_fSpeed * 20 + m_trackingPlayer->m_upDir * -200);
		effect->SetScale({ 50.0f,50.0f,50.0f });
		m_trackingPlayer->m_trackingMissileNum--;
		DeleteGO(this);
	}

	m_lifeSpan--;
	if (m_lifeSpan == 0)
		DeleteGO(this);
}