#include "stdafx.h"
#include "Title.h"
#include "GameScene.h"

void Title::OnDestroy()
{

}

bool Title::Start()
{
	return true;
}

void Title::Update()
{
	if (Pad(0).IsTrigger(enButtonA))
	{
		NewGO<GameScene>(0, "gamescene");
		DeleteGO(this);
	}
}