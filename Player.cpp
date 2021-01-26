#include "stdafx.h"
#include "Player.h"
#include "IEnemy.h"
#include "LaserEnemy.h"
#include "Missile.h"
#include "Blackhole.h"
#include "SmokeGenerator.h"
#include "GameScene.h"
#include "EnemyGenerator.h"
#include "GameOver.h"


void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_sightSpriteRender);
	DeleteGO(m_lockonSpriteRender);
	DeleteGO(m_fontRender);
	DeleteGO(m_smokeGenerator);
	DeleteGO(m_speedFontRender);
	DeleteGO(m_speedSpriteBackRender);
	DeleteGO(m_speedSpriteFrontRender);
	DeleteGO(m_HPSpriteBackRender);
	DeleteGO(m_HPSpriteFrontRender);
	DeleteGO(m_FuelSpriteBackRender);
	DeleteGO(m_FuelSpriteFrontRender);
	DeleteGO(m_controlSpriteRender);
	DeleteGO(m_missionPanelRender);

	DeleteGO(m_missileTrackingSpriteRender);


	DeleteGO(m_engineSound);

	if (m_frictionEffect != nullptr && m_frictionEffect->IsPlay() == true)
		DeleteGO(m_frictionEffect);

	if (m_jetEffectRight != nullptr && m_jetEffectRight->IsPlay() == true)
		DeleteGO(m_jetEffectRight);

	if (m_jetEffectLeft != nullptr && m_jetEffectLeft->IsPlay() == true)
		DeleteGO(m_jetEffectLeft);

	m_gameScene->m_isPlayerDead = true;
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

	//操作パネルを表示
	m_controlSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_controlSpriteRender->Init(L"sprite/HFControl.dds",230.0f,45.0f);
	m_controlSpriteRender->SetPosition({ -500.0f,250.0f,0.0f });

	//HPを表示
	m_HPSpriteBackRender = NewGO<prefab::CSpriteRender>(0);
	m_HPSpriteBackRender->Init(L"sprite/HFHPUI_Back.dds",455.0f,44.0f);
	m_HPSpriteBackRender->SetPosition({ -625.0f,330.0f,0.0f });
	m_HPSpriteBackRender->SetPivot({ 0.0f, 0.5f });

	m_HPSpriteFrontRender = NewGO<prefab::CSpriteRender>(0);
	m_HPSpriteFrontRender->Init(L"sprite/HFHPUI_Front.dds",455.0f,44.0f);
	m_HPSpriteFrontRender->SetPosition({ -621.0f,330.0f,0.0f });
	m_HPSpriteFrontRender->SetPivot({ 0.01f, 0.5f });

	//燃料を表示
	m_FuelSpriteBackRender = NewGO<prefab::CSpriteRender>(0);
	m_FuelSpriteBackRender->Init(L"sprite/HFFuelUI_Back.dds", 455.0f, 33.0f);
	m_FuelSpriteBackRender->SetPosition({ -625.0f,295.0f,0.0f });
	m_FuelSpriteBackRender->SetPivot({ 0.0f,0.5f });

	m_FuelSpriteFrontRender = NewGO<prefab::CSpriteRender>(0);
	m_FuelSpriteFrontRender->Init(L"sprite/HFFuelUI_Front.dds", 455.0f, 33.0f);
	m_FuelSpriteFrontRender->SetPosition({ -621.0f,295.0f,0.0f });
	m_FuelSpriteFrontRender->SetPivot({ 0.01f,0.5f });

	//スピードを表示
	m_speedSpriteBackRender = NewGO<prefab::CSpriteRender>(0);
	m_speedSpriteBackRender->Init(L"sprite/SpeedMeterBack.dds",275,275);
	m_speedSpriteBackRender->SetPosition({ 568.0f,-225.0f,0.0f });

	m_speedSpriteFrontRender = NewGO<prefab::CSpriteRender>(0);
	m_speedSpriteFrontRender->Init(L"sprite/SpeedMeterFront.dds", 275, 275);
	m_speedSpriteFrontRender->SetPosition({ 568.0f,-225.0f,0.0f });

	m_speedFontRender = NewGO<prefab::CFontRender>(0);
	m_speedString = std::to_wstring(int(m_fSpeed));
	m_speedFontRender->SetText(m_speedString.c_str());
	m_speedFontRender->SetScale(0.75f);
	m_speedFontRender->SetPivot({ 0.5f,0.5f });
	m_speedFontRender->SetPosition({ 595.0f,-155.0f });

	//ミッションパネルを表示
	m_missionPanelRender = NewGO<prefab::CSpriteRender>(0);
	m_missionPanelRender->Init(L"sprite/Missions/MissionPanel.dds", 300, 250);
	m_missionPanelRender->SetPosition({ -490.0f,-235.0f,0.0f });
	
	//ミサイル警告のスプライト
	m_missileTrackingSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_missileTrackingSpriteRender->Init(L"sprite/MissileTrackingWarning.dds",1280,720);
	m_missileTrackingSpriteRender->SetScale(CVector3::Zero);

	//煙を生成
	m_smokeGenerator = NewGO<SmokeGenerator>(0, "smokeganerator");

	//照準のスプライトを作成
	m_sightSpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_sightSpriteRender->Init(L"sprite/Sight.dds", 32, 32);
	m_sightSpriteRender->SetScale(CVector3::Zero);


	m_gameScene = FindGO<GameScene>("gamescene");

	//エンジン音
	m_engineSound = NewGO<prefab::CSoundSource>(0);
	m_engineSound->Init(L"sound/Engine.wav");
	m_engineSound->Play(true);
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
	if (m_trackingMissileNum < 0)
		m_trackingMissileNum = 0;

	//ミッションパネルの移動
	if (m_gameScene->m_gameTimer > 10500)
	{

		m_missionPanelRender->SetPosition({ -490.0f,-235.0f - 2 * (m_gameScene->m_gameTimer - 10500),0.0f });
	}

	//HPゲージの変化
	m_HPSpriteFrontRender->SetScale({ m_hp / 500.0f,1.0f,1.0f });

	//スピードの表示
	m_fSpeed += Pad(0).GetRStickYF();

	if (m_fSpeed < 30)
		m_fSpeed = 30;

	if (m_fSpeed > 150)
		m_fSpeed = 150;

	m_fuel -= m_fSpeed / 500;

	if (m_fuel < 0)
	{
		m_fuel = 0;
		m_fSpeed = 30;
	}

	CQuaternion speedSpriteRot;
	speedSpriteRot.SetRotationDeg(CVector3::AxisZ, -12 - (156 * (m_fSpeed-30)/120));

	m_speedSpriteFrontRender->SetRotation(speedSpriteRot);

	m_speedString = std::to_wstring(int(m_fSpeed));
	m_speedFontRender->SetText(m_speedString.c_str());

	if (m_fSpeed < 40)
	{
		m_speedFontRender->SetColor({ 0.0f,0.0f,1.0f,1.0f });
	}
	else if (m_fSpeed > 100)
	{
		m_speedFontRender->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}
	else
	{
		m_speedFontRender->SetColor({ 0.0f,1.0f,0.0f,1.0f });
	}

	//エンジン音をスピードに応じて変化させる。
	m_engineSound->SetFrequencyRatio(max(0.5f,m_fSpeed / 500.0f + 0.3f));
	m_engineSound->SetVolume(max(0.1f, m_fSpeed / 300.0f));

	//燃料ゲージの変化
	m_FuelSpriteFrontRender->SetScale({ min(m_fuel / 1000,1),1.0f,1.0f });

	//ミサイル追尾警告
	if (m_trackingMissileNum > 0 && m_isTurning == false)
	{
		m_missileTrackingSpriteRender->SetScale(CVector3::One);

		if (m_missileEngageSoundFlag == false)
		{
			m_missileEngageSound = NewGO<prefab::CSoundSource>(0);
			m_missileEngageSound->Init(L"sound/MissileEngage.wav");
			m_missileEngageSound->SetVolume(0.2f);
			m_missileEngageSound->SetFrequencyRatio(2.0f);
			m_missileEngageSound->Play(true);

			m_missileEngageSoundFlag = true;
		}
	}
	else
	{
		m_missileTrackingSpriteRender->SetScale(CVector3::Zero);

		if (m_missileEngageSoundFlag == true)
		{
			DeleteGO(m_missileEngageSound);

			m_missileEngageSoundFlag = false;
		}
	}

	if (m_gameScene->m_isBlackholePhase == false)
	{
		if (Pad(0).IsTrigger(enButtonX) && m_isTurning == false)
		{
			m_isTurning = true;
			prefab::CEffect* chaff = NewGO<prefab::CEffect>(0);
			chaff->SetPosition(m_position);
			chaff->Play(L"effect/Chaff.efk");
			chaff->SetScale({ 25.0f,25.0f,25.0f });
			//m_fSpeedBeforeTurn = m_fSpeed;
		}
	}

	//インメルマンターンの動き
	if (m_isTurning)
	{
		if (m_gameScene->m_isBlackholePhase == true)
		{
			m_isTurning = false;
		}

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
		else if(m_turnCount < 135)
		{
			m_z += 4.0f;
		}
		else
		{
			m_turnCount = 0;
			//m_fSpeed = m_fSpeedBeforeTurn;
			m_isTurning = false;
		}
		m_turnCount++;
	}
	else //通常時の動き
	{
		if (m_fSpeed > 100)
		{
			

			if (m_isBoosting == false) {

				m_frictionEffect = NewGO<prefab::CEffect>(0);
				m_frictionEffect->Play(L"effect/Friction.efk");
				m_frictionEffect->SetScale({ 10.0f,10.0f,10.0f });


				m_sbEffectRight = NewGO<prefab::CEffect>(0);
				m_sbEffectRight->Play(L"effect/Sonicboom.efk");
				m_sbEffectRight->SetScale({ 2.5f,2.5f,5.0f });


				m_sbEffectLeft = NewGO<prefab::CEffect>(0);
				m_sbEffectLeft->Play(L"effect/Sonicboom.efk");
				m_sbEffectLeft->SetScale({ 2.5f,2.5f,5.0f });

				prefab::CSoundSource* boostSound = NewGO<prefab::CSoundSource>(0);
				boostSound->Init(L"sound/Boost.wav");
				boostSound->SetVolume(0.5f);
				boostSound->Play(false);
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


		/*
		if (Pad(0).IsPress(enButtonLB2))
		{
			m_fSpeed *= 0;
		}
		*/


		
		if (m_gameScene->m_isBlackholePhase == false)
		{
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


			/*
			if (Pad(0).GetLStickYF() > 0.5)
				m_x++;
			else if (Pad(0).GetLStickYF() < -0.5)
				m_x--;

			if (Pad(0).GetLStickXF() < -0.5)
				m_z++;
			else if (Pad(0).GetLStickXF() > 0.5)
				m_z--;
				*/

			m_x += Pad(0).GetLStickYF();
			m_z -= 1.5 * Pad(0).GetLStickXF();
		}

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
		m_sbEffectRight->SetPosition(m_position + m_rightDir * 7.5f + m_moveDir * -90 + m_upDir * 20);
		m_sbEffectRight->SetRotation(m_rotation);
	}

	if (m_sbEffectLeft != nullptr)
	{
		m_sbEffectLeft->SetPosition(m_position + m_rightDir * -7.5f + m_moveDir * -90 + m_upDir * 20);
		m_sbEffectLeft->SetRotation(m_rotation);
	}

	if (m_jetEffectRight != nullptr)
	{
		m_jetEffectRight->SetPosition(m_position + m_rightDir * 5.5f + m_moveDir * -90 + m_upDir * 15);
		m_jetEffectRight->SetRotation(m_rotation);
		m_jetEffectRight->SetScale({ 5.0f,5.0f,1 + m_fSpeed / 10 });
	}

	if (m_jetEffectLeft != nullptr)
	{
		m_jetEffectLeft->SetPosition(m_position + m_rightDir * -5.5f + m_moveDir * -90 + m_upDir * 15);
		m_jetEffectLeft->SetRotation(m_rotation);
		m_jetEffectLeft->SetScale({ 5.0f,5.0f,1 + m_fSpeed / 10 });
	}

	//ロックオン用の関数
	if (m_gameScene->m_isBlackholePhase == true)
		m_maxLockonDistance = 0.0f;
	else
		m_maxLockonDistance = 40000.0f;

		Lockon();
	
	if (m_isTurning == false)
		m_sightSpriteRender->SetScale(CVector3::One);
	else
		m_sightSpriteRender->SetScale(CVector3::Zero);

	if (FindGO<Blackhole>("blackhole", false) != nullptr)
	{
		m_sightSpriteRender->SetScale(CVector3::Zero);
	}
	


	//ミサイル発射用の関数
	if (Pad(0).IsTrigger(enButtonRB1) && m_lockingEnemy != nullptr && m_lockingEnemy->m_isMortal == false &&m_shotcooldown <= 60)
	{
		ShootMissile();
		m_shotcooldown += 60;
	}
	m_shotcooldown--;
	if (m_shotcooldown < 0)
		m_shotcooldown = 0;


	//デバッグ用
	/*
	std::wstring a = L"Missile = " + std::to_wstring((120 - m_shotcooldown) / 60) + L"\nFUEL = " + std::to_wstring(m_fuel) + L"\nHP = " + std::to_wstring(m_hp) + L"\nSpeed = " + std::to_wstring(m_fSpeed) + L"\nLocked = " + std::to_wstring(m_trackingMissileNum);
	m_fontRender->SetText(a.c_str());
	*/
	

	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);

	//そのフレームごとの移動量を使うので毎フレーム0にする
	m_x = m_y = m_z = 0;

	//ミッション用
	if (m_fSpeed > 149)
	{
		m_highSpeedCount++;
	}
	else
	{
		m_highSpeedCount = 0;
	}

	if (m_fSpeed < 31)
	{
		m_lowSpeedCount++;
	}
	else
	{
		m_lowSpeedCount = 0;
	}

	if (m_oldHp == m_hp)
	{
		m_damageCount++;
	}
	else
	{
		m_damageCount = 0;
	}

	//HPが0になった時削除する
	if (m_hp <= 0)
	{
		prefab::CEffect* deadEffect = NewGO<prefab::CEffect>(0);
		deadEffect->SetPosition(m_position + MainCamera().GetForward() * 100);
		deadEffect->SetScale({ 100.0f,100.0f,100.0f });
		deadEffect->Play(L"effect/Explosion.efk");

		prefab::CSoundSource* explosionSound = NewGO<prefab::CSoundSource>(0);
		explosionSound->Init(L"sound/Boom2.wav");
		explosionSound->SetVolume(0.4f);
		explosionSound->Play(false);

		NewGO<GameOver>(0, "gameover");

		if (m_missileEngageSoundFlag)
			DeleteGO(m_missileEngageSound);

		DeleteGO(this);
	}

	m_oldHp = m_hp;
}

