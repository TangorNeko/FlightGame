#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "Blackhole.h"

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_sightSpriteRender);
	DeleteGO(m_lockonSpriteRender);
	DeleteGO(m_fontRender);

	if (m_frictionEffect != nullptr && m_frictionEffect->IsPlay() == true)
		DeleteGO(m_frictionEffect);

	/*
	if (m_jetEffectRight != nullptr && m_jetEffectRight->IsPlay() == true)
		DeleteGO(m_jetEffectRight);

	if (m_jetEffectLeft != nullptr && m_jetEffectLeft->IsPlay() == true)
		DeleteGO(m_jetEffectRight);
		*/


}

bool Player::Start()
{
	

	//プレイヤーのモデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Vehicle.cmo");
	m_skinModelRender->SetScale({ 0.1f,0.1f,0.1f });
	m_skinModelRender->SetShadowReceiverFlag(true);
	m_skinModelRender->SetShadowCasterFlag(true);

	m_specSRV.CreateFromDDSTextureFromFile(L"modelData/Vehicle.fbm/vehicle_spec.dds");
	m_skinModelRender->FindMaterial([&](auto material) {
		material->SetSpecularMap(m_specSRV.GetBody());
	});
	//照準のスプライトを作成
	m_sightSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_sightSpriteRender->Init(L"sprite/Sight.dds", 32, 32);

	//ロックオンマーカーのスプライトを作成
	m_lockonSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_lockonSpriteRender->Init(L"sprite/Lockon.dds", 256, 256);


	//ジェットエフェクト
	m_jetEffectRight = NewGO<prefab::CEffect > (0);
	m_jetEffectRight->Play(L"effect/Jet.efk");

	m_jetEffectLeft = NewGO<prefab::CEffect>(0);
	m_jetEffectLeft->Play(L"effect/Jet.efk");


	//残弾数、燃料、スコアの文字を表示(テスト用)
	m_fontRender = NewGO<prefab::CFontRender>(0);
	m_fontRender->SetScale(0.5f);
	m_fontRender->SetPosition({ -600.0f,250.0f });

	return true;
}
void Player::PostUpdate()
{
	//照準の描画
	CVector2 screenpos2d;
	MainCamera().CalcScreenPositionFromWorldPosition(screenpos2d, m_position + m_moveDir * 10000);
	CVector3 screenpos = { screenpos2d.x,screenpos2d.y,0.0f };
	m_sightSpriteRender->SetPosition(screenpos);

}
void Player::Update()
{
	//スピード

	m_fSpeed += Pad(0).GetRStickYF();

	if (m_fSpeed < 20)
		m_fSpeed = 20;

	if (m_fSpeed > 150)
		m_fSpeed = 150;

	m_fuel -= m_fSpeed / 2000;

	if (Pad(0).IsTrigger(enButtonX))
	{
		m_isTurning = true;
	}

	if (m_isTurning)
	{
		m_fSpeed = 50;

		if (m_isBoosting == true)
		{
			DeleteGO(m_frictionEffect);
			m_frictionEffect = nullptr;
		}
		m_isBoosting = false;

		if (m_turnCount < 90)
		{
			m_x -= 2.0f;
		}
		else if(m_turnCount < 180)
		{
			m_z += 2.0f;
		}
		else
		{
			m_turnCount = 0;
			m_isTurning = false;
		}
		m_turnCount++;
	}
	else
	{
		if (m_fSpeed > 100)
		{
			

			if (m_isBoosting == false) {

				m_frictionEffect = NewGO<prefab::CEffect>(0);
				m_frictionEffect->Play(L"effect/Drill2.efk");
				m_frictionEffect->SetScale({ 10.0f,10.0f,10.0f });


				m_sbEffectRight = NewGO<prefab::CEffect>(0);
				m_sbEffectRight->Play(L"effect/Sonicboom.efk");
				m_sbEffectRight->SetScale({ 2.5f,2.5f,5.0f });


				m_sbEffectLeft = NewGO<prefab::CEffect>(0);
				m_sbEffectLeft->Play(L"effect/Sonicboom.efk");
				m_sbEffectLeft->SetScale({ 2.5f,2.5f,5.0f });
			}

			m_isBoosting = true;
		}
		else
		{
			if (m_isBoosting == true)
			{
				DeleteGO(m_frictionEffect);
				m_frictionEffect = nullptr;
			}
			m_isBoosting = false;
		}


		if (Pad(0).IsPress(enButtonLB2))
		{
			m_fSpeed *= 0;
		}


		
		//入力に応じて角度を変える
		if (Pad(0).IsPress(enButtonDown))
			m_x -= 1.0;

		if (Pad(0).IsPress(enButtonUp))
			m_x += 1.0;

		/*
		if (Pad(0).IsPress(enButtonA))
			m_y--;

		if (Pad(0).IsPress(enButtonB))
			m_y++;
		*/

		if (Pad(0).IsPress(enButtonRight))
			m_z -= 1.0;

		if (Pad(0).IsPress(enButtonLeft))
			m_z += 1.0;
		

		if (Pad(0).GetLStickYF() > 0.5)
			m_x--;
		else if (Pad(0).GetLStickYF() < -0.5)
			m_x++;

		if (Pad(0).GetLStickXF() < -0.5)
			m_z++;
		else if (Pad(0).GetLStickXF() > 0.5)
			m_z--;


	}

	//入力された角度をモデルに反映する
	m_qRotX.SetRotationDeg(CVector3::AxisX, m_x);

	m_qRotZ.SetRotationDeg(CVector3::AxisZ, m_z);

	m_rotation.Multiply(m_qRotX, m_rotation);
	m_rotation.Multiply(m_qRotZ, m_rotation);


	//入力された角度を進行方向に反映する
	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_moveDir.x = mRot.m[2][0];
	m_moveDir.y = mRot.m[2][1];
	m_moveDir.z = mRot.m[2][2];
	m_position += m_moveDir * m_fSpeed;

	m_rightDir.x = mRot.m[0][0];
	m_rightDir.y = mRot.m[0][1];
	m_rightDir.z = mRot.m[0][2];

	m_upDir.x = mRot.m[1][0];
	m_upDir.y = mRot.m[1][1];
	m_upDir.z = mRot.m[1][2];

	if (m_frictionEffect != nullptr)
	{
		m_frictionEffect->SetPosition(m_position);
		m_frictionEffect->SetRotation(m_rotation);
	}

	if (m_sbEffectRight != nullptr)
	{
		m_sbEffectRight->SetPosition(m_position + m_rightDir * 7.5 + m_moveDir * -90 + m_upDir * 20);
		m_sbEffectRight->SetRotation(m_rotation);
	}

	if (m_sbEffectLeft != nullptr)
	{
		m_sbEffectLeft->SetPosition(m_position + m_rightDir * -7.5 + m_moveDir * -90 + m_upDir * 20);
		m_sbEffectLeft->SetRotation(m_rotation);
	}

	if (m_jetEffectRight != nullptr)
	{
		m_jetEffectRight->SetPosition(m_position + m_rightDir * 7 + m_moveDir * -90 + m_upDir * 15);
		m_jetEffectRight->SetRotation(m_rotation);
		m_jetEffectRight->SetScale({ 5.0f,5.0f, 1 + m_fSpeed / 10 });
	}

	if (m_jetEffectLeft != nullptr)
	{
		m_jetEffectLeft->SetPosition(m_position + m_rightDir * -7 + m_moveDir * -90 + m_upDir * 15);
		m_jetEffectLeft->SetRotation(m_rotation);
		m_jetEffectLeft->SetScale({ 5.0f,5.0f,1 + m_fSpeed / 10 });
	}

	//ロックオン用の関数
	Lockon();

	
	if (FindGO<Blackhole>("blackhole", false) == nullptr)
		m_sightSpriteRender->SetScale({ 1.0f,1.0f,1.0f });
	else
		m_sightSpriteRender->SetScale({ 0.0f,0.0f,0.0f });
	


	//ミサイル発射用の関数
	if (Pad(0).IsTrigger(enButtonB) && m_lockingEnemy != nullptr && m_lockingEnemy->m_isMortal == false &&m_shotcooldown <= 60)
	{
		ShootMissile();
		m_shotcooldown += 60;
	}
	m_shotcooldown--;
	if (m_shotcooldown < 0)
		m_shotcooldown = 0;


	//デバッグ用
	std::wstring a = L"(仮)装弾数 = " + std::to_wstring((120 - m_shotcooldown) / 60) + L"\n(仮)燃料 = " + std::to_wstring(m_fuel) + L"\n(仮)スコア = " + std::to_wstring(m_score) + L"\n(仮)HP = " + std::to_wstring(m_hp) + L"\n(仮)スピード = " + std::to_wstring(m_fSpeed);
	m_fontRender->SetText(a.c_str());


	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);

	//そのフレームごとの移動量を使うので毎フレーム0にする
	m_x = m_y = m_z = 0;
}

