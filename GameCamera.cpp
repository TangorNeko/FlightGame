#include "stdafx.h"
#include "GameCamera.h"
#include "Blackhole.h"
#include "GameScene.h"

void GameCamera::OnDestroy()
{

}

bool GameCamera::Start()
{
	m_gameScene = FindGO<GameScene>("gamescene");
	m_player = FindGO<Player>("player");

	return true;
}

void GameCamera::Update()
{
	CVector3 Up = MainCamera().GetUp();
	//カメラの位置はプレイヤーの進行方向の逆側の少し上側にする

	if (m_gameScene->m_isPlayerDead == false)
	{
		if (m_player->m_isTurning == true)
		{
			m_position.x = m_player->m_position.x + m_player->m_moveDir.x * 500 + Up.x * 600 + 500;
			m_position.y = m_player->m_position.y + m_player->m_moveDir.y * 500 + Up.y * 600;
			m_position.z = m_player->m_position.z + m_player->m_moveDir.z * 500 + Up.z * 600;
		}
		else if (FindGO<Blackhole>("blackhole", false) != nullptr)
		{
			
			switch (m_blackholeCameraScene)
			{
			case 0:
				m_position.x = m_player->m_position.x + m_player->m_moveDir.x * 500 + Up.x * 600;
				m_position.y = m_player->m_position.y + m_player->m_moveDir.y * 500 + Up.y * 600;
				m_position.z = m_player->m_position.z + m_player->m_moveDir.z * 500 + Up.z * 600;
				break;
			case 1:
				m_position.x = m_player->m_position.x + m_player->m_moveDir.x * 1000 + Up.x * 10000 + m_player->m_rightDir.x * 1000;
				m_position.y = m_player->m_position.y + m_player->m_moveDir.y * 1000 + Up.y * 10000 + m_player->m_rightDir.y * 1000;
				m_position.z = m_player->m_position.z + m_player->m_moveDir.z * 1000 + Up.z * 10000 + m_player->m_rightDir.z * 1000;
				break;
			case 2:
				if (m_isCameraChangeFrame == true)
				{
					m_position.x = m_player->m_position.x + m_player->m_moveDir.x * 20000 + Up.x * -5000 + m_player->m_rightDir.x * 1000;
					m_position.y = m_player->m_position.y + m_player->m_moveDir.y * 20000 + Up.y * -5000 + m_player->m_rightDir.x * 1000;
					m_position.z = m_player->m_position.z + m_player->m_moveDir.z * 20000 + Up.z * -5000 + m_player->m_rightDir.x * 1000;
					
					m_isCameraChangeFrame = false;
				}
				break;
			case 3:
				m_position.x = m_player->m_position.x + m_player->m_rightDir.x * 3000;
				m_position.y = m_player->m_position.y + m_player->m_rightDir.y * 3000;
				m_position.z = m_player->m_position.z + m_player->m_rightDir.z * 3000;

			}
		}
		else
		{
			m_position.x = m_player->m_position.x - m_player->m_moveDir.x * 500 + Up.x * 600;
			m_position.y = m_player->m_position.y - m_player->m_moveDir.y * 500 + Up.y * 600;
			m_position.z = m_player->m_position.z - m_player->m_moveDir.z * 500 + Up.z * 600;
		}
		MainCamera().SetPosition(m_position);

		//カメラの注視点はプレイヤーにする
		if (m_player->m_isTurning == true)
		{
			m_target = m_player->m_position;
			MainCamera().SetTarget(m_target);
		}
		else
		{
			m_target = m_player->m_position + Up * 500.0f;
			MainCamera().SetTarget(m_target);
		}

		if (m_player->m_isTurning == true)
		{
			MainCamera().SetUp(CVector3::Up);
		}
		else if (m_player->m_skinModelRender != nullptr) {
			CMatrix mRotPlayer = m_player->m_skinModelRender->GetSkinModel().GetWorldMatrix();
			CVector3 playerUp = { mRotPlayer.m[2][0], mRotPlayer.m[2][1], mRotPlayer.m[2][2] };
			MainCamera().SetUp(playerUp);

		}
	}
	else
	{
		MainCamera().SetPosition(m_position);
		MainCamera().SetTarget(m_target);
	}


	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1500000.0f);
	

	MainCamera().Update();
}