#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_spriteRender);
}

bool Player::Start()
{
	//�v���C���[�̃��f�����쐬
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Vehicle.cmo");
	m_skinModelRender->SetScale({ 0.1f,0.1f,0.1f });

	//���b�N�I���}�[�J�[�̃X�v���C�g���쐬
	m_spriteRender = NewGO<prefab::CSpriteRender>(0);
	m_spriteRender->Init(L"sprite/lock.dds", 128, 128);

	return true;
}

void Player::Update()
{
	//�X�s�[�h
	m_fSpeed = 10;

	//���̃t���[�����Ƃ̈ړ��ʂ��g���̂Ŗ��t���[��0�ɂ���
	m_x = m_y = m_z = 0;

	//���͂ɉ����Ċp�x��ς���
	if (Pad(0).IsPress(enButtonDown))
		m_x--;

	if (Pad(0).IsPress(enButtonUp))
		m_x++;

	/*
	if (Pad(0).IsPress(enButtonA))
		m_y--;

	if (Pad(0).IsPress(enButtonB))
		m_y++;
	*/

	if (Pad(0).IsPress(enButtonRight))
		m_z--;

	if (Pad(0).IsPress(enButtonLeft))
		m_z++;


	//���͂��ꂽ�p�x�����f���ɔ��f����
	m_qRotX.SetRotationDeg(CVector3::AxisX, m_x);

	m_qRotZ.SetRotationDeg(CVector3::AxisZ, m_z);

	m_rotation.Multiply(m_qRotX, m_rotation);
	m_rotation.Multiply(m_qRotZ, m_rotation);


	//���͂��ꂽ�p�x��i�s�����ɔ��f����
	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_moveDir.x = mRot.m[2][0];
	m_moveDir.y = mRot.m[2][1];
	m_moveDir.z = mRot.m[2][2];
	m_position += m_moveDir * m_fSpeed;

	//���b�N�I���p�̊֐�
	Lockon();

	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

void Player::Lockon()
{
	//�ō����b�N�I���\������ݒ肷��
	CVector3 Lockonpos = m_moveDir * 100000.0f;

	
	//���b�N�I���\�Ȋp�x��ݒ�
	const int lockondegree = 20;


	//�ō����b�N�I���\�������G�̈ʒu�܂ł̋������Z�������b�N�I���\�Ȋp�x���Ȃ烍�b�N�I���������X�V���鏈�����J��Ԃ�
	QueryGOs<Enemy>("enemy", [&Lockonpos,lockondegree,this](Enemy* enemy)->bool {
		CVector3 tmp1 = enemy->m_position - this->m_position,
			tmp2 = Lockonpos - this->m_position;
		if (tmp1.Length() < tmp2.Length())
		{
			tmp1.Normalize();
			if(this->m_moveDir.Dot(tmp1) > cos(lockondegree * CMath::PI / 180))
			Lockonpos = enemy->m_position;
		}
		return true;
		}
	);

	//0�Ȃ�ǂ̓G�����b�N�I���ł��Ă��Ȃ��B
	CVector3 Check = Lockonpos - m_moveDir * 100000.0f;
	
	//���b�N�I�������G�̃��[���h���W����X�N���[�����W�ɕϊ��������̂��󂯎��p
	CVector2 spritepos;

	//���b�N�I���ł��Ă���Ȃ�G�̈ʒu�Ƀ��b�N�I���}�[�J�[��\������
	if (Check.Length())
	{
		dbg::DrawVector(Lockonpos, { 0.0f,0.0f,0.0f }, "locking");
		MainCamera().CalcScreenPositionFromWorldPosition(spritepos, Lockonpos);
		CVector3 screenpos = { spritepos.x,spritepos.y,0.0f };
		m_spriteRender->SetPosition(screenpos);
	}
	//�ł��Ă��Ȃ��Ȃ�v���C���[�̂���ɕ\��
	else
	{
		m_spriteRender->SetPosition({ 0.0f,150.0f,0.0f });
	}
}