void Player::Lockon()
{
	//最高ロックオン可能距離を設定する
	CVector3 Lockonpos = m_position + m_moveDir * m_maxLockonDistance;

	//ロックオン可能な角度を設定
	const int lockondegree = 20;

	//最高ロックオン可能距離より敵の位置までの距離が短いかつロックオン可能な角度内ならロックオン距離を更新する処理を繰り返す
	QueryGOs<Enemy>("enemy", [&Lockonpos,lockondegree,this](Enemy* enemy)->bool
		{
			//敵から自機への距離
			CVector3 tmp1 = enemy->m_position - this->m_position,
			//現在の最短距離から自機への距離
			tmp2 = Lockonpos - this->m_position;
			if (tmp1.Length() < tmp2.Length())
			{
				tmp1.Normalize();
				if (this->m_moveDir.Dot(tmp1) > cos(lockondegree * CMath::PI / 180) && enemy->m_isMortal == false)
				{
					Lockonpos = enemy->m_position;
					m_lockingEnemy = enemy;
				}
			}
			return true;
		}
	);

	//0ならどの敵もロックオンできていない。
	CVector3 Check = Lockonpos - (m_position + m_moveDir * m_maxLockonDistance);
	
	//ロックオンした敵のワールド座標からスクリーン座標に変換したものを受け取る用
	CVector2 spritepos;

	//ロックオンできているなら敵の位置にロックオンマーカーを表示する
	if (Check.Length())
	{
		//dbg::DrawVector(Lockonpos - m_position, m_position, "locking");
		MainCamera().CalcScreenPositionFromWorldPosition(spritepos, Lockonpos);
		CVector3 screenpos = { spritepos.x,spritepos.y,0.0f };
		m_lockonSpriteRender->SetPosition(screenpos);

		CVector3 distance = Lockonpos - m_position;

		//距離に応じてロックオンマーカーの大きさを変更
		m_lockonSpriteRender->SetScale({ 50 / (distance.Length() / 100) ,50 / (distance.Length() / 100) ,50 / (distance.Length() / 100) });
		m_lockonSpriteRender->SetScale({1.0f,1.0f,1.0f });
	}
	//できていないならロックオンマーカーを非表示に
	else
	{	
		m_lockonSpriteRender->SetScale(CVector3::Zero);
		m_lockingEnemy = nullptr;
	}
}

void Player::ShootMissile()
{
	Missile* missile = NewGO<Missile>(0, "missile");

	//左右交互にミサイルを撃つ
	if (m_isMissileRight)
	{
		missile->m_position = m_position + m_moveDir * 100.0f + m_rightDir * 50.0f;
		m_isMissileRight = false;
	}
	else
	{
		missile->m_position = m_position + m_moveDir * 100.0f + m_rightDir * -50.0f;
		m_isMissileRight = true;
	}

	missile->m_fSpeed = max(1,m_fSpeed - 30);

	//ミサイルに追尾先の敵を教える
	missile->m_trackingEnemy = m_lockingEnemy;
	//撃った瞬間相手に死にゆく定めを付与する
	m_lockingEnemy->m_isMortal = true;
}