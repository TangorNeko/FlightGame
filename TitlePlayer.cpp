#include "stdafx.h"
#include "TitlePlayer.h"
#include "TitleScene.h"

void TitlePlayer::OnDestroy()
{
	DeleteGO(m_speedFontRender);
	DeleteGO(m_speedSpriteBackRender);
	DeleteGO(m_speedSpriteFrontRender);
	DeleteGO(m_HPSpriteBackRender);
	DeleteGO(m_HPSpriteFrontRender);
	DeleteGO(m_FuelSpriteBackRender);
	DeleteGO(m_FuelSpriteFrontRender);
	DeleteGO(m_skinModelRender);
	DeleteGO(m_missionPanelRender);

	DeleteGO(m_engineSound);

	if (m_frictionEffect != nullptr && m_frictionEffect->IsPlay() == true)
		DeleteGO(m_frictionEffect);

	if (m_jetEffectRight != nullptr && m_jetEffectRight->IsPlay() == true)
		DeleteGO(m_jetEffectRight);

	if (m_jetEffectLeft != nullptr && m_jetEffectLeft->IsPlay() == true)
		DeleteGO(m_jetEffectLeft);

}

bool TitlePlayer::Start()
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

	//ジェットエフェクト
	m_jetEffectRight = NewGO<prefab::CEffect >(0);
	m_jetEffectRight->Play(L"effect/Jet.efk");

	m_jetEffectLeft = NewGO<prefab::CEffect>(0);
	m_jetEffectLeft->Play(L"effect/Jet.efk");


	//HPバーを表示
	m_HPSpriteBackRender = NewGO<prefab::CSpriteRender>(0);
	m_HPSpriteBackRender->Init(L"sprite/HFHPUI_Back.dds", 455.0f, 44.0f);
	m_HPSpriteBackRender->SetPosition({ -1500.0f,330.0f,0.0f });
	m_HPSpriteBackRender->SetPivot({ 0.0f, 0.5f });

	m_HPSpriteFrontRender = NewGO<prefab::CSpriteRender>(0);
	m_HPSpriteFrontRender->Init(L"sprite/HFHPUI_Front.dds", 455.0f, 44.0f);
	m_HPSpriteFrontRender->SetPosition({ -1500.0f,330.0f,0.0f });
	m_HPSpriteFrontRender->SetPivot({ 0.0f, 0.5f });

	//燃料バーを表示
	m_FuelSpriteBackRender = NewGO<prefab::CSpriteRender>(0);
	m_FuelSpriteBackRender->Init(L"sprite/HFFuelUI_Back.dds", 455.0f, 33.0f);
	m_FuelSpriteBackRender->SetPosition({ -1750.0f,295.0f,0.0f });
	m_FuelSpriteBackRender->SetPivot({ 0.0f,0.5f });

	m_FuelSpriteFrontRender = NewGO<prefab::CSpriteRender>(0);
	m_FuelSpriteFrontRender->Init(L"sprite/HFFuelUI_Front.dds", 455.0f, 33.0f);
	m_FuelSpriteFrontRender->SetPosition({ -1750.0f,295.0f,0.0f });
	m_FuelSpriteFrontRender->SetPivot({ 0.0f,0.5f });

	//スピードメーターを表示
	m_speedSpriteBackRender = NewGO<prefab::CSpriteRender>(0);
	m_speedSpriteBackRender->Init(L"sprite/SpeedMeterBack.dds", 275, 275);
	m_speedSpriteBackRender->SetPosition({ 1000.0f,-225.0f,0.0f });

	m_speedSpriteFrontRender = NewGO<prefab::CSpriteRender>(0);
	m_speedSpriteFrontRender->Init(L"sprite/SpeedMeterFront.dds", 275, 275);
	m_speedSpriteFrontRender->SetPosition({ 1000.0f,-225.0f,0.0f });

	m_speedFontRender = NewGO<prefab::CFontRender>(0);
	m_speedString = std::to_wstring(int(m_fSpeed));
	m_speedFontRender->SetText(m_speedString.c_str());
	m_speedFontRender->SetScale(0.75f);
	m_speedFontRender->SetPivot({ 0.5f,0.5f });
	m_speedFontRender->SetPosition({ 1027.0f,-155.0f });

	//ミッションパネルを表示
	m_missionPanelRender = NewGO<prefab::CSpriteRender>(0);
	m_missionPanelRender->Init(L"sprite/Missions/MissionPanel.dds", 300, 250);
	m_missionPanelRender->SetPosition({ -490.0f,-1000.0f,0.0f });

	//スタート画面の文字
	m_startSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_startSpriteRender->Init(L"sprite/HFStart.dds", 535.0f, 55.0f);
	m_startSpriteRender->SetPosition({ 0.0f,-200.0f,0.0f });
	m_startSpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });

	//タイトルロゴ
	m_titleSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_titleSpriteRender->Init(L"sprite/HFTitle.dds", 500.0f, 250.0f);
	m_titleSpriteRender->SetPosition({ 0.0f,200.0f,0.0f });
	m_titleSpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });

	m_titleScene = FindGO<TitleScene>("titlescene");

	//エンジン音
	m_engineSound = NewGO<prefab::CSoundSource>(0);
	m_engineSound->Init(L"sound/Engine.wav");
	m_engineSound->SetVolume(0.0f);
	m_engineSound->Play(true);

	return true;
}

