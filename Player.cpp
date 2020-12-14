#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "Blackhole.h"
#include "Laser.h"

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_sightSpriteRender);
	DeleteGO(m_lockonSpriteRender);
	DeleteGO(m_fontRender);

	if(m_isBoosting == true)
	DeleteGO(m_frictionEffect);
}

bool Player::Start()
{
	

	//�v���C���[�̃��f�����쐬
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Vehicle.cmo");
	m_skinModelRender->SetScale({ 0.1f,0.1f,0.1f });
	m_skinModelRender->SetShadowReceiverFlag(true);
	m_skinModelRender->SetShadowCasterFlag(true);

	m_specSRV.CreateFromDDSTextureFromFile(L"modelData/Vehicle.fbm/vehicle_spec.dds");
	m_skinModelRender->FindMaterial([&](auto material) {
		material->SetSpecularMap(m_specSRV.GetBody());
	});
	//�Ə��̃X�v���C�g���쐬
	m_sightSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_sightSpriteRender->Init(L"sprite/Sight.dds", 32, 32);

	//���b�N�I���}�[�J�[�̃X�v���C�g���쐬
	m_lockonSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_lockonSpriteRender->Init(L"sprite/Lockon.dds", 256, 256);


	//�c�e���A�R���A�X�R�A�̕�����\��(�e�X�g�p)
	m_fontRender = NewGO<prefab::CFontRender>(0);
	m_fontRender->SetScale(0.5f);
	m_fontRender->SetPosition({ -600.0f,250.0f });

	return true;
}
void Player::PostUpdate()
{
	//�Ə��̕`��
	CVector2 screenpos2d;
	MainCamera().CalcScreenPositionFromWorldPosition(screenpos2d, m_position + m_moveDir * 10000);
	CVector3 screenpos = { screenpos2d.x,screenpos2d.y,0.0f };
	m_sightSpriteRender->SetPosition(screenpos);

}
void Player::Update()
{
	//�X�s�[�h
	m_fSpeed = 50;

	if (Pad(0).IsTrigger(enButtonX))
	{
		m_isTurning = true;
	}

	if (m_isTurning)
	{
		if (m_isBoosting == true)
			DeleteGO(m_frictionEffect);
		m_isBoosting = false;

		if (m_turnCount < 90)
		{
			m_x -= 2.0f;
		}
		else if(m_turnCount < 180)
		{
			m_z += 2.0f;
		}
		else
		{
			m_turnCount = 0;
			m_isTurning = false;
		}
		m_turnCount++;
	}
	else
	{
		if (Pad(0).IsPress(enButtonLB3) && m_fuel > 0)
		{
			m_fSpeed *= 2;
			m_fuel--;

			if (m_isBoosting == false) {
				m_frictionEffect = NewGO<prefab::CEffect>(0);
				m_frictionEffect->Play(L"effect/Drill2.efk");
				m_frictionEffect->SetScale({ 10.0f,10.0f,10.0f });
			}

			m_isBoosting = true;
		}
		else
		{
			if (m_isBoosting == true)
				DeleteGO(m_frictionEffect);
			m_isBoosting = false;
		}
		if (Pad(0).IsPress(enButtonLB2))
		{
			m_fSpeed *= 0;
		}

		//���͂ɉ����Ċp�x��ς���
		if (Pad(0).IsPress(enButtonDown))
			m_x -= 1.0;

		if (Pad(0).IsPress(enButtonUp))
			m_x += 1.0;

		/*
		if (Pad(0).IsPress(enButtonA))
			m_y--;

		if (Pad(0).IsPress(enButtonB))
			m_y++;
		*/

		if (Pad(0).IsPress(enButtonRight))
			m_z -= 1.0;

		if (Pad(0).IsPress(enButtonLeft))
			m_z += 1.0;

	}

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

	if (m_frictionEffect != nullptr)
	{
		m_frictionEffect->SetPosition(m_position);
		m_frictionEffect->SetRotation(m_rotation);
	}

	//���b�N�I���p�̊֐�
	Lockon();

	
	if (FindGO<Blackhole>("blackhole", false) == nullptr)
		m_sightSpriteRender->SetScale({ 1.0f,1.0f,1.0f });
	else
		m_sightSpriteRender->SetScale({ 0.0f,0.0f,0.0f });
	


	//�~�T�C�����˗p�̊֐�
	if (Pad(0).IsTrigger(enButtonB) && m_lockingEnemy != nullptr && m_lockingEnemy->m_isMortal == false &&m_shotcooldown <= 60)
	{
		ShootMissile();
		m_shotcooldown += 60;
	}
	m_shotcooldown--;
	if (m_shotcooldown < 0)
		m_shotcooldown = 0;


	//�f�o�b�O�p
	std::wstring a = L"(��)���e�� = " + std::to_wstring((120 - m_shotcooldown) / 60) + L"\n(��)�R�� = " + std::to_wstring(m_fuel) + L"\n(��)�X�R�A = " + std::to_wstring(m_score) + L"\n(��)HP = " + std::to_wstring(m_hp);
	m_fontRender->SetText(a.c_str());


	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);

	//���̃t���[�����Ƃ̈ړ��ʂ��g���̂Ŗ��t���[��0�ɂ���
	m_x = m_y = m_z = 0;
}

