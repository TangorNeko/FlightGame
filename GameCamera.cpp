#include "stdafx.h"
#include "GameCamera.h"
#include "Blackhole.h"

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
	if (FindGO<Blackhole>("blackhole",false) != nullptr)
	{
		m_position.x = player->m_position.x + player->m_moveDir.x * 500 + Up.x * 600;
		m_position.y = player->m_position.y + player->m_moveDir.y * 500 + Up.y * 600;
		m_position.z = player->m_position.z + player->m_moveDir.z * 500 + Up.z * 600;
	}
	else
	{
		m_position.x = player->m_position.x - player->m_moveDir.x * 500 + Up.x * 600;
		m_position.y = player->m_position.y - player->m_moveDir.y * 500 + Up.y * 600;
		m_position.z = player->m_position.z - player->m_moveDir.z * 500 + Up.z * 600;
	}
	MainCamera().SetPosition(m_position);

	//カメラの注視点はプレイヤーにする
	m_target = player->m_position + Up * 500.0f;
	MainCamera().SetTarget(m_target);

	if (player->m_skinModelRender != nullptr) {
		CMatrix mRotPlayer = player->m_skinModelRender->GetSkinModel().GetWorldMatrix();
		CVector3 playerUp = { mRotPlayer.m[2][0], mRotPlayer.m[2][1], mRotPlayer.m[2][2] };
		MainCamera().SetUp(playerUp);

	}


	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1500000.0f);
	

	MainCamera().Update();
}