void TitlePlayer::Update()
{

		//スピード
		if (m_isBoosting == false)
			m_fSpeed += Pad(0).GetRStickYF() * 0.5;
		else
			m_fSpeed += 0.5;

		if (m_fSpeed < 0)
			m_fSpeed = 0;

		if (m_fSpeed > 150)
			m_fSpeed = 150;

		//エンジン音をスピードに応じて変化させる。
		m_engineSound->SetFrequencyRatio(max(0.5f, m_fSpeed / 500.0f + 0.3f));
		m_engineSound->SetVolume(max(0.0f, m_fSpeed / 300.0f));


		CQuaternion speedSpriteRot;
		speedSpriteRot.SetRotationDeg(CVector3::AxisZ, min(-12 - (156 * (m_fSpeed - 30) / 120), -12));
		m_speedSpriteFrontRender->SetRotation(speedSpriteRot);

		m_speedString = std::to_wstring(int(m_fSpeed));
		m_speedFontRender->SetText(m_speedString.c_str());

		/*
		m_startFontRender->SetPosition({ 0.0f ,-150.0f });
		m_startFontRender->SetScale(1.0f);
		*/


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

		if (m_isBoosting == false)
		{
			if (m_fontAlpha < 1.0f)
				m_fontAlpha += 0.05f;

			m_titleSpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,m_fontAlpha });
			m_startSpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,m_fontAlpha });
		}
		else
		{
			m_position += m_moveDir * m_fSpeed;

			//スプライトを移動
			m_spriteMoveCount += 5;

			m_HPSpriteBackRender->SetPosition({ min(-1500.0f + m_spriteMoveCount,-625.0f),330.0f,0.0f });
			m_HPSpriteFrontRender->SetPosition({ min(-1500.0f + m_spriteMoveCount,-625.0f),330.0f,0.0f });
			m_FuelSpriteBackRender->SetPosition({ min(-1750.0f + m_spriteMoveCount,-625.0f),295.0f,0.0f });
			m_FuelSpriteFrontRender->SetPosition({ min(-1750.0f + m_spriteMoveCount,-625.0f),295.0f,0.0f });
			m_speedSpriteBackRender->SetPosition({ max(1000.0f - m_spriteMoveCount,568.0f),-225.0f,0.0f });
			m_speedSpriteFrontRender->SetPosition({ max(1000.0f - m_spriteMoveCount,568.0f),-225.0f,0.0f });
			m_speedFontRender->SetPosition({ max(1027.0f - m_spriteMoveCount,595.0f),-155.0f });
			m_missionPanelRender->SetPosition({ -490.0f,min(-1000.0f + m_spriteMoveCount,-235.0f),0.0f });

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

			if (m_fontAlpha > 0.0f)
				m_fontAlpha -= 0.025f;

			m_titleSpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,m_fontAlpha });
			m_startSpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,m_fontAlpha });
		}

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
			m_jetEffectRight->SetPosition(m_position + m_rightDir * 5.5f + m_moveDir * -90 + m_upDir * 15);
			m_jetEffectRight->SetRotation(m_rotation);
			if (m_fSpeed == 0)
			{
				m_jetEffectRight->SetScale({ 0.0f,0.0f,0.0f });
			}
			else
			{
				m_jetEffectRight->SetScale({ 5.0f,5.0f,1 + m_fSpeed / 10 });
			}
		}

		if (m_jetEffectLeft != nullptr)
		{
			m_jetEffectLeft->SetPosition(m_position + m_rightDir * -5.5f + m_moveDir * -90 + m_upDir * 15);
			m_jetEffectLeft->SetRotation(m_rotation);
			if (m_fSpeed == 0)
			{
				m_jetEffectLeft->SetScale({ 0.0f,0.0f,0.0f });
			}
			else
			{
				m_jetEffectLeft->SetScale({ 5.0f,5.0f,1 + m_fSpeed / 10 });
			}
		}


		m_skinModelRender->SetRotation(m_rotation);
		m_skinModelRender->SetPosition(m_position);

		//そのフレームごとの移動量を使うので毎フレーム0にする
		m_x = m_y = m_z = 0;
	
}