#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"

void Player::OnDestroy()
{
	DeleteGO(Vmodel);
	DeleteGO(lock);
}

bool Player::Start()
{
	//�v���C���[�̃��f�����쐬
	Vmodel = NewGO<prefab::CSkinModelRender>(0);
	Vmodel->Init(L"modelData/Vehicle.cmo");
	Vmodel->SetScale({ 0.1f,0.1f,0.1f });

	//���b�N�I���}�[�J�[�̃X�v���C�g���쐬
	lock = NewGO<prefab::CSpriteRender>(0);
	lock->Init(L"sprite/lock.dds", 128, 128);

	return true;
}

void Player::Update()
{
	//�X�s�[�h
	fSpeed = 10;

	//���̃t���[�����Ƃ̈ړ��ʂ��g���̂Ŗ��t���[��0�ɂ���
	x = y = z = 0;

	//���͂ɉ����Ċp�x��ς���
	if (Pad(0).IsPress(enButtonDown))
		x--;

	if (Pad(0).IsPress(enButtonUp))
		x++;

	/*
	if (Pad(0).IsPress(enButtonA))
		y--;

	if (Pad(0).IsPress(enButtonB))
		y++;
	*/

	if (Pad(0).IsPress(enButtonRight))
		z--;

	if (Pad(0).IsPress(enButtonLeft))
		z++;


	//���͂��ꂽ�p�x�����f���ɔ��f����
	qRotX.SetRotationDeg(CVector3::AxisX, x);

	qRotZ.SetRotationDeg(CVector3::AxisZ, z);

	m_rotation.Multiply(qRotX, m_rotation);
	m_rotation.Multiply(qRotZ, m_rotation);


	//���͂��ꂽ�p�x��i�s�����ɔ��f����
	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);
	MoveDir.x = mRot.m[2][0];
	MoveDir.y = mRot.m[2][1];
	MoveDir.z = mRot.m[2][2];
	position += MoveDir * fSpeed;

	//���b�N�I���p�̊֐�
	Lockon();

	Vmodel->SetRotation(m_rotation);
	Vmodel->SetPosition(position);
}

void Player::Lockon()
{
	//�ō����b�N�I���\������ݒ肷��
	CVector3 Lockonpos = MoveDir * 100000.0f;

	
	//���b�N�I���\�Ȋp�x��ݒ�
	const int lockondegree = 20;


	//�ō����b�N�I���\�������G�̈ʒu�܂ł̋������Z�������b�N�I���\�Ȋp�x���Ȃ烍�b�N�I���������X�V���鏈�����J��Ԃ�
	QueryGOs<Enemy>("enemy", [&Lockonpos,lockondegree,this](Enemy* enemy)->bool {
		CVector3 tmp1 = enemy->position - this->position,
			tmp2 = Lockonpos - this->position;
		if (tmp1.Length() < tmp2.Length())
		{
			tmp1.Normalize();
			if(this->MoveDir.Dot(tmp1) > cos(lockondegree * CMath::PI / 180))
			Lockonpos = enemy->position;
		}
		return true;
		}
	);

	//0�Ȃ�ǂ̓G�����b�N�I���ł��Ă��Ȃ��B
	CVector3 Check = Lockonpos - MoveDir * 100000.0f;
	
	//���b�N�I�������G�̃��[���h���W����X�N���[�����W�ɕϊ��������̂��󂯎��p
	CVector2 spritepos;

	//���b�N�I���ł��Ă���Ȃ�G�̈ʒu�Ƀ��b�N�I���}�[�J�[��\������
	if (Check.Length())
	{
	dbg::DrawVector(Lockonpos, { 0.0f,0.0f,0.0f }, "locking");
	MainCamera().CalcScreenPositionFromWorldPosition(spritepos, Lockonpos);
	CVector3 screenpos = { spritepos.x,spritepos.y,0.0f };
	lock->SetPosition(screenpos);
	}
	//�ł��Ă��Ȃ��Ȃ�v���C���[�̂���ɕ\��
	else
	{
		lock->SetPosition({ 0.0f,150.0f,0.0f });
	}
}