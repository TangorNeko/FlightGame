#include "stdafx.h"
#include "BackGround.h"

void BackGround::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

bool BackGround::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/background.cmo");
	m_skinModelRender->SetPosition({ 0.0f,-300.0f,0.0f });
	m_skinModelRender->SetScale({ 5.0f,5.0f,5.0f });
	return true;
}

void BackGround::Update()
{

}