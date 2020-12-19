#include "stdafx.h"
#include "TitlePlayer.h"

void TitlePlayer::OnDestroy()
{
	DeleteGO(m_skinModelRender);

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

	return true;
}

void TitlePlayer::Update()
{
	//スピード

	if (m_isBoosting == false)
		m_fSpeed += Pad(0).GetRStickYF() * 0.25;
	else
		m_fSpeed += 0.25;

	if (m_fSpeed < 0)
		m_fSpeed = 0;

	if (m_fSpeed > 150)
		m_fSpeed = 150;


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

	/*
	if (Pad(0).IsPress(enButtonLB2))
	{
		m_fSpeed *= 0;
	}
	*/


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

	if (m_isBoosting)
	{
		m_position += m_moveDir * m_fSpeed;
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
			m_jetEffectRight->SetScale({0.0f,0.0f,0.0f });
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