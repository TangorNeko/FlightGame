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
		m_position.x = player->m_position.x + player->m_moveDir.x * 250 + Up.x * 100;
		m_position.y = player->m_position.y + player->m_moveDir.y * 250 + Up.y * 100;
		m_position.z = player->m_position.z + player->m_moveDir.z * 250 + Up.z * 100;
	}
	else
	{
		m_position.x = player->m_position.x - player->m_moveDir.x * 250 + Up.x * 100;
		m_position.y = player->m_position.y - player->m_moveDir.y * 250 + Up.y * 100;
		m_position.z = player->m_position.z - player->m_moveDir.z * 250 + Up.z * 100;
	}
	MainCamera().SetPosition(m_position);

	//カメラの注視点はプレイヤーにする
	m_target = player->m_position;

	MainCamera().SetTarget(m_target);


	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1500000.0f);
	

	MainCamera().Update();
}