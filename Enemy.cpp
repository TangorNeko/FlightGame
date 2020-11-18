#include "stdafx.h"
#include "Enemy.h"

void Enemy::OnDestroy()
{
	DeleteGO(tModel);
}

bool Enemy::Start()
{
	tModel = NewGO<prefab::CSkinModelRender>(0);
	tModel->Init(L"modelData/target.cmo");
	tModel->SetScale({ 100.0f,100.0f,100.0f });
	return true;
}

void Enemy::Update()
{
	tModel->SetPosition(position);
}