void Player::Lockon()
{
	//最高ロックオン可能距離を設定する
	CVector3 Lockonpos = m_position + m_moveDir * m_maxLockonDistance;

	//ロックオン可能な角度を設定
	const int lockondegree = 20;

	//最高ロックオン可能距離より敵の位置までの距離が短いかつロックオン可能な角度内ならロックオン距離を更新する処理を繰り返す
	QueryGOs<IEnemy>("enemy", [&Lockonpos,lockondegree,this](IEnemy* enemy)->bool
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
	
	if (m_isTurning)
	{
		m_lockonSpriteRender->SetScale(CVector3::Zero);
		m_lockingEnemy = nullptr;
	}
	else if (Check.Length())
	{
		//dbg::DrawVector(Lockonpos - m_position, m_position, "locking");
		MainCamera().CalcScreenPositionFromWorldPosition(spritepos, Lockonpos);
		CVector3 screenpos = { spritepos.x,spritepos.y,0.0f };
		m_lockonSpriteRender->SetPosition(screenpos);

		CVector3 distance = Lockonpos - m_position;

		//距離に応じてロックオンマーカーの大きさを変更
		m_lockonSpriteRender->SetScale({ 50 / (distance.Length() / 100) ,50 / (distance.Length() / 100) ,50 / (distance.Length() / 100) });
		m_lockonSpriteRender->SetScale({1.0f,1.0f,1.0f });

		if (m_lockingEnemy != m_oldLockingEnemy)
		{
			m_lockonSound = NewGO<prefab::CSoundSource>(0);
			m_lockonSound->Init(L"sound/Lock.wav");
			m_lockonSound->SetVolume(0.5f);
			m_lockonSound->Play(false);
		}
	}
	//できていないならロックオンマーカーを非表示に
	else
	{	
		m_lockonSpriteRender->SetScale(CVector3::Zero);
		m_lockingEnemy = nullptr;
	}

	m_oldLockingEnemy = m_lockingEnemy;
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

	m_missileShotSound = NewGO<prefab::CSoundSource>(0);
	m_missileShotSound->Init(L"sound/Missile.wav");
	m_missileShotSound->SetVolume(0.3f);
	m_missileShotSound->Play(false);

	missile->m_fSpeed = max(10,m_fSpeed - 30);

	//ミサイルに追尾先の敵を教える
	missile->m_trackingEnemy = m_lockingEnemy;
	//撃った瞬間相手に死にゆく定めを付与する
	m_lockingEnemy->m_isMortal = true;

	//ミッション用
	m_shootMissileNum++;
}