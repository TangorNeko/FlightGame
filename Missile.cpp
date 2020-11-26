#include "stdafx.h"
#include "Missile.h"
#include "Enemy.h"

void Missile::OnDestroy()
{

}

bool Missile::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modeldata/testMissile.cmo");
	return true;
}

void Missile::Update()
{
	m_skinModelRender->SetPosition(m_position);
}