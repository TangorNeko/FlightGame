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

	player = FindGO<Player>("player");

	m_qRot.SetRotationDeg(CVector3::Up, 180.0f);

	m_skinModelRender->SetRotation(m_qRot);
	return true;
}

void Ring::Update()
{

	CVector3 diff = m_position - player->m_position;

	if (diff.Length() < 200 && m_position.z -50 < player->m_position.z && player->m_position.z < m_position.z + 50)
	{
		if (m_direction.Dot(player->m_moveDir) > 0)
		{
			DeleteGO(this);
		}
	}

	m_skinModelRender->SetPosition(m_position);
}