void Player::Lockon()
{
	//�ō����b�N�I���\������ݒ肷��
	CVector3 Lockonpos = m_position + m_moveDir * m_maxLockonDistance;

	//���b�N�I���\�Ȋp�x��ݒ�
	const int lockondegree = 20;

	//�ō����b�N�I���\�������G�̈ʒu�܂ł̋������Z�������b�N�I���\�Ȋp�x���Ȃ烍�b�N�I���������X�V���鏈�����J��Ԃ�
	QueryGOs<Enemy>("enemy", [&Lockonpos,lockondegree,this](Enemy* enemy)->bool
		{
			//�G���玩�@�ւ̋���
			CVector3 tmp1 = enemy->m_position - this->m_position,
			//���݂̍ŒZ�������玩�@�ւ̋���
			tmp2 = Lockonpos - this->m_position;
			if (tmp1.Length() < tmp2.Length())
			{
				tmp1.Normalize();
				if (this->m_moveDir.Dot(tmp1) > cos(lockondegree * CMath::PI / 180) && enemy->m_isMortal == false)
				{
					Lockonpos = enemy->m_position;
					m_lockingEnemy = enemy;
				}
			}
			return true;
		}
	);

	//0�Ȃ�ǂ̓G�����b�N�I���ł��Ă��Ȃ��B
	CVector3 Check = Lockonpos - (m_position + m_moveDir * m_maxLockonDistance);
	
	//���b�N�I�������G�̃��[���h���W����X�N���[�����W�ɕϊ��������̂��󂯎��p
	CVector2 spritepos;

	//���b�N�I���ł��Ă���Ȃ�G�̈ʒu�Ƀ��b�N�I���}�[�J�[��\������
	if (Check.Length())
	{
		//dbg::DrawVector(Lockonpos - m_position, m_position, "locking");
		MainCamera().CalcScreenPositionFromWorldPosition(spritepos, Lockonpos);
		CVector3 screenpos = { spritepos.x,spritepos.y,0.0f };
		m_lockonSpriteRender->SetPosition(screenpos);

		CVector3 distance = Lockonpos - m_position;

		//�����ɉ����ă��b�N�I���}�[�J�[�̑傫����ύX
		m_lockonSpriteRender->SetScale({ 50 / (distance.Length() / 100) ,50 / (distance.Length() / 100) ,50 / (distance.Length() / 100) });
		m_lockonSpriteRender->SetScale({1.0f,1.0f,1.0f });
	}
	//�ł��Ă��Ȃ��Ȃ烍�b�N�I���}�[�J�[���\����
	else
	{	
		m_lockonSpriteRender->SetScale(CVector3::Zero);
		m_lockingEnemy = nullptr;
	}
}

void Player::ShootMissile()
{
	Missile* missile = NewGO<Missile>(0, "missile");
	missile->m_position = m_position + m_moveDir * 100;
	//�~�T�C���ɒǔ���̓G��������
	missile->m_trackingEnemy = m_lockingEnemy;
	//�������u�ԑ���Ɏ��ɂ䂭��߂�t�^����
	m_lockingEnemy->m_isMortal = true;
}