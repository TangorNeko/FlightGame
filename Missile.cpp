#include "stdafx.h"
#include "Missile.h"
#include "LaserEnemy.h"
#include "MissileEnemy.h"
#include "GameScoreManager.h"

void Missile::OnDestroy()
{
	
	DeleteGO(m_effect2);

	DeleteGO(m_skinModelRender);
}

bool Missile::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Missile.cmo");
	m_effect2 = NewGO<prefab::CEffect>(0);
	m_skinModelRender->SetScale({ 1.5f,1.5f,1.5f });
	m_gamescoremanager = FindGO<GameScoreManager>("gamescoremanager");
	return true;
}

void Missile::Update()
{	
	
	if (m_effect2->IsPlay() == false)
	{
		m_effect2->Play(L"effect/Missile.efk");
	}

	//敵を追尾する
	CVector3 diff = m_moveDir = m_trackingEnemy->m_position - m_position;
	m_moveDir.Normalize();
	m_fSpeed += m_fSpeed / 10;
	if (m_fSpeed > 300)
	{
		m_fSpeed = 300;
	}
	m_position += m_moveDir * m_fSpeed;
	m_skinModelRender->SetPosition(m_position);

	m_effect2->SetPosition(m_position + m_moveDir * -25.0f);
	m_effect2->SetScale({ 15.0f,15.0f,15.0f });


	//ミッション用
	/*
	if (m_isAttackDirChecked == false)
	{
		float attackDeg = m_moveDir.Dot(m_trackingEnemy->m_moveDir);

		if (attackDeg > 0)
		{
			m_isBackAttack = true;
		}
		else
		{
			m_isBackAttack = false;
		}

		m_isAttackDirChecked = true;
	}
	*/

	//敵の方に向く
	CVector3 zx = { m_moveDir.x,0.0f,m_moveDir.z };
	zx.Normalize();

	//x軸の回転
	float degx = acos(m_moveDir.Dot(zx));
	//角度しか手に入らないので向いてる方向が下なら負の数にする
	if (m_moveDir.y > 0)
		degx = -degx;
	CQuaternion qRotx;
	qRotx.SetRotation(CVector3::AxisX, degx - odegx);

	//y軸の回転
	float degy = acos(CVector3::Front.Dot(zx));
	//角度しか手に入らないので向いてる方向が左なら負の数にする
	if (m_moveDir.x < 0)
		degy = -degy;
	CQuaternion qRoty;
	qRoty.SetRotation(CVector3::AxisY, degy - odegy);

	m_qRot.Multiply(qRotx, m_qRot);
	m_qRot.Multiply(qRoty, m_qRot);

	m_skinModelRender->SetRotation(m_qRot);
	m_effect2->SetRotation(m_qRot);

	odegx = degx;
	odegy = degy;

	
	//敵の近くまで来たら
	if (diff.Length() < 200)
	{
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->SetPosition(m_position);
		effect->Play(L"effect/Explosion.efk");
		effect->SetScale({ 100.0f,100.0f,100.0f });

		prefab::CSoundSource* explosionSound = NewGO<prefab::CSoundSource>(0);
		explosionSound->Init(L"sound/Boom3.wav");
		explosionSound->SetVolume(0.4f);
		explosionSound->Play(false);


		//ミッション用
		if (m_trackingEnemy->m_isUFO == true)
		{
			if (m_trackingEnemy->m_isAttackSuccess == false)
			{
				IEnemy::s_surpriseUFONum++;
			}

			m_gamescoremanager->m_gameScore += 200;
		}
		else
		{
			if (m_trackingEnemy->m_isAttackSuccess == false)
			{
				IEnemy::s_surpriseFighterNum++;
			}

			m_gamescoremanager->m_gameScore += 500;
		}

		IEnemy::s_killEnemyNum++;

		DeleteGO(m_trackingEnemy);
		DeleteGO(this);
	}
}