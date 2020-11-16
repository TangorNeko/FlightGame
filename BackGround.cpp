#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	BGmodel = NewGO<prefab::CSkinModelRender>(0);
	BGmodel->Init(L"modelData/background.cmo");
	BGmodel->SetPosition({ 0.0f,-300.0f,0.0f });
	BGmodel->SetScale({ 5.0f,5.0f,5.0f });
	return true;
}

void BackGround::Update()
{

}