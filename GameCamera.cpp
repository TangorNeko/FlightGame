#include "stdafx.h"
#include "GameCamera.h"

void GameCamera::OnDestroy()
{

}

bool GameCamera::Start()
{
	player = FindGO<Player>("player");

	return true;
}

void GameCamera::Update()
{
	CVector3 Up = MainCamera().GetUp();
	//カメラの位置はプレイヤーの進行方向の逆側の少し上側にする
	position.x = player->position.x - player->MoveDir.x * 250 + Up.x * 100;
	position.y = player->position.y - player->MoveDir.y * 250 + Up.y * 100;
	position.z = player->position.z - player->MoveDir.z * 250 + Up.z * 100;
	MainCamera().SetPosition(position);

	//カメラの注視点はプレイヤーにする
	target = player->position;

	MainCamera().SetTarget(target);


	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(100000.0f);
	

	MainCamera().Update();
}