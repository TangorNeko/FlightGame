#include "stdafx.h"
#include "Laser.h"
#include "Player.h"
#include "Enemy.h"

void Laser::OnDestroy()
{
	DeleteGO(m_effect);
}

bool Laser::Start()
{
	m_targetPlayer = FindGO<Player>("player");
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Play(L"effect/Laser.efk");
	m_effect->SetScale({ 25.0f,25.0f,100.0f });
	return true;
}

void Laser::Update()
{

	//���[�U�[���o���ʒu�̓A���e�i����ɂ���
	m_position.x = m_owner->m_position.x;
	m_position.y = m_owner->m_position.y + 950.0f;
	m_position.z = m_owner->m_position.z;

	m_effect->SetPosition(m_position);

	//���[�U�[�̌������擾
	CVector3 laserDir = m_targetPlayer->m_position - m_position;
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
	
	
	//���t���[�����ƂɃv���C���[�Ƀ_���[�W
	m_damageCount++;
	if (m_damageCount == 50)
	{
		m_targetPlayer->m_hp--;
		m_damageCount = 0;
	}


	//���t���[���̊p�x���L�^���Ă���
	odegx = degx;
	odegy = degy;
}