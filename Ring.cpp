#include "stdafx.h"
#include "Ring.h"
#include "Player.h"
#include "tkEngine/Light/tkPointLight.h"

int Ring::s_ringNum = 0;

void Ring::OnDestroy()
{
	s_ringNum--;
	DeleteGO(m_skinModelRender);
	DeleteGO(plight);
}

bool Ring::Start()
{
	plight = NewGO<prefab::CPointLight>(0);
	CVector3 attn = CVector3::Zero;
	attn.x = 300.0f;
	attn.y = 1.0f;
	plight->SetAttn(attn);
	plight->SetColor({ 100.0f,100.0f,100.0f });

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/ring.cmo");
	m_skinModelRender->SetScale({ 2.0f,2.0f,2.0f });

	player = FindGO<Player>("player");

	s_ringNum++;
	return true;
}

void Ring::Update()
{
	m_qRotX.SetRotationDeg(CVector3::AxisX, m_x);

	m_qRotY.SetRotationDeg(CVector3::AxisY, m_y);

	m_rotation.Multiply(m_qRotX, m_rotation);
	m_rotation.Multiply(m_qRotY, m_rotation);

	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_direction.x = mRot.m[2][0];
	m_direction.y = mRot.m[2][1];
	m_direction.z = mRot.m[2][2];

	m_direction.Normalize();

	
	CVector3 diff = m_position - player->m_position;

	if (diff.Length() < 400)
	{

		//角度が80度より小さいなら取得
		//赤い面から通ったならスコアを増加
		if (m_direction.Dot(player->m_moveDir) > 0.17)
		{
			player->m_score += 100;
			DeleteGO(this);
		}
		//青い面から通ったなら燃料を増加
		if (-m_direction.Dot(player->m_moveDir) > 0.17)
		{
			player->m_fuel += 100;
			DeleteGO(this);
		}
	}

	if (diff.Length() > 50000)
	{
		DeleteGO(this);
	}

	m_skinModelRender->SetPosition(m_position);
	plight->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);

	m_x = 0;
	m_y = 0;
}