#include "stdafx.h"
#include "Blackhole.h"
#include "Player.h"
#include "RingGenerator.h"

void Blackhole::OnDestroy()
{
	DeleteGO(m_bhEffect);
}

bool Blackhole::Start()
{
	
	DeleteGO(FindGO<RingGenerator>("ringgenerator"));
	m_bhEffect = NewGO<prefab::CEffect>(0);
	m_bhEffect->Play(L"effect/blackhole.efk");

	player = FindGO<Player>("player");

	m_scaling.x = 1000.0f;
	m_scaling.y = 1000.0f;
	m_scaling.z = 1000.0f;

	bhradius = 700.0f;
	return true;
}

void Blackhole::Update()
{
	//�u���b�N�z�[���G�t�F�N�g���Đ�����Ă��Ȃ��Ȃ�Đ�(�����I�ȃ��[�v)
	if (m_bhEffect->IsPlay() == false)
	{
		m_bhEffect->Play(L"effect/blackhole.efk");
	}
	m_bhEffect->SetScale(m_scaling);
	m_bhEffect->SetPosition(m_position);

	CVector3 rad = { 0.0f,0.0f,bhradius };
	dbg::DrawVector(rad,m_position,"bhradius");


	//�z�����ޗp�Ƀv���C���[����u���b�N�z�[���̒��S�܂ł̋������擾
	CVector3 pdiff = player->m_position - m_position;

	//�u���b�N�z�[���̔��a���v���C���[�Ƃ̋����̕����߂���΁A���t���[��100�̑����Ńu���b�N�z�[���̒��S�ɋz���񂹂�
	if (pdiff.Length() < bhradius)
	{
		float a = pdiff.Length();
		pdiff.Normalize();
		player->m_position -= pdiff * 100;
	}
	

	//�t���O���g�����u���b�N�z�[���̊g�又��
	//isExpanding(�g�咆)��true�Ȃ�A���t���[���G�t�F�N�g�̊g�嗦��150,���a��105����(�g�嗦:���a = 10 : 7)
	if (isExpanding) {
		m_scaling.x += 150.0f;
		m_scaling.y += 150.0f;
		m_scaling.z += 150.0f;

		bhradius += 105.0f;
	}
	else //�g�咆�łȂ�(�k����)�Ȃ珙�X�ɏk�����x�𑝂��Ȃ���k������
	{
		m_scaling.x -= expire;
		m_scaling.y -= expire;
		m_scaling.z -= expire;

		bhradius -= 0.7 * expire;

		expire *= 1.01f;
	}

	//�k���̌��ʃu���b�N�z�[���̔��a��60�ȉ��ɂȂ�΍폜
	if (bhradius < 60.0f)
	{
		DeleteGO(this);
	}

	//1800�t���[���܂Ŋg�債���̌�k������
	if (count == 1800)
	{
		isExpanding = false;
	}

	count++;
	
}