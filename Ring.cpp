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

	return true;
}

void Ring::Update()
{
	CVector3 diff = m_position;
}