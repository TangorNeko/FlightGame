#include "stdafx.h"
#include "Ring.h"
#include "Player.h"

void Ring::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

bool Ring::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/ring.cmo");
	m_skinModelRender->SetScale({ 2.0f,2.0f,2.0f });

	player = FindGO<Player>("player");
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
		if (m_direction.Dot(player->m_moveDir) > 0)
		{
			player->m_score += 100;
			DeleteGO(this);
		}

		if (-m_direction.Dot(player->m_moveDir) > 0)
		{
			player->m_fuel += 100;
			DeleteGO(this);
		}
	}

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);

	m_x = 0;
	m_y = 0;
}