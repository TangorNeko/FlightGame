#include "stdafx.h"
#include "SpaceDock.h"
#include "TitlePlayer.h"
#include "TitlePlayer.h"

void SpaceDock::OnDestroy()
{
	DeleteGO(m_portalEffect);
	DeleteGO(m_teleporterModel);
	DeleteGO(m_dockModel);
}

bool SpaceDock::Start()
{
	m_dockModel = NewGO<prefab::CSkinModelRender>(0);
	m_dockModel->Init(L"modelData/Corridor.cmo");
	m_dockModel->SetPosition({ 0.0f,0.0f,2500.0f });

	m_teleporterModel = NewGO<prefab::CSkinModelRender>(0);
	m_teleporterModel->Init(L"modelData/Teleporter.cmo");
	m_teleporterModel->SetPosition({ 0.0f,0.0f,40000.0f });
	CQuaternion teleporterRot;
	teleporterRot.SetRotationDeg(CVector3::AxisY, 180.0f);
	m_teleporterModel->SetRotation(teleporterRot);
	/*
	m_portalEffect = NewGO<prefab::CEffect>(0);
	m_portalEffect->Play(L"effect/Portal.efk");
	m_portalEffect->SetPosition({ 0.0f,0.0f,44000.0f });
	m_portalEffect->SetScale({ 250.0f,250.0f,250.0f });
	*/

	m_player = FindGO<TitlePlayer>("player");
	return true;
}

void SpaceDock::Update()
{
	if ((Pad(0).IsTrigger(enButtonLB1) || m_player->m_position.z > 20000) && m_isPortalOpen == false)
	{
		m_portalEffect = NewGO<prefab::CEffect>(0);
		m_portalEffect->Play(L"effect/Portal.efk");
		m_portalEffect->SetPosition({ 0.0f,0.0f,35500.0f });
		m_portalEffect->SetScale({ 250.0f,250.0f,250.0f });
		m_isPortalOpen = true;
	}
}