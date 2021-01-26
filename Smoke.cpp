#include "stdafx.h"
#include "Smoke.h"
#include "Player.h"

int Smoke::s_smokeNum = 0;

void Smoke::OnDestroy()
{
	s_smokeNum--;
	DeleteGO(m_skinModelRender);
}

bool Smoke::Start()
{
	s_smokeNum++;
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Smoke.cmo");

	m_player = FindGO<Player>("player");
	return true;
}

void Smoke::Update()
{
	m_timer += GameTime().GetFrameDeltaTime();
	float t = min( 1.0f,  m_timer / 1.0f );
	m_alpha = sin(t * CMath::PI);
	CMatrix mRot = MainCamera().GetCameraRotation();

	m_rotation.SetRotation(mRot);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetScale({ 15.0f,15.0f,15.0f });
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetAddObjectFlag(true);
	m_skinModelRender->GetSkinModel().SetEmissionColor({ m_alpha * 0.08f, 1.0f, 1.0f });
	CVector3 diff = m_player->m_position - m_position;

	if (diff.Length() > 2500)
		DeleteGO(this);
}